#include "../includes/minishell.h"

// extern int	g_status;

void	*error_handling(int error, char *param)
{
	//function handle errors ecountred while the test
	//parsing
	if (error == 1)
		ft_putstr_fd("minishell: unclosed quotes found\n", 2);
	//error in the fd
	else if (error == 2)
		ft_putstr_fd("minishell: No such file or directory: ", 2);
	else if (error == 3)
		ft_putstr_fd("minishell: permission denied: ", 2);
	//error in the pipe
	else if (error == 9)
		ft_putstr_fd("minishell: error creating pipe\n", 2);
	//error while testing the heredoc
	else if (error == 10)
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
	ft_putstr_fd(param, 2);
	return (NULL);
}

void	free_nodes_content(void *content)
{
	t_mishell	*node;

	node = content;
	ft_free_envp(&node->full_command);
	free(node->full_path);
	if (node->in != 0)
		close(node->in);
	if (node->out != 1)
		close(node->out);
	free(node);
}
