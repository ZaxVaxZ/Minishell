/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_checkers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 17:05:39 by marvin            #+#    #+#             */
/*   Updated: 2024/03/15 13:05:15 by pipolint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

/* -----------------------
 * Functions in the file:
 *   is_meta_char()
 *   is_valid_var_char()
 *   is_allowed_in_word()
 *   is_legal_queue_end()
 *   is_legal_control_op()
 * -----------------------*/

/// @brief Checks if the string starts with a meta-character
/// @param str The string to check
/// @param control_op True to check for control operators specifically
/// @return True if the first char is a meta-character, False otherwise
t_bool	is_meta_char(char *str, t_bool control_op)
{
	if (str[0] == PIPE || str[0] == AND || str[0] == SC || str[0] == LP
		|| str[0] == RP || str[0] == NL)
		return (True);
	return (!control_op && (str[0] == INF || str[0] == OUF
			|| str[0] == DS));
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
	if (!*s || *s == SPACE || *s == TAB || *s == DQ || *s == SQ)
		return (False);
	if (is_meta_char(s, False))
	{
		if (*s == AND && *(s + 1) != AND)
			return (True);
		else
			return (False);
	}
	return (True);
}

/// @brief Asserts the legality of the last operator if parsing is done
/// @param q The parsing queue
/// @param s The string left unparsed
/// @return True if the last operator node is legal, False otherwise
t_bool	is_legal_queue_end(t_queue *q, char *s)
{
	t_queue	*q_end;

	if (!q || !s || (*s && *s != NL))
		return (True);
	q_end = NULL;
	while (q)
	{
		if (q->type != Whitespace)
			q_end = q;
		q = q->next;
	}
	if (q_end->type != Op_logic && q_end->type != Op_redir && q_end->type != Op_pipe)
		return (True);
	while (q_end->next)
		delete_next(&q_end);
	q_end->type = Illegal;
	free(q_end->s);
	q_end->s = NULL;
	return (False);
}

/// @brief Helps with catching illegal parsing with misplaced control ops
/// @param s The remaining unparsed string
/// @param q The parse queue
/// @return True if it's valid syntax for the control op, False otherwise
t_bool	is_legal_control_op(char *s, t_queue *q)
{
	int		opened;
	t_queue	*q_end;

	opened = 0;
	q_end = NULL;
	while (q)
	{
		opened += (q->type == Bracket_open);
		opened -= (q->type == Bracket_closed);
		if (q->type != Whitespace)
			q_end = q;
		q = q->next;
	}
	if ((!q_end && is_meta_char(s, True) && s[0] != LP) || (s[0] == RP && !opened))
		return (False);
	if (is_meta_char(s, True) && s[0] != LP && (q_end->type == Op_logic
			|| q_end->type == Op_pipe || q_end->type == Op_redir
			|| q_end->type == Bracket_open || q_end->type == Semicolon))
		return (False);
	if ((s[0] == INF || s[0] == OUF) && q_end && q_end->type == Op_redir)
		return (False);
	return (True);
}
