/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_nodes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelidrys <aelidrys@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 10:13:46 by aelidrys          #+#    #+#             */
/*   Updated: 2023/05/19 18:17:11 by aelidrys         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static t_cmd	*mini_init(void)
{
	t_cmd	*mini;

	mini = malloc(sizeof(t_cmd));
	if (!mini)
		return (NULL);
	mini->cmmd = NULL;
	mini->path = NULL;
	mini->next = NULL;
	mini->in = STDIN_FILENO;
	mini->out = STDOUT_FILENO;
	return (mini);
}

static t_cmd	*get_params(t_cmd *node, char **a[2], int *i)
{
	t_herdoc	*tmp;

	if (a[0][*i])
	{
		if (a[0][*i][0] == '>' && a[0][*i + 1] && a[0][*i + 1][0] == '>')
			node = get_outf2(node, a[1], i);
		else if (a[0][*i][0] == '>')
			node = get_outf1(node, a[1], i);
		else if (a[0][*i][0] == '<' && a[0][*i + 1][0] == '<')
		{
			tmp = shell->hdc;
			node->in = shell->hdc->in;
			shell->hdc = shell->hdc->next;
			free(tmp);
			*i += 2;
		}
		else if (a[0][*i][0] == '<')
			node = get_inf1(node, a[1], i);
		else if (a[0][*i][0] != '|')
			node->cmmd = ft_ext_mat(node->cmmd, a[1][*i]);
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

t_list	*stop_fill(t_list *cmds, char **args, char **temp)
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
	// for (int b = 0; args[b]; b++)
	// 	printf("%s\n", args[b]);
	temp[1] = get_trimmed(args);
	if (!open_here_doc(args, -1))
	{
		ft_free_envp(&temp[1]);
		ft_free_envp(&args);
		return (NULL);
	}
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
		int j = i;
		cmds[1]->content = get_params(cmds[1]->content, temp, &i);
		if (i < 0){
			((t_cmd *)cmds[1]->content)->s = -1;
			i = j;
			while (args[i] && args[i + 1] && !str_comp(args[i + 1], "|"))
				i++;
		}
		if (!args[i])
			break ;
	}
	ft_free_envp(&temp[1]);
	ft_free_envp(&args);
	return (cmds[0]);
}
