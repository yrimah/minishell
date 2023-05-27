/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_handle.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelidrys <aelidrys@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 10:16:21 by aelidrys          #+#    #+#             */
/*   Updated: 2023/05/25 18:37:42 by yrimah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_here_str(char *str[2], size_t len, char *limit, char *warn)
{
	char	*temp;
	t_shell	*sh;
	int		quotes[2];

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
		sh = NULL;
		if (!shell->quotes->quote && !str_comp(limit, str[0]))
			str[0] = expand_vars(str[0], -1, quotes, sh, 1);
		temp = str[0];
		str[0] = ft_strjoin(str[0], "\n");
		free(temp);
		len = ft_strlen(str[0]) - 1;
	}
	free(str[0]);
	return (str[1]);
}

int	help_fd(int *fd)
{
	if (pipe(fd) == -1)
	{
		error_handling(9, NULL);
		return (-1);
	}
	return (0);
}

int	get_here_doc(char *str[2], char *aux[2])
{
	t_quotes	*tquotes;
	int			fd[2];

	shell->g_status = 0;
	if (help_fd(fd) == -1)
		return (-1);
	if (!ft_fork(shell, 1))
	{
		signal(SIGQUIT, SIG_IGN);
		str[1] = get_here_str(str, 0, aux[0], aux[1]);
		write(fd[1], str[1], ft_strlen(str[1]));
		exit (0);
	}
	if (shell->quotes)
	{
		tquotes = shell->quotes;
		shell->quotes = shell->quotes->next;
		free(tquotes);
	}
	free(str[1]);
	close(fd[1]);
	wait(&shell->ex_st);
	if (WIFSIGNALED(shell->ex_st))
		shell->g_status = 1;
	return (fd[0]);
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
	while (args[++a])
	{
		if (args[a + 1])
		{
			if (check_double_redirect(args, a))
				return (0);
		}
		if (is_input_redirect(args, a))
		{
			if (check_invalid_redirect(args, a))
				return (0);
			add_herdoc(&(shell->hdc), get_infile2(args, &a));
			if (check_status())
			{
				clear_herdoc();
				return (0);
			}
		}
	}
	return (1);
}
