/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_infile_outfile.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelidrys <aelidrys@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 10:16:01 by aelidrys          #+#    #+#             */
/*   Updated: 2023/05/25 17:20:31 by yrimah           ###   ########.fr       */
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
		error_handling(2, path);
	else if (!flags[0] && access(path, R_OK) == -1)
		error_handling(3, path);
	else if (flags[0] && access(path, W_OK) == -1 && access(path, F_OK) == 0)
		error_handling(3, path);
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
	char	*error;

	flags[0] = 1;
	flags[1] = 1;
	error = "minishell: syntax error near unexpected token `newline'";
	(*i)++;
	if (args[++(*i)])
		node->out = get_fd(node->out, args[*i], flags);
	if (!args[*i] || node->out == -1)
	{
		*i = -1;
		if (node->out != -1)
		{
			ft_putendl_fd(error, 2);
			shell->g_status = 258;
		}
		else
			shell->g_status = 258;
	}
	return (node);
}

t_cmd	*get_inf1(t_cmd *node, char **args, int *i)
{
	int		flags[2];
	char	*error;

	flags[0] = 0;
	flags[1] = 0;
	error = "minishell: syntax error near unexpected token `newline'";
	(*i)++;
	if (args[*i])
		node->in = get_fd(node->in, args[*i], flags);
	if (!args[*i] || node->in == -1)
	{
		*i = -1;
		if (node->in != -1)
		{
			ft_putendl_fd(error, 2);
			shell->g_status = 258;
		}
		else
			shell->g_status = 258;
	}
	return (node);
}

void	add_quotes(t_quotes **lqts, int q)
{
	t_quotes	*quote_new;
	t_quotes	*quote_tmp;

	if (lqts)
	{
		quote_new = malloc(sizeof(t_quotes));
		quote_new->quote = q;
		quote_new->next = NULL;
		if (*lqts)
		{
			quote_tmp = *lqts;
			while (quote_tmp->next)
				quote_tmp = quote_tmp->next;
			quote_tmp->next = quote_new;
		}
		else
			*lqts = quote_new;
	}
}

int	get_infile2(char **args, int *i)
{
	int		in;
	int		index;
	char	*aux[2];
	char	*str[2];
	char	*tmp;
	int		begin;
	int		end;
	int		counter;

	in = -2;
	str[0] = NULL;
	str[1] = NULL;
	aux[0] = NULL;
	tmp = NULL;
	begin = 0;
	end = 0;
	aux[1] = "minishell: warning: here-document delimited by end-of-file";
	(*i)++;
	if (args[++(*i)])
	{
		aux[0] = args[*i];
		index = 0;
		counter = 0;
		if (aux[0][counter] == '\'' || aux[0][counter] == '"')
			add_quotes(&(shell->quotes), 1);
		else
			add_quotes(&(shell->quotes), 0);
		if ((aux[0][counter] == '\'' && aux[0][counter + 1] != '"')
			|| (aux[0][counter] == '"' && aux[0][counter + 1] != '\''))
		{
			while (aux[0][counter] == '\'' || aux[0][counter] == '"')
			{
				if (aux[0][begin] == '"')
				{
					while (aux[0][begin] == '\"')
						begin++;
					if (begin == 1)
						end = ft_strlen(aux[0]) - begin;
					else
						end = ft_strlen(aux[0]) - (begin * 2) + 1;
				}
				if (aux[0][begin] == '\'')
				{
					while (aux[0][begin] == '\'')
						begin++;
					if (begin == 1)
						end = ft_strlen(aux[0]) - begin;
					else
						end = ft_strlen(aux[0]) - (begin * 2) + 1;
				}
				counter++;
			}
		}
		else
		{
			if (aux[0][begin] == '\'' || aux[0][begin] == '"')
			{
				begin++;
				end = ft_strlen(aux[0]) - begin;
			}
		}
	}
	tmp = ft_substr(aux[0], begin, end - 1);
	aux[0] = ft_strdup(tmp);
	free(tmp);
	in = get_here_doc(str, aux);
	free(aux[0]);
	if (!args[*i] || in == -1)
	{
		*i = -1;
		if (in != -1)
		{
			ft_putendl_fd("minishell: syntax\
					error near unexpected token `newline'", 2);
			shell->g_status = 258;
		}
	}
	return (in);
}
