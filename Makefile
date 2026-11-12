NAME	= minishell

SRC_DIR	= src/
OBJ_DIR	= obj/
INC_DIR		= includes/

SRC		= main.c \
			parsing/ft_check_syntax.c parsing/parsing.c parsing/expand.c parsing/expand_utils.c parsing/expand_bound.c\
			token/buf_quote.c token/read_token.c token/token.c \
			ft_contruct/argv.c ft_contruct/ft_contruct_pipeline.c ft_contruct/redir.c \
			utils/ft_error_free.c utils/ft_free_minishell.c utils/utils.c \
			execution/execute.c execution/prepare_cmd.c execution/build_path.c \
			execution/redirections.c execution/heredoc.c execution/heredoc_signal.c \
			execution/heredoc_utils.c execution/pipeline.c execution/env.c \
			execution/env_utils.c \
			builtins/builtins.c builtins/cd_builtin.c builtins/echo_builtin.c \
			builtins/env_builtin.c builtins/exit_builtin.c builtins/export_builtin.c \
			builtins/export_print.c \
			builtins/pwd_builtin.c builtins/unset_builtin.c \
			libft/ft_split.c libft/libft-utils.c libft/libft-utils2.c libft/libft-utils3.c \
			libft/libft-utils4.c libft/libft-utils5.c signals.c

SRCS	= $(addprefix ${SRC_DIR}, ${SRC})
OBJS	= $(SRCS:$(SRC_DIR)%.c=$(OBJ_DIR)%.o)

CC		= cc
CFLAGS	= -Wall -Wextra -Werror -g
INCLUDES = -I $(INC_DIR)
READLINE = -lreadline


all: $(NAME)

$(NAME): $(OBJS)
		$(CC) $(CFLAGS) $(OBJS) $(READLINE) -o $(NAME)

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@rm -rf $(OBJ_DIR)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all re clean fclean

