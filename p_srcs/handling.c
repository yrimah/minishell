/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handling.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelidrys <aelidrys@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 10:16:10 by aelidrys          #+#    #+#             */
/*   Updated: 2023/05/31 19:21:09 by yrimah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	error_help(int error)
{
	error_handling(error, NULL);
	g_shell->g_status = 258;
}

void	*error_handling(int error, char *param)
{
	if (error == 1)
		ft_putstr_fd("minishell: unclosed quotes found\n", 2);
	else if (error == 2)
		a_printf("minishell: %s%s\n", param, ": No such file or directory", 2);
	else if (error == 3)
		a_printf("minishell: %s%s\n", param, ": permission denied", 2);
	else if (error == 9)
		ft_putstr_fd("minishell: error creating pipe\n", 2);
	else if (error == 10)
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
	else if (error == 11)
		ft_putstr_fd("minishell: syntax error near unexpected token `<'\n", 2);
	else if (error == 12)
		ft_putstr_fd("minishell: syntax error near unexpected token `>'\n", 2);
	else if (error == 13)
		ft_putstr_fd("minishell: syntax error near unexpected token `<<'\n", 2);
	else if (error == 14)
		ft_putstr_fd("minishell: syntax error near unexpected token `>>'\n", 2);
	if (error != 2 && error != 3)
		ft_putstr_fd(param, 2);
	return (NULL);
}

t_cmd	*get_in_error(t_cmd *node, int *i, int n_error, int flag)
{
	if (!flag)
		error_handling(n_error, NULL);
	*i = -3;
	g_shell->g_status = 258;
	if (flag)
	{
		ft_putendl_fd("minishell: syntax error near\
		unexpected token `newline'", 2);
		node->in = -1;
	}
	return (node);
}

void	free_nodes_content(void *content)
{
	t_cmd	*node;

	node = content;
	ft_free_envp(&node->cmmd);
	free(node->path);
	if (node->in != 0)
		close(node->in);
	if (node->out != 1)
		close(node->out);
	free(node);
}
