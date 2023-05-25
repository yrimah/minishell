/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelidrys <aelidrys@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/11 15:49:32 by aelidrys          #+#    #+#             */
/*   Updated: 2023/05/19 18:05:25 by aelidrys         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//  x = 1 one command //
//  x = 2 multible cmd //
void	waiting(int x)
{
	if (x == 2)
	{
		waitpid(shell->id1 ,&shell->ex_st, 0);
		while (wait(NULL) != -1)
			;
	}
	while (x == 1 && wait(&shell->ex_st) != -1)
		;
	if (WIFEXITED(shell->ex_st))
		shell->g_status = WEXITSTATUS(shell->ex_st);
	if (x && WIFSIGNALED(shell->ex_st))
	{
		if (WTERMSIG(shell->ex_st) == 2)
			shell->g_status = 130;
		if (WTERMSIG(shell->ex_st) == 3)
		{
			shell->g_status = 131;
			write(1, "Quit: 3\n", 8);
		}
	}
	shell->flag = 0;
}


void	free_cmd(t_cmd *cmd)
{
	t_cmd	*cmd1;
	int		i;

	i = 0;
	while (cmd)
	{
		cmd1 = cmd;
		ft_free(cmd->cmmd);
		cmd = cmd->next;
		free(cmd1);
	}
}

void	add_cmd(t_cmd **list, t_cmd *new_cmd)
{
	t_cmd	*tmp;

	if (list && new_cmd)
	{
		if (*list)
		{
			tmp = *list;
			while (tmp->next)
				tmp = tmp->next;
			tmp->next = new_cmd;
		}
		else
			*list = new_cmd;
	}
}

void	cmd_list(t_shell *shell)
{
	t_list	*ptr;
	t_list	*ptr1;

	ptr = shell->commands;
	while (ptr)
	{
		add_cmd(&shell->cmd, (t_cmd *)ptr->content);
		ptr1 = ptr;
		ptr = ptr->next;
		free(ptr1);
	}
	shell->commands = NULL;
}

void	one_cmd(t_shell *shell, t_cmd *cmd)
{
	char	*path;
	char	**env;

	if (!cmd || !cmd->cmmd || builtine(shell, cmd) || cmd->s == -1)
		return ;
	if (!ft_fork(shell, 0))
	{
		env = convert_env(shell->env);
		path = get_pth(shell->env, env, cmd->cmmd[0], -1);
		if (!path)
		{
			a_printf("minishell: %s%s\n", cmd->cmmd[0],
				": command not found", 2);
			exit(127);
		}
		if (cmd->in >= 0 && dup2(cmd->in, 0) == -1)
			print_error1("minishell: Error in dup1\n", 1);
		if (cmd->out >= 0 && dup2(cmd->out, 1) == -1)
			print_error1("minishell: Error in dup2\n", 1);
		execve(path, cmd->cmmd, env);
		a_printf("minishell: %s%s\n", cmd->cmmd[0], ": command not found", 2);
		exit(127);
	}
	waiting(1);
}
