/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_nodes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelidrys <aelidrys@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 10:13:46 by aelidrys          #+#    #+#             */
/*   Updated: 2023/05/25 19:05:19 by yrimah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static t_cmd	*check_redirec(t_cmd *node, char **a[2], int *i)
{
	if (a[0][*i])
	{
		if (check_double_append(a, i, node))
			return (node);
		else if (check_single_append(a, i, node))
			return (node);
		else if (check_double_input(a, i, node))
			return (node);
		else if (check_single_input(a, i, node))
			return (node);
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

t_list	*fill_nodes(char **args, int i)
{
	t_list	*cmds[2];
	char	**temp[2];
	int		j;

	if (!help_fill1(args, &temp[1], &cmds[0]))
		return (NULL);
	while (args[++i])
	{
		help_fill2(cmds, &i, args, &temp[0]);
		j = i;
		cmds[1]->content = check_redirec(cmds[1]->content, temp, &i);
		if (i < 0)
		{
			((t_cmd *)cmds[1]->content)->s = -1;
			i = j;
			while (args[i] && args[i + 1] && !str_comp(args[i + 1], "|"))
				i++;
		}
		if (!args[i])
			break ;
	}
	help_free(args, &temp[1]);
	return (cmds[0]);
}
