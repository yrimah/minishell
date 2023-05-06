/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrimah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/17 17:44:29 by yrimah            #+#    #+#             */
/*   Updated: 2023/04/17 17:44:39 by yrimah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../includes/minishell.h"

char	*mini_getenv(char *var, char **envp, int n)
{
	int	i;
	int	n2;

	i = 0;
	if (n < 0)
		n = ft_strlen(var);
	while (!ft_strchr(var, '=') && envp && envp[i])
	{
		n2 = n;
		if (n2 < ft_strchr_i(envp[i], '='))
			n2 = ft_strchr_i(envp[i], '=');
		if (!ft_strncmp(envp[i], var, n2))
			return (ft_substr(envp[i], n2 + 1, ft_strlen(envp[i])));
		i++;
	}
	return (NULL);
}

static char	*get_substr_var(char *str, int i, t_shell *prompt)
{
	char	*aux;
	int		pos;
	char	*path;
	char	*var;

	pos = ft_strchars_i(&str[i], "|\"\'$?>< ") + (ft_strchr("$?", str[i]) != 0);
	if (pos == -1)
		pos = ft_strlen(str) - 1;
	aux = ft_substr(str, 0, i - 1);
	var = mini_getenv(&str[i], prompt->envp, \
		ft_strchars_i(&str[i], "\"\'$|>< "));
	if (!var && str[i] == '$')
		var = ft_itoa(prompt->pid);
	else if (!var && str[i] == '?')
		var = ft_itoa(g_status);
	path = ft_strjoin(aux, var);
	free(aux);
	aux = ft_strjoin(path, &str[i + pos]);
	free(var);
	free(path);
	free(str);
	return (aux);
}

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

// char	*expand_vars(char *str, int i, int quotes[2], t_shell *prompt)
// {
// 	quotes[0] = 0;
// 	quotes[1] = 0;
// 	while (str && str[++i])
// 	{
// 		quotes[0] += (!quotes[1] && str[i] == '\'');
// 		quotes[1] += (!quotes[0] && str[i] == '\"');
// 		if (quotes[0] % 2 == 0 && quotes[1] % 2 == 0 && str[i] == '$' && str[i + 1])
// 		{
// 			if (ft_strchars_i("/~%^{}:; ", &str[i + 1]) && !quotes[1])
// 				return (expand_vars(get_substr_var(str, ++i, prompt), -1, quotes, prompt));
// 			if (ft_strchars_i("/~%^{}:;\"", &str[i + 1]) && quotes[1])
// 				return (expand_vars(get_substr_var(str, ++i, prompt), -1, quotes, prompt));
// 		}
// 	}
// 	return (str);
// }

char	*expand_vars(char *str, int i, int quotes[2], t_shell *prompt)
{
    quotes[0] = 0;
    quotes[1] = 0;
    while (str && str[++i])
    {
        quotes[0] += (!quotes[1] && str[i] == '\'');
        quotes[1] += (!quotes[0] && str[i] == '\"');
        if (quotes[0] % 2 == 0 && str[i] == '$' && str[i + 1])
        {
            if (!quotes[1] && ft_strchars_i("/~%^{}:; ", &str[i + 1]))
                return (expand_vars(get_substr_var(str, ++i, prompt), -1, quotes, prompt));
            else if (quotes[1] && ft_strchars_i("/~%^{}:;\"", &str[i + 1]))
                return (expand_vars(get_substr_var(str, ++i, prompt), -1, quotes, prompt));
        }
    }
    return (str);
}

// char	*expand_vars(char *str, int i, int quotes[2], t_shell *prompt, char *heredoc_delim)
// {
//     quotes[0] = 0;
//     quotes[1] = 0;
//     while (str && str[++i])
//     {
//         quotes[0] += (!quotes[1] && str[i] == '\'');
//         quotes[1] += (!quotes[0] && str[i] == '\"');
//         if (quotes[0] % 2 == 0 && quotes[1] % 2 == 0 && str[i] == '$' && str[i + 1])
//         {
//             if (ft_strchars_i("/~%^{}:; ", &str[i + 1]) && !quotes[1])
//             {
//                 char *var = get_substr_var(str, ++i, prompt);
//                 if (!heredoc_delim || ft_strcmp(var, heredoc_delim) != 0)
//                     return (expand_vars(var, -1, quotes, prompt, heredoc_delim));
//             }
//             if (ft_strchars_i("/~%^{}:;\"", &str[i + 1]) && quotes[1])
//             {
//                 char *var = get_substr_var(str, ++i, prompt);
//                 if (!heredoc_delim || ft_strcmp(var, heredoc_delim) != 0)
//                     return (expand_vars(var, -1, quotes, prompt, heredoc_delim));
//             }
//         }
//     }
//     return (str);
// }

// char	*expand_vars(char *str, int i, int quotes[2], t_shell *shell)
// {
// 	quotes[0] = 0;
// 	quotes[1] = 0;
// 	while (str && str[++i])
// 	{
// 		quotes[0] += (!quotes[1] && str[i] == '\'');
// 		quotes[1] += (!quotes[0] && str[i] == '\"');
// 		if (shell->in_heredoc && !ft_strncmp(&str[i], shell->heredoc_delim, ft_strlen(shell->heredoc_delim)))
// 		{
// 			if (i > 0 && str[i - 1] == '\\')
// 				continue;
// 			shell->in_heredoc = 0;
// 			i += ft_strlen(shell->heredoc_delim) - 1;
// 			continue;
// 		}
// 		if (quotes[0] % 2 == 0 && str[i] == '$' && str[i + 1])
// 		{
// 			if (ft_strchars_i("/~%^{}:; ", &str[i + 1]) && !quotes[1])
// 				str = expand_vars(get_substr_var(str, ++i, shell), -1, quotes, shell);
// 			else if (ft_strchars_i("/~%^{}:;\"", &str[i + 1]) && quotes[1])
// 				str = expand_vars(get_substr_var(str, ++i, shell), -1, quotes, shell);
// 		}
// 	}
// 	return (str);
// }



char	*expand_path(char *str, int i, int quotes[2], char *var)
{
	char	*path;
	char	*aux;

	quotes[0] = 0;
	quotes[1] = 0;
	while (str && str[++i])
	{
		quotes[0] = (quotes[0] + (!quotes[1] && str[i] == '\'')) % 2;
		quotes[1] = (quotes[1] + (!quotes[0] && str[i] == '\"')) % 2;
		// if (!quotes[0] && !quotes[1] && str[i] == '~' && (i == 0 || \
		// 	str[i - 1] != '$' || str[i - 1] == ' '))
		if (!quotes[0] && !quotes[1] && str[i] == '~' && (i == 0 || str[i - 1] == ' ' || str[i - 1] == '$'))
		{
			aux = ft_substr(str, 0, i);
			path = ft_strjoin(aux, var);
			free(aux);
			aux = ft_substr(str, i + 1, ft_strlen(str));
			free(str);
			str = ft_strjoin(path, aux);
			free(aux);
			free(path);
			return (expand_path(str, i + ft_strlen(var) - 1, quotes, var));
		}
	}
	free(var);
	return (str);
}
