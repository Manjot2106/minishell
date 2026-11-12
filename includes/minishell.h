/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msingh <msingh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/22 11:06:29 by akaddo            #+#    #+#             */
/*   Updated: 2026/03/29 12:36:56 by msingh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <dirent.h>
# include <errno.h>
# include <stdio.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdlib.h>
# include <string.h>
# include <sys/ioctl.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <termios.h>
# include <unistd.h>
# define Q_SQUOTE 1
# define Q_DQUOTE 2
# define PATH_MAX 4096

typedef enum e_parse_err
{
	P_OK,
	P_ERR_MALLOC,
	P_ERR_QUOTE
}					t_parse_err;

typedef enum e_token_type
{
	WORD,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	HEREDOC
}					t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	int				qmask;
	char			*qstate;
	struct s_token	*next;
}					t_token;

typedef struct s_word
{
	char			*str;
	char			*qstate;
	int				qmask;
}					t_word;

typedef struct s_redir
{
	t_token_type	type;
	char			*target;
	int				quoted;
	int				fd;
	struct s_redir	*next;
}					t_redir;

typedef struct s_command
{
	char			**argv;
	t_redir			*redirs;
}					t_command;

typedef struct s_pipeline
{
	t_command		*commands;
	int				nb_commands;
}					t_pipeline;

typedef struct s_bufpair
{
	char			*out;
	char			*st;
	size_t			cap_out;
	size_t			cap_st;
	size_t			len;
}					t_bufpair;

typedef struct s_argvb
{
	char			**v;
	int				cap;
	int				len;
}					t_argvb;

typedef struct s_shell
{
	char			**envp;
	int				last_exit_status;
}					t_shell;

typedef struct s_exec_state
{
	t_pipeline		*p;
	t_shell			*shell;
	int				prev_fd;
	int				fd[2];
}					t_exec_state;

extern int	g_signal_status;

void				ft_free(t_pipeline *pipeline);
void				free_tab(char **argv);
void				free_redirs(t_redir *r);
int					error_close(char *msg, int fd);
void				free_all_before_exit(t_shell *shell, t_pipeline *p,
						int status);

// Utils-libft
size_t				ft_strlen(const char *str);
int					ft_strncmp(const char *s1, const char *s2, size_t n);
char				*ft_strjoin(char const *s1, char const *s2);
char				*ft_strchr(const char *s, int c);
char				*ft_strdup(const char *s);
char				**ft_split(char const *s, char c);
void				ft_putstr_fd(char const *s, int fd);
void				ft_putendl_fd(char const *s, int fd);
void				*ft_memcpy(void *dst, const void *src, size_t n);
void				ft_bzero(void *s, size_t n);
void				*ft_calloc(size_t nmemb, size_t size);
int					ft_strcmp(const char *s1, const char *s2);
char				*ft_itoa(int n);
int					ft_atoi(const char *nptr);
int					ft_isdigit(int i);
int					is_numeric(char *s);
int					ft_isalpha(int c);
int					ft_isalnum(int c);
char				*ft_substr(char const *s, unsigned int start, size_t len);
#endif
