/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehammoud <ehammoud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 17:43:32 by ehammoud          #+#    #+#             */
/*   Updated: 2024/03/02 20:25:57 by ehammoud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# define UNDERSCORE '_'
# define LOWERCASE "abcdefghijklmnopqrstuvwxyz"
# define UPPERCASE "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
# define DIGIT "0123456789"
# define EQUAL '='
# define SPACE ' '
# define PIPE '|'
# define DASH '-'
# define TAB '\t'
# define AND '&'
# define INF '<'
# define OUF '>'
# define NL '\n'
# define SQ '\''
# define DQ '\"'
# define LP '('
# define RP ')'
# define SC ';'
# define DS '$'

# include "general.h"
# include "libft.h"
# include "queues.h"

t_queue	*parse(char *s);
t_bool	parse_op(t_queue **q, char **s, char op, int max_occurs);
t_bool	parse_double_quote(t_queue **q, char **s);
t_bool	parse_single_quote(t_queue **q, char **s);

int		op_occur(char c, char *s);
t_bool	found_in(char c, char *s);
t_bool	add_str_to_queue(t_queue **q, char *str);
t_bool	parse_word(t_queue **q, char **s, t_bool var_name);

t_bool	is_control_op(char *str);
t_bool	is_legal_control_op(char *str, t_queue *tmp);
t_bool	is_meta_char(char *str);
t_bool	is_valid_var_char(char c);
t_bool	is_allowed_in_word(char *s, t_bool valid_name, t_bool var_name);

#endif