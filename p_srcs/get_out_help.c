/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_out_help.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrimah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/31 16:43:26 by yrimah            #+#    #+#             */
/*   Updated: 2023/05/31 16:43:32 by yrimah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../minishell.h"

void	get_out_help1(int *i, t_cmd *node, char **nl)
{
	*i = -3;
	if (node->out != -1)
	{
		ft_putendl_fd(*nl, 2);
		g_shell->g_status = 258;
	}
	else
		g_shell->g_status = 1;
}

void	get_out_help2(int *i, char **nl)
{
	ft_putendl_fd(*nl, 2);
	*i = -3;
	g_shell->g_status = 258;
}

void	get_out_help3(int *i)
{
	error_handling(11, NULL);
	*i = -3;
	g_shell->g_status = 258;
}

void	get_out_help4(int *i)
{
	g_shell->g_status = 0;
	(*i)++;
}
