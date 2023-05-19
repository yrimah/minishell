/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multible_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelidrys <aelidrys@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/18 07:14:16 by aelidrys          #+#    #+#             */
/*   Updated: 2023/05/18 16:52:28 by aelidrys         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

pid_t	ft_fork(t_shell *shell, int s)
{
	pid_t	id;

	shell->flag = 1;
	id = fork();
	if (id == -1)
	{
		perror("minishell: fork");
		exit(1);
	}
	if (id == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
	}
	if (s && id > 0)
		add_pid(&shell->pid, id);
	return (id);
}

void	first_cmd(t_shell *shell, t_cmd *cmd)
{
	char	*path;
	char	**env;

	if (!cmd || !cmd->cmmd || cmd->s == -1)
		exit(1);
	if (!str_comp(cmd->cmmd[0], "./minishell"))
		check_error(close(shell->pip[0]), close(shell->pip[1]));
	if ((cmd->in >= 0 && dup2(cmd->in, 0) == -1)
		|| (cmd->out >= 0 && dup2(cmd->out, 1) == -1))
		print_error1("minishell: Error in first_dup\n", 1);
	if (builtine(shell, cmd))
		exit(shell->g_status);
	env = convert_env(shell->env);
	path = get_pth(shell->env, env, cmd->cmmd[0], -1);
	if (!path)
	{
		a_printf("minishell: %s%s\n", cmd->cmmd[0], ": command not found", 2);
		exit (127);
	}
	execve(path, cmd->cmmd, env);
	a_printf("minishell: %s%s\n", cmd->cmmd[0], ": command not found", 2);
	exit(127);
}

void	cmd_x(t_shell *shell, t_cmd *cmd)
{
	char	*path;
	char	**env;

	if (!cmd || !cmd->cmmd || cmd->s == -1)
		exit(1);
	if ((cmd->out >= 0 && dup2(cmd->out, 1) == -1))
		print_error1("minishell: Error in dup or close\n", 1);
	if ((cmd->in >= 0 && dup2(cmd->in, 0) == -1))
		print_error1("minishell: Error in dup or close\n", 1);
	if (!str_comp(cmd->cmmd[0], "./minishell"))
		check_error(close(shell->pip[1]), close(shell->pip[0]));
	if (builtine(shell, cmd))
		exit(shell->g_status);
	env = convert_env(shell->env);
	path = get_pth(shell->env, env, cmd->cmmd[0], -1);
	if (!path)
	{
		a_printf("minishell: %s%s\n", cmd->cmmd[0], ": command not found", 2);
		exit(127);
	}
	execve(path, cmd->cmmd, env);
	a_printf("minishell: %s%s\n", cmd->cmmd[0], ": command not found", 2);
	exit(127);
}

void	last_cmd(t_shell *shell, t_cmd *cmd)
{
	char	*path;
	char	**env;

	if (!cmd || !cmd->cmmd || cmd->s == -1)
		exit(1);
	check_error(dup2(shell->old_out, 1), 0);
	if (cmd->out >= 0 && dup2(cmd->out, 1) == -1)
		print_error1("minishell: Error in last_dup\n", 1);
	if (cmd->in >= 0 && dup2(cmd->in, 0) == -1)
		print_error1("minishell: Error in last_dup\n", 1);
	if (builtine(shell, cmd))
		exit(shell->g_status);
	env = convert_env(shell->env);
	path = get_pth(shell->env, env, cmd->cmmd[0], -1);
	if (!path)
	{
		a_printf("minishell: %s%s\n", cmd->cmmd[0], ": command not found", 2);
		exit(127);
	}
	execve(path, cmd->cmmd, env);
	a_printf("minishell: %s%s\n", cmd->cmmd[0], ": command not found", 2);
	exit(127);
}

void	multible_cmd(t_shell *shell, t_cmd *cmd, int a)
{
	a = list_size(cmd, 0);
	check_error(pipe(shell->pip), dup2(shell->pip[1], 1));
	if (!ft_fork(shell, 0))
		first_cmd(shell, cmd);
	cmd = cmd->next;
	check_error(dup2(shell->pip[0], 0), close(shell->pip[1]));
	close(shell->pip[0]);
	while (cmd->next)
	{
		check_error(pipe(shell->pip), dup2(shell->pip[1], 1));
		if (!ft_fork(shell, 0))
			cmd_x(shell, cmd);
		check_error(dup2(shell->pip[0], 0), close(shell->pip[1]));
		close(shell->pip[0]);
		cmd = cmd->next;
	}
	shell->id1 = ft_fork(shell, 0);
	if (shell->id1 == 0)
		last_cmd(shell, cmd);
	check_error(dup2(shell->old_out, 1), dup2(shell->old_in, 0));
	waiting(2);
}
