/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mshell.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrimah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 11:48:48 by yrimah            #+#    #+#             */
/*   Updated: 2023/05/25 11:49:32 by yrimah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef MSHELL_H
# define MSHELL_H

# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <fcntl.h>
# include "../libft/libft.h"
# include "../minishell.h"
# include <sys/wait.h>
# include <signal.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <dirent.h>
# include <sys/ioctl.h>

typedef struct s_herdoc
{
	int					in;
	struct s_herdoc		*next;
}	t_herdoc;

typedef struct s_quotes
{
	int					quote;
	struct s_quotes		*next;
}	t_quotes;

typedef struct s_cmd
{
	int				s;
	int				in;
	int				out;
	char			*path;
	char			**cmmd;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_env
{
	int				a;
	size_t			index;
	char			*key;
	char			*val;
	struct s_env	*prev;
	struct s_env	*next;
}	t_env;

typedef struct s_pid
{
	pid_t			id;
	struct s_pid	*next;
}	t_pid;

typedef struct s_shell
{
	int				y;
	int				g_status;
	int				ex_st;
	int				flag;
	int				pip[2];
	int				old_out;
	int				old_in;
	int				in_heredoc;
	int				tilde;
	int				quote;
	pid_t			id;
	pid_t			id1;
	char			*heredoc_delim;
	char			**envp;
	char			*str;
	char			*pwd;
	char			*old_pwd;
	t_list			*commands;
	t_env			*env;
	t_cmd			*cmd;
	t_herdoc		*hdc;
	t_quotes		*quotes;
	t_pid			*pid;
}	t_shell;

void	init_shell(t_shell *shell, char **argv, char **envp);
void	rl_replace_line(const char *text, int clear_undo);
// char	**ft_split_buff(char *str, char *charset);
char	**ft_dup_envp(char **envp);
char	**ft_split_space_quoted(char const *str, char *delim);

//error
void	*error_handling(int error, char *param);

//free nodes content if error
void	free_nodes_content(void *content);

// help.c
int		ft_strchars_i(const char *str, char *set);
int		ft_strchr_i(const char *s, int c);
//last split
char	**ft_split_command(char const *s, char *set);

// free triple pointer
void	ft_free_envp(char ***envp);

//envp
int		ft_rows_number(char **envp);
char	**ft_dup_envp(char **envp);

//parse
void	shell_pid(t_shell *sh);
void	set_shlvl(t_shell *sh);
void	set_path(t_shell *sh);
void	set_arg0(t_shell *sh, char *arg);
void	parsing(char **args, t_shell *sh);
void	check(char **str, t_shell *sh);
void	init_vars(t_shell *sh, char *str, char **argv);

//help
char	**ft_mat_rep_in(char ***big, char **small, int n);
char	**ft_ext_mat(char **in, char *newstr);

//final split
char	*ft_last_split(char const *s1, int squote, int dquote);

//get in-out file
t_cmd	*get_outf1(t_cmd *node, char **args, int *i);
t_cmd	*get_outf2(t_cmd *node, char **args, int *i);
t_cmd	*get_inf1(t_cmd *node, char **args, int *i);
t_cmd	*get_inf2(t_cmd *node, char **args, int *i);
int		get_infile2(char **args, int *i);

//set-get env
char	**mini_setenv(char *var, char *value, char **envp, int n);
char	*mini_getenv(char *var, char **envp, int n);

//nodes fill
t_list	*stop_fill(t_list *cmds, char **args, char **temp);
int		check_double_append(char **a[2], int *i, t_cmd *node);
int		check_single_append(char **a[2], int *i, t_cmd *node);
int		check_double_input(char **a[2], int *i, t_cmd *node);
int		check_single_input(char **a[2], int *i, t_cmd *node);
void	help_free(char **args, char ***temp);
int		help_fill1(char **args, char ***temp, t_list **cmds);
void	help_fill2(t_list *cmds[2], int *i, char **args, char ***temp);
t_list	*fill_nodes(char **args, int i);

//heredoc
int		get_here_doc(char *str[2], char *aux[2]);
void	add_herdoc(t_herdoc **lst, int in);
int		open_here_doc(char **args, int a);
void	add_quotes(t_quotes **lqts, int q);
int		check_double_redirect(char **args, int a);
int		is_input_redirect(char **args, int a);
int		check_invalid_redirect(char **args, int a);
int		check_status(void);
void	clear_herdoc(void);

//signal
// void	signal_handle(int signal);

char	*expand_path(char *str, int i, int quotes[2], char *var, t_shell *shell);
char	*expand_vars(char *str, int i, int quotes[2], t_shell *prompt, int q);
char	*mini_getenv(char *var, char **envp, int n);
#endif
