/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrimah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 18:33:55 by yrimah            #+#    #+#             */
/*   Updated: 2023/03/31 18:33:58 by yrimah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
# include "../includes/minishell.h"

// char	*mini_getenv(char *var, char **envp, int n)
// {
// 	int	i;
// 	int	n2;

// 	i = 0;
// 	if (n < 0)
// 		n = ft_strlen(var);
// 	while (!ft_strchr(var, '=') && envp && envp[i])
// 	{
// 		n2 = n;
// 		if (n2 < ft_strchr_i(envp[i], '='))
// 			n2 = ft_strchr_i(envp[i], '=');
// 		if (!ft_strncmp(envp[i], var, n2))
// 			return (ft_substr(envp[i], n2 + 1, ft_strlen(envp[i])));
// 		i++;
// 	}
// 	return (NULL);
// }

// static char	*get_substr_var(char *str, int i, t_shell *prompt)
// {
// 	char	*aux;
// 	int		pos;
// 	char	*path;
// 	char	*var;

// 	pos = ft_strchars_i(&str[i], "|\"\'$?>< ") + (ft_strchr("$?", str[i]) != 0);
// 	if (pos == -1)
// 		pos = ft_strlen(str) - 1;
// 	aux = ft_substr(str, 0, i - 1);
// 	var = mini_getenv(&str[i], prompt->envp, \
// 		ft_strchars_i(&str[i], "\"\'$|>< "));
// 	if (!var && str[i] == '$')
// 		var = ft_itoa(prompt->pid);
// 	else if (!var && str[i] == '?')
// 		var = ft_itoa(g_status);
// 	path = ft_strjoin(aux, var);
// 	free(aux);
// 	aux = ft_strjoin(path, &str[i + pos]);
// 	free(var);
// 	free(path);
// 	free(str);
// 	return (aux);
// }

// char	*expand_vars(char *str, int i, int quotes[2], t_shell *prompt)
// {
// 	quotes[0] = 0;
// 	quotes[1] = 0;
// 	while (str && str[++i])
// 	{
// 		quotes[0] = (quotes[0] + (!quotes[1] && str[i] == '\'')) % 2;
// 		quotes[1] = (quotes[1] + (!quotes[0] && str[i] == '\"')) % 2;
// 		if (!quotes[0] && str[i] == '$' && str[i + 1] && \
// 			((ft_strchars_i(&str[i + 1], "/~%^{}:; ") && !quotes[1]) || \
// 			(ft_strchars_i(&str[i + 1], "/~%^{}:;\"") && quotes[1])))
// 			return (expand_vars(get_substr_var(str, ++i, prompt), -1, \
// 				quotes, prompt));
// 	}
// 	return (str);
// }

// char	*expand_path(char *str, int i, int quotes[2], char *var)
// {
// 	char	*path;
// 	char	*aux;

// 	quotes[0] = 0;
// 	quotes[1] = 0;
// 	while (str && str[++i])
// 	{
// 		quotes[0] = (quotes[0] + (!quotes[1] && str[i] == '\'')) % 2;
// 		quotes[1] = (quotes[1] + (!quotes[0] && str[i] == '\"')) % 2;
// 		if (!quotes[0] && !quotes[1] && str[i] == '~' && (i == 0 || \
// 			str[i - 1] != '$'))
// 		{
// 			aux = ft_substr(str, 0, i);
// 			path = ft_strjoin(aux, var);
// 			free(aux);
// 			aux = ft_substr(str, i + 1, ft_strlen(str));
// 			free(str);
// 			str = ft_strjoin(path, aux);
// 			free(aux);
// 			free(path);
// 			return (expand_path(str, i + ft_strlen(var) - 1, quotes, var));
// 		}
// 	}
// 	free(var);
// 	return (str);
// }

static char	**split_all(char **args, t_shell *prompt)
{
	char	**subsplit;
	int		i;
	int		quotes[2];

	i = -1;
	while (args && args[++i])
	{
		//var expander edit
		args[i] = expand_vars(args[i], -1, quotes, prompt);
		//path expander fixed space
		args[i] = expand_path(args[i], -1, quotes, mini_getenv("HOME", prompt->envp, 4));
		subsplit = ft_split_command(args[i], "|><");
		ft_mat_rep_in(&args, subsplit, i);
		i += ft_rows_number(subsplit) - 1;
		ft_free_envp(&subsplit);
	}
	return (args);
}
// int ft_strequ(const char *s1, const char *s2)
// {
//     if (!s1 || !s2)
//         return (0);
//     return (strcmp(s1, s2) == 0);
// }

// static char	**split_all(char **args, t_shell *prompt)
// {
//     char	**subsplit;
//     int		i;
//     int		quotes[2];
//     int		in_heredoc = 0;
//     char	*heredoc_delim = NULL;

//     i = -1;
//     while (args && args[++i])
//     {
//         if (!in_heredoc)
//         {
//             args[i] = expand_vars(args[i], -1, quotes, prompt);
//             args[i] = expand_path(args[i], -1, quotes, \
//                 mini_getenv("HOME", prompt->envp, 4));
//         }
        
//         if (ft_strequ(args[i], "<<") && args[i + 1])
//         {
//             in_heredoc = 1;
//             heredoc_delim = ft_strdup(args[i + 1]);
//             i++;
//         }
//         else if (ft_strequ(args[i], heredoc_delim))
//         {
//             in_heredoc = 0;
//             heredoc_delim = NULL;
//         }

//         if (!in_heredoc)
//         {
//             subsplit = ft_split_command(args[i], "<|>");
//             ft_mat_rep_in(&args, subsplit, i);

//             i += ft_rows_number(subsplit) - 1;
//             ft_free_envp(&subsplit);
//         }
//     }

//     prompt->in_heredoc = in_heredoc;
//     prompt->heredoc_delim = heredoc_delim;

//     return (args);
// }


//test function
void print_mishell(t_mishell *ms)
{
    printf("full_command: ");
    char **cmd = ms->full_command;
    while (*cmd)
    {
        printf("%s ", *cmd);
        cmd++;
    }
    printf("\n");
    printf("full_path: %s\n", ms->full_path);
    printf("in: %d\n", ms->in);
    printf("out: %d\n", ms->out);
}
//test function
void	print_cmd(t_list *cmd)
{
    while (cmd)
    {
        print_mishell((t_mishell *)cmd->content);
        cmd = cmd->next;
    }
}

void	parsing(char **args, t_shell *sh)
{
	args = split_all(args, sh);
	sh->commands = fill_nodes(args, -1);
	//just a test
	printf("list size: %d\n",ft_lstsize(sh->commands));//
	if (sh->commands)//
	{
		t_list *node2 = sh->commands;
		// test function
		print_cmd(node2);
	}
}