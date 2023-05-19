/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelidrys <aelidrys@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/19 11:14:03 by aelidrys          #+#    #+#             */
/*   Updated: 2023/05/18 12:03:45 by aelidrys         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	pars_here(char *line)
{
	char	**spl;

	spl = ft_split_space_quoted(line, " ");
	if (!spl)
		error_handling(1, NULL);
	if (spl)
		check(spl, shell);
}

void	execution(void)
{
		cmd_list(shell);
		if (list_size(shell->cmd, 0) == 1)
			one_cmd(shell, shell->cmd);
		else if (shell->cmd)
			multible_cmd(shell, shell->cmd, 0);
		free_cmd((t_cmd *)shell->cmd);
		shell->cmd = NULL;
}

void	minishell(t_shell *shell)
{
	char	*line;

	line = NULL;
	while (1)
	{
		dup2(2, 1);
		line = readline("minishell$ ");
		dup2(shell->old_out, 1);
		if (!line)
			break ;
		if (a_strlen(line))
			add_history(line);
		pars_here(line);
		execution();
		free(line);
	}
}

void	a_start(t_shell *shell, char **env1, int x)
{
	x = -1;
	shell->old_in = dup(0);
	shell->old_out = dup(1);
	shell->env = NULL;
	shell->cmd = NULL;
	shell->pid = NULL;
	shell->flag = 0;
	shell->ex_st = 0;
	shell->g_status = 0;
	shell->old_pwd = NULL;
	while (env1[++x])
		add_back(&shell->env, new_env(env1[x], 1));
	delete_env(shell, "OLDPWD", 0);
	add_back(&shell->env, new_env("OLDPWD", 1));
	sort_env(shell->env);
	signal(SIGINT, sigint_hand);
	signal(SIGQUIT, SIG_IGN);
}

int	main(int ac, char **av, char **env)
{
	shell = malloc(sizeof(t_shell));
	setup_term();
	init_shell(shell, av, env);
	a_start(shell, env, ac);
	minishell(shell);
	a_printf("%s\n", "exit", NULL, shell->old_out);
	exit(0);
}
