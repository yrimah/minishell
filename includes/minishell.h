#ifndef FIRST_H
#define FIRST_H

# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <fcntl.h>
# include "../libft/libft.h"

# include <sys/wait.h>
//signals
# include <signal.h>
//readline
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
//
# include <dirent.h>
# include <sys/ioctl.h>
// in libft //
// typedef struct s_list
// {
//     int             content;
//     struct s_list   *next;
// }   t_list;
int	g_status;

enum	e_errros
{
	NQTS = 1, // FOR QUOTES
	IDIR = 2, // NOT FOUND DIR
	PERM = 3, // PERMISSION
	// NCMD = 6, // NO COMMAND
	// DUPERR = 7, //
	FORKERR = 8,
	PIPERR = 9,
	PIPENDERR = 10,
	// MEM = 11,
	// IS_DIR = 12,
	// NOT_DIR = 13
};


typedef struct s_shell
{
	t_list	*commands;
	char	**envp;
	pid_t	pid;
	int		in_heredoc;
	char	*heredoc_delim;
}		t_shell;

typedef struct s_mishell
{
	char	**full_command;
	char	*full_path;
	int     in;
	int     out;
}		t_mishell;

void	rl_replace_line (const char *text, int clear_undo);
// char	**ft_split_buff(char *str, char *charset);
char	**ft_dup_envp(char **envp);
char	**ft_split_space_quoted(char const *str, char *delim);


//error
void	*error_handling(int error, char *param);

//free nodes content if error
void	free_nodes_content(void *content);

// help.c
int	ft_strchars_i(const char *str, char *set);
int	ft_strchr_i(const char *s, int c);
//last split
char	**ft_split_command(char const *s, char *set);

// free triple pointer
void    ft_free_envp(char ***envp);

//envp
int ft_rows_number(char **envp);
char **ft_dup_envp(char **envp);

//parse
void	parsing(char **args, t_shell *sh);

//help
char	**ft_mat_rep_in(char ***big, char **small, int n);
char	**ft_ext_mat(char **in, char *newstr);


//final split 
char	*ft_last_split(char const *s1, int squote, int dquote);

//get in-out file
t_mishell	*get_outfile1(t_mishell *node, char **args, int *i);
t_mishell	*get_outfile2(t_mishell *node, char **args, int *i);
t_mishell	*get_infile1(t_mishell *node, char **args, int *i);
t_mishell	*get_infile2(t_mishell *node, char **args, int *i);

//set-get env
char	**mini_setenv(char *var, char *value, char **envp, int n);
char	*mini_getenv(char *var, char **envp, int n);

//nodes fill
t_list	*fill_nodes(char **args, int i);

//heredoc
int	get_here_doc(char *str[2], char *aux[2]);

//signal
void	signal_handle(int signal);

char	*expand_path(char *str, int i, int quotes[2], char *var);
char	*expand_vars(char *str, int i, int quotes[2], t_shell *prompt);
char	*mini_getenv(char *var, char **envp, int n);
#endif