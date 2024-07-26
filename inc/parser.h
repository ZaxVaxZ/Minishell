/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 17:43:32 by ehammoud          #+#    #+#             */
/*   Updated: 2024/07/26 10:48:53 by pipolint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "general.h"
# include "libft.h"
# include "queues.h"
# include "env_list.h"

t_queue	*parse(char *s);
t_bool	parse_op(t_queue **q, char **s, char op, int max_occurs);
t_bool	parse_double_quote(t_queue **q, char **s);
t_bool	parse_single_quote(t_queue **q, char **s);

int		op_occur(char c, char *s);
t_bool	found_in(char c, char *s);
t_bool	add_str_to_queue(t_queue **q, char *str);
t_bool	parse_word(t_queue **q, char **s, t_bool var_name);
int		parse_clean_up(t_main *m, t_queue **h);
int		syntax_error(t_queue **q, char *token, t_bool missing, t_bool at_end);
void	delete_next(t_queue **q);
void	parse_brackets(t_queue **q, t_queue **q_end, int *opened);
int		dont_unpack(t_queue *q);
void	check_parentheses(t_queue *q, t_queue **q_end, int *opened, int *word);
void	set_zero(void *arr, size_t size);
void	set_non_whitespace(t_queue **q, t_queue *last, t_bool ill, char *str);
t_token	token_type(char *s);
int		free_queue_and_return(t_queue **q);

t_bool	is_meta_char(char *str, t_bool control_op);
t_bool	is_legal_control_op(char *str, t_queue *q);
t_bool	is_legal_queue_end(t_queue *q, char *s);
t_bool	is_valid_var_char(char c);
t_bool	is_allowed_in_word(char *s, t_bool valid_name, t_bool var_name);

#endif