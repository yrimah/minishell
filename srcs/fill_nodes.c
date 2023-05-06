#include "../includes/minishell.h"

static t_mishell	*mini_init(void)
{
	t_mishell	*mini;

	mini = malloc(sizeof(t_mishell));
	if (!mini)
		return (NULL);
	mini->full_command = NULL;
	mini->full_path = NULL;
	mini->in = STDIN_FILENO;
	mini->out = STDOUT_FILENO;
	return (mini);
}

static t_mishell	*get_params(t_mishell *node, char **a[2], int *i)
{
	if (a[0][*i])
	{
		if (a[0][*i][0] == '>' && a[0][*i + 1] && a[0][*i + 1][0] == '>')
			node = get_outf2(node, a[1], i);
		else if (a[0][*i][0] == '>')
			node = get_outf1(node, a[1], i);
		else if (a[0][*i][0] == '<' && a[0][*i + 1] && \
			a[0][*i + 1][0] == '<')
			node = get_inf2(node, a[1], i);
		else if (a[0][*i][0] == '<')
			node = get_inf1(node, a[1], i);
		else if (a[0][*i][0] != '|')
			node->full_command = ft_ext_mat(node->full_command, a[1][*i]);
		else
		{
			error_handling(10, NULL);
			*i = -2;
		}
		return (node);
	}
	error_handling(10, NULL);
	*i = -2;
	return (node);
}

static char	**get_trimmed(char **args)
{
	char	**temp;
	char	*aux;
	int		j;

	j = -1;
	temp = ft_dup_envp(args);
	while (temp && temp[++j])
	{
		aux = ft_last_split(temp[j], 0, 0);
		free(temp[j]);
		temp[j] = aux;
	}
	return (temp);
}

static t_list	*stop_fill(t_list *cmds, char **args, char **temp)
{
	ft_lstclear(&cmds, free_nodes_content);
	ft_free_envp(&temp);
	ft_free_envp(&args);
	return (NULL);
}

t_list	*fill_nodes(char **args, int i)
{
	t_list	*cmds[2];
	char	**temp[2];
	
	cmds[0] = NULL;
	temp[1] = get_trimmed(args);
	while (args[++i])
	{
		cmds[1] = ft_lstlast(cmds[0]);
		if (i == 0 || (args[i][0] == '|' && args[i + 1] && args[i + 1][0]))
		{
			i += args[i][0] == '|';
			ft_lstadd_back(&cmds[0], ft_lstnew(mini_init()));
			cmds[1] = ft_lstlast(cmds[0]);
		}
		temp[0] = args;
		cmds[1]->content = get_params(cmds[1]->content, temp, &i);
		if (i < 0)
			return (stop_fill(cmds[0], args, temp[1]));
		if (!args[i])
			break ;
	}
	ft_free_envp(&temp[1]);
	ft_free_envp(&args);
	return (cmds[0]);
}
