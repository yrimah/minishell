NAME		:= minishell

CC			:= cc
FLAGS		:= -Wall -Wextra -Werror
RD_FLAGS	:= -lreadline

EXE			:= ./minishell
DIR_SRCS	:=	srcs
DIR_INCS	:=	includes

LST_SRCS	:=	main.c split_space_quoted.c handling.c \
				ft_dup_envp.c fill_nodes.c ft_last_split.c \
				ft_split_command.c get_infile_outfile.c \
				help.c parsing.c heredoc_handle.c expander.c
LST_INCS	:= 	minishell.h

OBJS	:=	$(LST_SRCS:.c=.o)

SRCS		:=	$(addprefix $(DIR_SRCS)/, $(LST_SRCS))
INCS		:=	$(addprefix $(DIR_INCS)/, $(LST_INCS))

%.o: $(DIR_SRCS)/%.c $(INCS)
	$(CC) -I $(DIR_INCS) -c $< -o $@

all:		$(NAME)

$(NAME):	$(OBJS)
	make -C libft/
	$(CC) $(OBJS) -L libft/ -lft -L /goinfre/yrimah/homebrew/opt/readline/lib -I /goinfre/yrimah/homebrew/opt/readline/include  $(RD_FLAGS) $(FLAGS) -o $@ && $(EXE)

# exec:
# 	./minishell

clean:
	rm -rf $(OBJS)
	make clean -C libft/

fclean:		clean
	rm -rf $(NAME)

re:			fclean all

.PHONY:		all clean fclean re