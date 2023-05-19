/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_handle.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelidrys <aelidrys@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 10:16:21 by aelidrys          #+#    #+#             */
/*   Updated: 2023/05/19 18:24:14 by aelidrys         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_here_str(char *str[2], size_t len, char *limit, char *warn)
{
	char	*temp;

	(void)warn;
	while (shell->g_status != 130 && (!str[0] || ft_strncmp(str[0], limit, len) \
		|| ft_strlen(limit) != len))
	{
		temp = str[1];
		str[1] = a_strjoin(str[1], str[0], 0, 0);
		free(temp);
		free(str[0]);
		str[0] = readline("> ");
		if (!str[0])
			break ;
		temp = str[0];
		str[0] = ft_strjoin(str[0], "\n");
		free(temp);
		len = ft_strlen(str[0]) - 1;
	}
	free(str[0]);
	return (str[1]);
}

void	add_herdoc(t_herdoc **lst, int in)
{
	t_herdoc	*new;
	t_herdoc	*tmp;

	if (lst)
	{
		new = malloc(sizeof(t_herdoc));
		new->in = in;
		new->next = NULL;
		if (*lst)
		{
			tmp = *lst;
			while (tmp->next)
				tmp = tmp->next;
			tmp->next = new;
		}
		else
			*lst = new;
	}
}

int	open_here_doc(char **args, int a)
{
	t_herdoc *hdc;

	a = -1;
	while (args[++a])
	{
		// printf("args[%d] = %s\n", a, args[a]);
		if (str_comp(args[a], "<") && str_comp(args[a + 1], "<"))
		{
			add_herdoc(&(shell->hdc), get_infile2(args, &a));
			if (shell->g_status == 1)
			{
				while (shell->hdc)
				{
					hdc = shell->hdc;
					shell->hdc = shell->hdc->next;
					free(hdc);
				}
				return (0);
			}
		}
	}
	return (1);
}

int	get_here_doc(char *str[2], char *aux[2])
{
	int		fd[2];

	shell->g_status = 0;
	if (pipe(fd) == -1)
	{
		error_handling(9, NULL);
		return (-1);
	}
	if (!ft_fork(shell, 1))
	{
		signal(SIGQUIT, SIG_IGN);
		str[1] = get_here_str(str, 0, aux[0], aux[1]);
		write(fd[1], str[1], ft_strlen(str[1]));
		if (shell->g_status == 130)
		{
			close(fd[0]);
			return (-1);
		}
		exit (0);
	}
	free(str[1]);
	close(fd[1]);
	wait(&shell->ex_st);
	if (WIFSIGNALED(shell->ex_st))
		shell->g_status = 1;
	return (fd[0]);
}
