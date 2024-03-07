/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_checkers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 17:05:39 by marvin            #+#    #+#             */
/*   Updated: 2024/03/07 17:05:39 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_bool	is_control_op(char *str)
{
	return (str[0] == PIPE || str[0] == AND || str[0] == SC || str[0] == LP
		|| str[0] == RP || str[0] == NL || str[0] == SC);
}

t_bool	is_meta_char(char *str)
{
	return (is_control_op(str) || str[0] == INF || str[0] == OUF
		|| str[0] == DS);
}

t_bool	is_valid_var_char(char c)
{
	return (found_in(c, DIGIT) || found_in(c, LOWERCASE)
		|| c == UNDERSCORE || found_in(c, UPPERCASE));
}

// (*s)[wlen] && (*s)[wlen] != SPACE && (*s)[wlen] != TAB
// 		&& !is_meta_char(*s + wlen)
t_bool	is_allowed_in_word(char *s, t_bool valid_name, t_bool var_name)
{
	if (var_name && !valid_name)
		return (False);
	if (!*s || *s == SPACE || *s == TAB)
		return (False);
	if (is_meta_char(s))
	{
		if (*s == AND && *(s + 1) != AND)
			return (True);
		else
			return (False);
	}
	return (True);
}

t_bool	is_legal_control_op(char *str, t_queue *tmp)
{
	if (!tmp && is_control_op(str))
		return (False);
	if (!tmp)
		return (True);
	if (is_control_op(str) && str[0] != LP && (tmp->type == Op_logic
			|| tmp->type == Op_pipe || tmp->type == Op_redir
			|| tmp->type == Bracket_open))
		return (False);
	if ((str[0] == INF || str[0] == OUF) && tmp->type == Op_redir)
		return (False);
	return (True);
}
