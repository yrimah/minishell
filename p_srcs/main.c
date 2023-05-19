/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelidrys <aelidrys@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/17 20:01:41 by yrimah            #+#    #+#             */
/*   Updated: 2023/05/11 08:19:54 by aelidrys         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	shell_pid(t_shell *sh)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		error_handling(8, NULL);
		ft_free_envp(&sh->envp);
		exit(1);
	}
	if (!pid)
	{
		ft_free_envp(&sh->envp);
		exit(1);
	}
	waitpid(pid, NULL, 0);
	sh->id = pid - 1;
}

void	set_shlvl(t_shell *sh)
{
	char	*str;
	char	*num;

	str = mini_getenv("SHLVL", sh->envp, 5);
	if (!str || ft_atoi(str) <= 0)
		num = ft_strdup("1");
	else
		num = ft_itoa(ft_atoi(str) + 1);
	free(str);
	sh->envp = mini_setenv("SHLVL", num, sh->envp, 5);
	free(num);
}

void	set_path(t_shell *sh)
{
	char	*str;

	str = mini_getenv("PATH", sh->envp, 4);
	if (!str)
		sh->envp = mini_setenv("PATH", \
			"/usr/local/sbin:/usr/local/bin:/usr/bin:/bin", sh->envp, 4);
	free(str);
}

void	set_arg0(t_shell *sh, char *arg)
{
	char	*str;

	str = mini_getenv("_", sh->envp, 1);
	if (!str)
		sh->envp = mini_setenv("_", arg, sh->envp, 1);
	free(str);
}

void	init_vars(t_shell *sh, char *str, char **argv)
{
	(void)str;
	set_shlvl(sh);
	set_path(sh);
	set_arg0(sh, argv[0]);
}

void	init_shell(t_shell *sh, char **argv, char **envp)
{
	char	*str;

	sh->commands = NULL;
	// if (!argv && !envp)
	// 	return (sh);
	str = NULL;
	// sh.in_heredoc = 0;
	// sh.heredoc_delim = NULL;
	sh->envp = ft_dup_envp(envp);
	shell_pid(sh);
	init_vars(sh, str, argv);
}

void	check(char **str, t_shell *sh)
{
	parsing(str, sh);
	// if (sh && sh->commands){
		// printf("enter\n");
	// 	ft_lstclear(&sh->commands, free_nodes_content);
	// }
}

// int	main(int argc, char **argv, char **envp)
// {
// 	char	**splited;
// 	char	*buff;
// 	t_shell	*sh;

// 	buff = NULL;
// 	sh = malloc(sizeof(t_shell));
// 	init_shell(sh, argv, envp);
// 	while (argv && argc)
// 	{
// 		buff = readline("42-minishell~>");
// 		if (ft_strlen(buff) > 0)
// 			add_history(buff);
// 		if (!buff)
// 			break ;
// 		splited = ft_split_space_quoted(buff, " ");
// 		free(buff);
// 		if (!splited)
// 			error_handling(1, NULL);
// 		if (splited)
// 			check(splited, sh);
// 	}
// 	return (EXIT_SUCCESS);
// }
