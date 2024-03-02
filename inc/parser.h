/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehammoud <ehammoud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 17:43:32 by ehammoud          #+#    #+#             */
/*   Updated: 2024/03/01 20:54:24 by ehammoud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# define UNDERSCORE '_'
# define LOWERCASE "abcdefghijklmnopqrstuvwxyz"
# define UPPERCASE "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
# define DIGIT "0123456789"
# define EQUAL '='
# define PIPE '|'
# define DASH '-'
# define AND '&'
# define INF '<'
# define OUF '>'
# define FP '.'
# define LP '('
# define RP ')'
# define SC ';'
# define DS '$'

# include "general.h"
# include "libft.h"
# include "queues.h"

int		op_occur(char c, char *s);
t_bool	found_in(char c, char *s);
t_bool	is_control_operator(char *str);
t_token	token_type(char *s);
t_bool	found_in(char c, char *s);
int	op_occur(char c, char *s);
t_bool	is_control_operator(char *str);

#endif