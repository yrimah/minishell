/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelidrys <aelidrys@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/17 17:44:29 by yrimah            #+#    #+#             */
/*   Updated: 2023/05/25 17:13:07 by yrimah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*mini_getenv(char *var, char **envp, int n)
{
	int	i;
	int	n2;

	i = 0;
	if (n < 0)
		n = ft_strlen(var);
	while (!ft_strchr(var, '=') && envp && envp[i])
	{
		n2 = n;
		if (n2 < ft_strchr_i(envp[i], '='))
			n2 = ft_strchr_i(envp[i], '=');
		if (!ft_strncmp(envp[i], var, n2))
			return (ft_substr(envp[i], n2 + 1, ft_strlen(envp[i])));
		i++;
	}
	return (NULL);
}

static char	*y_get_env(char *var, t_env *env, int n, int *index)
{
	int	n2;

	if (n < 0)
		n = ft_strlen(var);
	while (var && env)
	{
		n2 = n;
		if (n2 < ft_strchars_i(env->key, "\"\'$|>< "))
			n2 = ft_strchars_i(env->key, "\"\'$|>< ");
		if (!ft_strncmp(env->key, var, n2) && !env->key[n2 + 1])
			return (ft_substr(env->val, 0, ft_strlen(env->val)));
		*index = n2;
		env = env->next;
	}
	return (NULL);
}

static char	*get_substr_var(char *str, int i, t_shell *prompt)
{
	char	*aux;
	char	*path;
	char	*var;
	char	*mytmp1;
	char	*mytmp2;
	int		pos;
	int		index;

	var = NULL;
	pos = ft_strchars_i(&str[i], "|\"\'$?>< ") + (ft_strchr("$?", str[i]) != 0);
	if (pos == -1)
		pos = ft_strlen(str) - 1;
	aux = ft_substr(str, 0, i - 1);
	index = 0;
	var = y_get_env(&str[i], shell->env, \
		ft_strchars_i(&str[i], "\"\'-+*.,:=~@#!<>$^&|%/ "), &index);
	mytmp1 = ft_substr(&str[i], index, ft_strlen(&str[i]) - 1);
	mytmp2 = a_strjoin(var, mytmp1, 0, 0);
	free(mytmp1);
	free(var);
	var = mytmp2;
	if (!var && str[i] == '$')
		var = ft_itoa(prompt->id);
	else if (!var && str[i] == '?')
		var = ft_itoa(shell->g_status);
	path = a_strjoin(aux, var, 0, 0);
	free(aux);
	aux = ft_strdup(path);
	free(var);
	free(path);
	free(str);
	return (aux);
}

// static char *get_var_value(char *str, int i, t_shell *prompt)
// {
//     char *var;
//     int index;
//
//     var = NULL;
//     index = 0;
//     var = y_get_env(&str[i], shell->env, \
//         ft_strchars_i(&str[i], "\"\'-+*.,=~@#!$^&|%/ "), &index);
//     return var;
// }
// static char *handle_var_value(char *var, char *aux, char *str, char *path)
// {
//     char *mytmp1;
//     char *mytmp2;
    
//     mytmp1 = ft_substr(&str[i], index, ft_strlen(&str[i]) - 1);
//     mytmp2 = a_strjoin(var, mytmp1, 0, 0);
//     free(mytmp1);
//     free(var);
//     var = mytmp2;
//     if (!var && str[i] == '$')
//         var = ft_itoa(prompt->id);
//     else if (!var && str[i] == '?')
//         var = ft_itoa(shell->g_status);
//     path = a_strjoin(aux, var, 0, 0);
//     free(aux);
//     aux = ft_strdup(path);
//     free(var);
//     free(path);
//     free(str);
//     return aux;
// }
// static char *get_substr_var(char *str, int i, t_shell *prompt)
// {
//     char *aux;
//     char *path;
//     char *var;
//     int pos;
//     int index;
    
//     var = NULL;
//     pos = ft_strchars_i(&str[i], "|\"\'$?>< ") + (ft_strchr("$?", str[i]) != 0);
//     if (pos == -1)
//         pos = ft_strlen(str) - 1;
//     aux = ft_substr(str, 0, i - 1);
//     index = 0;
//     var = get_var_value(str, i, prompt);
//     return handle_var_value(var, aux, str, path);
// }

char	*expand_vars(char *str, int i, int quotes[2], t_shell *prompt, int q)
{
	quotes[0] = 0;
	quotes[1] = 0;
	while (str && str[++i])
	{
		quotes[0] += (!quotes[1] && str[i] == '\'');
		quotes[1] += (!quotes[0] && str[i] == '\"');
		if (q)
			quotes[0] = 0;
		if (str[i] && quotes[0] % 2 == 0 && str[i] == '$' && str[i + 1])
		{
			if (!quotes[1] && ft_strchars_i("~%^{}:;<> ", &str[i + 1]))
				return (expand_vars(get_substr_var(str, ++i, prompt),
						-1, quotes, prompt, q));
			else if (quotes[1] && ft_strchars_i("~%^{}:;<>\"", &str[i + 1]))
				return (expand_vars(get_substr_var(str, ++i, prompt),
						-1, quotes, prompt, q));
		}
	}
	return (str);
}

char	*expand_path(char *str, int i, int quotes[2], char *var, t_shell *shell)
{
	char	*path;
	char	*aux;

	quotes[0] = 0;
	quotes[1] = 0;
	while (str && str[++i])
	{
		quotes[0] = (quotes[0] + (!quotes[1] && str[i] == '\'')) % 2;
		quotes[1] = (quotes[1] + (!quotes[0] && str[i] == '\"')) % 2;
		if (!quotes[0] && !quotes[1] && str[i] == '~'
			&& (i == 0 || str[i - 1] == ' ' || str[i - 1] == '$'))
		{
			aux = ft_substr(str, 0, i);
			path = ft_strjoin(aux, var);
			free(aux);
			aux = ft_substr(str, i + 1, ft_strlen(str));
			free(str);
			str = ft_strjoin(path, aux);
			free(aux);
			free(path);
			return (expand_path(str, i + ft_strlen(var) - 1,
					quotes, var, shell));
		}
	}
	free(var);
	return (str);
}
