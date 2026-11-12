/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msingh <msingh@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 11:45:00 by msingh            #+#    #+#             */
/*   Updated: 2026/03/16 12:23:34 by msingh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "minishell.h"

t_pipeline	*parse_line(char *line, int last_status, char **envp,
				int *parse_status);

int			grow_buf(char **buf, size_t *cap, size_t need);
int			bufpair_init(t_bufpair *bp);
int			bufpair_push(t_bufpair *bp, char c, char q);
char		qchar_from_quote(char quote);
int			handle_quote(char *line, int *i, char *quote, int *qmask);

void		ft_free_tokens(t_token *toks);

int			is_space(char c);
int			is_meta(char c);
int			is_redir(t_token_type t);

t_token		*ft_tokeniser(char *line, t_parse_err *err);
t_token		*token_new(t_token_type type, char *value, int qmask, char *qstate);
void		token_add_back(t_token **toks, t_token *new);

int			ft_read_word(char *line, int *i, t_word *w, t_parse_err *err);
int			ft_push(t_token **toks, char *line, int i, t_parse_err *err);

int			ft_check_syntax(t_token *token);

t_pipeline	*ft_construct_pipeline(t_token *token);

int			count_cmds(t_token *t);
int			argvb_push(t_argvb *b, char *s);
int			argvb_init(t_argvb *b);
void		argvb_free(t_argvb *b);

int			redir_push_from_token(t_command *cmd, t_token *t);
int			ft_expand_tokens(t_token *tokens, int last_status, char **envp);
int			expand_dollar(t_bufpair *bp, const char *s, int *i, char **envp);
int			expand_bounded(t_token *t, t_bufpair *bp, int *i, char **envp);
int			append_status(t_bufpair *bp, int last_status);

#endif
