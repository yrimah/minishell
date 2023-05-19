/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelidrys <aelidrys@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 10:28:50 by aelidrys          #+#    #+#             */
/*   Updated: 2023/05/13 09:34:30 by aelidrys         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	whit_newline(char **s)
{
	size_t	x;
	size_t	y;

	x = 0;
	while (s && s[x])
	{
		y = 0;
		if (s[x][y] == '-' && s[x][y + 1] == 'n')
		{
			while (s[x][++y] == 'n')
				;
			if (s[x][y])
				return (x);
		}
		else
			return (x);
		x++;
	}
	return (x);
}

void	a_echo(t_cmd *cmd, char **s)
{
	size_t	x;
	int		fd;

	fd = cmd->out;
	if (fd == -2)
		fd = 1;
	x = whit_newline(s);
	shell->g_status = 0;
	if (x)
	{
		while (s[x])
		{
			a_printf("%s", s[x], NULL, fd);
			if (s[x++ + 1])
				a_printf("%s", " ", NULL, fd);
		}
		return ;
	}
	while (s[x])
	{
		a_printf("%s", s[x], NULL, fd);
		if (s[x++ + 1])
			a_printf("%s", " ", NULL, fd);
	}
	a_printf("%s", "\n", NULL, fd);
}
