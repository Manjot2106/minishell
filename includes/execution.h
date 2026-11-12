/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msingh <msingh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 13:19:51 by akaddo            #+#    #+#             */
/*   Updated: 2026/03/29 15:33:28 by msingh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# include "minishell.h"

# define CMD_NOT_FOUND 127
# define CMD_NOT_EXECUTABLE 126

// Executing
void	init_mini(char **envp, t_shell *shell);
char	*get_env_value(char *key, char **envp);
void	update_env(char *key, char *value, t_shell *shell);
int		env_index(char **envp, char *key);
void	update_shlvl(t_shell *shell);
void	print_cmd_not_found(char *cmd);
// build_path.c
char	*resolve_path(char *cmd, char **envp);
// single.c
void	single_cmd(t_command *cmd, t_shell *shell, t_pipeline *p);
// multiple.c
void	multiple_cmd(t_pipeline *p, t_shell *shell);
// redirections.c
int		handle_redirs(t_command *cmd);
int		heredoc_preparation(t_pipeline *p, t_shell *shell);
int		ft_heredoc_signals_enter(void);
int		ft_heredoc_signals_leave(int stdin_backup);
int		ft_heredoc_interrupted(void);
void	ft_warn_heredoc_eof(char *delim, int line_no);
char	*ft_expand_heredoc_line(char *line, t_shell *shell);
// pipeline.c
void	execution(t_pipeline *p, t_shell *shell);
void	child_process(t_command *cmd, t_shell *shell, t_pipeline *p);
void	wait_pipeline(pid_t last_pid, t_shell *shell, int n);
pid_t	fork_pipeline(t_exec_state *state);

// builtins
int		handler_builtin(t_command *cmd, t_shell *shell, t_pipeline *p);
int		cmd_echo(t_command *cmd);
int		cmd_cd(t_command *cmd, t_shell *shell);
int		cmd_pwd(void);
int		cmd_export(t_command *cmd, t_shell *shell);
int		cmd_unset(t_command *cmd, t_shell *shell);
int		cmd_env(t_shell *shell);
int		cmd_exit(t_command *cmd, t_shell *shell, t_pipeline *p);

// signal
int		ft_consume_signal_status(int fallback);
int		ft_get_signal_status(void);
void	ft_set_signal_status(int status);
void	ft_setup_child_signals(void);
void	ft_setup_interactive_signals(void);

// utils
int		export_valid_identifier(char *str);
#endif
