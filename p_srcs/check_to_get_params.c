/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_to_get_params.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrimah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 19:02:52 by yrimah            #+#    #+#             */
/*   Updated: 2023/05/25 19:03:01 by yrimah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_double_append(char **a[2], int *i, t_cmd *node)
{
	if (a[0][*i][0] == '>' && a[0][*i + 1] && a[0][*i + 1][0] == '>')
	{
		if (str_comp(&a[0][*i + 2][0], ">"))
		{
			error_handling(12, NULL);
			shell->g_status = 258;
			node->out = -1;
			return (1);
		}
		else if (str_comp(&a[0][*i + 2][0], "<"))
		{
			error_handling(11, NULL);
			shell->g_status = 258;
			node->out = -1;
			return (1);
		}
		else
			node = get_outf2(node, a[1], i);
	}
	return (0);
}

int	check_single_append(char **a[2], int *i, t_cmd *node)
{
	if (a[0][*i][0] == '>')
	{
		node = get_outf1(node, a[1], i);
		return (1);
	}
	return (0);
}

int	check_double_input(char **a[2], int *i, t_cmd *node)
{
	t_herdoc	*tmp;

	if (a[0][*i][0] == '<' && a[0][*i + 1] && a[0][*i + 1][0] == '<')
	{
		tmp = shell->hdc;
		node->in = shell->hdc->in;
		shell->hdc = shell->hdc->next;
		free(tmp);
		*i += 2;
		return (1);
	}
	return (0);
}

int	check_single_input(char **a[2], int *i, t_cmd *node)
{
	if (a[0][*i][0] == '<')
	{
		node = get_inf1(node, a[1], i);
		return (1);
	}
	return (0);
}
