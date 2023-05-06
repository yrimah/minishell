#include "../includes/minishell.h"

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

t_mishell	*get_outf1(t_mishell *node, char **args, int *i)
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
			g_status = 2;
		}
		else
			g_status = 1;
	}
	return (node);
}

t_mishell	*get_outf2(t_mishell *node, char **args, int *i)
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
			g_status = 2;
		}
		else
			g_status = 1;
	}
	return (node);
}

t_mishell	*get_inf1(t_mishell *node, char **args, int *i)
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
			g_status = 2;
		}
		else
			g_status = 1;
	}
	return (node);
}

t_mishell	*get_inf2(t_mishell *node, char **args, int *i)
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
			ft_putendl_fd("minishell: syntax error near unexpected token `newline'", 2);
			g_status = 2;
		}
	}
	return (node);
}
