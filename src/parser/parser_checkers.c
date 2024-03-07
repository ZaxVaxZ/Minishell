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

/* -----------------------
 * Functions in the file:
 *   is_control_op()
 *   is_meta_char()
 *   is_valid_var_char()
 *   is_allowed_in_word()
 *   is_legal_control_op()
 * -----------------------*/

/// @brief Checks if the string starts with a control operator
/// @param str The string to check
/// @return True if the first char is a control operator, False otherwise
t_bool	is_control_op(char *str)
{
	return (str[0] == PIPE || str[0] == AND || str[0] == SC || str[0] == LP
		|| str[0] == RP || str[0] == NL || str[0] == SC);
}

/// @brief Checks if the string starts with a meta-character
/// @param str The string to check
/// @return True if the first char is a meta-character, False otherwise
t_bool	is_meta_char(char *str)
{
	return (is_control_op(str) || str[0] == INF || str[0] == OUF
		|| str[0] == DS);
}

/// @brief Checks if the character is allowed in a variable name
/// @param c The character to check
/// @return True if the character is valid for var names, False otherwise
t_bool	is_valid_var_char(char c)
{
	return (found_in(c, DIGIT) || found_in(c, LOWERCASE)
		|| c == UNDERSCORE || found_in(c, UPPERCASE));
}

/// @brief Helps with parsing words and variable names in parse_word()
/// @param str The string to check
/// @param valid_name If the word has been valid for var names so far or not
/// @param var_name If the function is parsing a word or a var name or not
/// @return True if the the next character is suitable, False otherwise
t_bool	is_allowed_in_word(char *s, t_bool valid_name, t_bool var_name)
{
	if (var_name && !valid_name)
		return (False);
	if (!(*s) || *s == SPACE || *s == TAB || *s == DQ || *s == SQ)
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

/// @brief Helps with catching illegal parsing with misplaced control ops
/// @param str The remaining unparsed string
/// @param q_end The last node in the queue. NULL if the queue is empty
/// @return True if it's valid syntax for the control op, False otherwise
t_bool	is_legal_control_op(char *str, t_queue *q_end)
{
	if (!q_end && is_control_op(str))
		return (False);
	if (!q_end)
		return (True);
	if (is_control_op(str) && str[0] != LP && (q_end->type == Op_logic
			|| q_end->type == Op_pipe || q_end->type == Op_redir
			|| q_end->type == Bracket_open))
		return (False);
	if ((str[0] == INF || str[0] == OUF) && q_end->type == Op_redir)
		return (False);
	return (True);
}
