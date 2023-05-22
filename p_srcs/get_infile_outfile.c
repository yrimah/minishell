/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_infile_outfile.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelidrys <aelidrys@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 10:16:01 by aelidrys          #+#    #+#             */
/*   Updated: 2023/05/19 15:30:56 by aelidrys         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	get_fd(int oldfd, char *path, int flags[2])
{
	int	fd;

	if (oldfd > 2)
		close(oldfd);
	if (!path)
		return (-1);
	if (access(path, F_OK) == -1 && !flags[0])
		error_handling(IDIR, path);
	else if (!flags[0] && access(path, R_OK) == -1)
		error_handling(PERM, path);
	else if (flags[0] && access(path, W_OK) == -1 && access(path, F_OK) == 0)
		error_handling(PERM, path);
	if (flags[0] && flags[1])
		fd = open(path, O_CREAT | O_WRONLY | O_APPEND, 0666);
	else if (flags[0] && !flags[1])
		fd = open(path, O_CREAT | O_WRONLY | O_TRUNC, 0666);
	else if (!flags[0] && oldfd != -1)
		fd = open(path, O_RDONLY);
	else
		fd = oldfd;
	return (fd);
}

t_cmd	*get_outf1(t_cmd *node, char **args, int *i)
{
	char	*nl;
	int		flags[2];

	flags[0] = 1;
	flags[1] = 0;
	nl = "minishell: syntax error near unexpected token `newline'";
	(*i)++;
	if (args[*i])
		node->out = get_fd(node->out, args[*i], flags);
	if (!args[*i] || node->out == -1)
	{
		*i = -1;
		if (node->out != -1)
		{
			ft_putendl_fd(nl, 2);
			shell->g_status = 2;
		}
		else
			shell->g_status = 1;
	}
	return (node);
}

t_cmd	*get_outf2(t_cmd *node, char **args, int *i)
{
	int		flags[2];

	flags[0] = 1;
	flags[1] = 1;
	(*i)++;
	if (args[++(*i)])
		node->out = get_fd(node->out, args[*i], flags);
	if (!args[*i] || node->out == -1)
	{
		*i = -1;
		if (node->out != -1)
		{
			ft_putendl_fd("minishell: syntax error near unexpected token `newline'", 2);
			shell->g_status = 2;
		}
		else
			shell->g_status = 1;
	}
	return (node);
}

t_cmd	*get_inf1(t_cmd *node, char **args, int *i)
{
	int		flags[2];

	flags[0] = 0;
	flags[1] = 0;
	(*i)++;
	if (args[*i])
		node->in = get_fd(node->in, args[*i], flags);
	if (!args[*i] || node->in == -1)
	{
		*i = -1;
		if (node->in != -1)
		{
			ft_putendl_fd("minishell: syntax error near unexpected token `newline'", 2);
			shell->g_status = 2;
		}
		else
			shell->g_status = 1;
	}
	return (node);
}

t_cmd	*get_inf2(t_cmd *node, char **args, int *i)
{
	char	*aux[2];
	char	*str[2];

	str[0] = NULL;
	str[1] = NULL;
	aux[0] = NULL;
	aux[1] = "minishell: warning: here-document delimited by end-of-file";
	(*i)++;
	if (args[++(*i)])
	{
		aux[0] = args[*i];
		node->in = get_here_doc(str, aux);
	}
	if (!args[*i] || node->in == -1)
	{
		*i = -1;
		if (node->in != -1)
		{
			ft_putendl_fd("minishell: syntax error near unexpected token `newline'\n", 2);
			shell->g_status = 2;
		}
	}
	return (node);
}

int	get_infile2(char **args, int *i)
{
	int		in;
	int		index;
	char	*aux[2];
	char	*str[2];
	char	*tmp = NULL;

	in = -2;
	str[0] = NULL;
	str[1] = NULL;
	aux[0] = NULL;
	aux[1] = "minishell: warning: here-document delimited by end-of-file";
	(*i)++;
	if (args[++(*i)])
	{
		// printf("%s\n", args[*i])
		aux[0] = args[*i];
		// printf("%s\n", aux[0]);
		int begin = 0;
		int end = 0;
		index = 0;
		// = ft_strlen(aux[0]);
		// printf("%c\n", aux[0][end - 1]);
		// end--;
		// printf("%d -- %s\n", end, aux[0]);
		while ((aux[0][begin] == '\'' || aux[0][begin] == '"'))
		{
			// printf("%cSS\n",aux[0][begin]);
			begin++;
			index = 1;
		}
		// printf("%d\n", begin);
		end = begin;
		while (aux[0][end] && (aux[0][end] != '\'' && aux[0][end] != '"'))
		{
			// printf("%s\n",aux[0]);
			// printf("%c\n",aux[0][end]);
			end++;
		}
		// printf("%c\n",aux[0][end]);
		if (index)
			end--;
		// printf("%c\n", aux[0][end - 1]);
		// if (ft_substr(aux[0], begin, end))
		tmp = ft_substr(aux[0], begin, end);
		aux[0] = ft_strdup(tmp);
		// printf("%s\n", aux[0]);
		in = get_here_doc(str, aux);
		free(tmp);
		free(aux[0]);
		// printf("%s\n", aux[0]);
	}
	if (!args[*i] || in == -1)
	{
		*i = -1;
		if (in != -1)
		{
			ft_putendl_fd("minishell: syntax error near unexpected token `newline'", 2);
			shell->g_status = 258;
		}
	}
	return (in);
}
