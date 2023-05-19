/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelidrys <aelidrys@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 10:49:41 by aelidrys          #+#    #+#             */
/*   Updated: 2023/05/13 09:32:06 by aelidrys         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	delete_env(t_shell *shell, char *key, int s)
{
	t_env	*env;

	if (!s && !check_key(shell, key, "unset", 0))
		return ;
	env = a_get_env(shell->env, key);
	if (env)
	{
		if (env->prev)
			env->prev->next = env->next;
		if (!env->prev)
			shell->env = env->next;
		if (env->next)
			env->next->prev = env->prev;
		free(env->key);
		free(env->val);
		free(env);
	}
}

void	a_unset(t_shell *shell, t_cmd *cmd)
{
	size_t	x;

	x = 1;
	if (!cmd && !cmd->cmmd)
		return ;
	shell->g_status = 0;
	while (cmd->cmmd[0] && cmd->cmmd[x])
		delete_env(shell, cmd->cmmd[x++], 0);
}
