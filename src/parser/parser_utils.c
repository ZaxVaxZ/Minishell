/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehammoud <ehammoud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/02 20:22:48 by ehammoud          #+#    #+#             */
/*   Updated: 2024/03/02 20:25:47 by ehammoud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

/* -----------------------
 * Functions in the file:
 *   found_in()
 *   op_occur()
 *   metacharacter_type()
 *   token_type()
 *   add_str_to_queue()
 * -----------------------*/

/// @brief Search for a character in a string
/// @param c Character to look for
/// @param s String to search in
/// @return True if the character is found, False otherwise
t_bool	found_in(char c, char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (c == s[i++])
			return (True);
	}
	return (False);
}

/// @brief Counts contiguous occurances of a character at the start of string
/// @param c Character to find at the start of string
/// @param s String to count occurances in
/// @return The number of times c appears uninterrupted at the start of s
int	op_occur(char c, char *s)
{
	int	cc;

	cc = 0;
	if (!s || !*s)
		return (-1);
	while (*s == c)
	{
		s++;
		cc++;
	}
	return (cc);
}

/// @brief Label a string token as which type of meta-character
/// @param s The token's string
/// @return The type of meta-char it is. Illegal if it's not a meta-char
static t_token	metacharacter_type(char *s)
{
	if (!ft_strncmp(s, "<", -1) || !ft_strncmp(s, "<<", -1)
		|| !ft_strncmp(s, ">", -1) ||!ft_strncmp(s, ">>", -1))
		return (Op_redir);
	if (!ft_strncmp(s, "&&", -1) || !ft_strncmp(s, "||", -1))
		return (Op_logic);
	if (!ft_strncmp(s, "|", -1))
		return (Op_pipe);
	if (!ft_strncmp(s, "(", -1))
		return (Bracket_open);
	if (!ft_strncmp(s, ")", -1))
		return (Bracket_closed);
	if (!ft_strncmp(s, ";", -1))
		return (Semicolon);
	if (!ft_strncmp(s, "$", -1))
		return (Variable);
	if (!ft_strncmp(s, " ", -1) || !ft_strncmp(s, "\t", -1))
		return (Whitespace);
	return (Illegal);
}

/// @brief Classify the string as what type of token it is
/// @param s The string to classify
/// @return The type of token the string represents, most general is Word.
static t_token	token_type(char *s)
{
	int	i;

	if (!s || found_in(s[0], DIGIT))
		return (Word);
	if (metacharacter_type(s) != Illegal)
		return (metacharacter_type(s));
	i = 0;
	while (s[i])
	{
		if (s[i] != UNDERSCORE && !found_in(s[i], DIGIT)
			&& !found_in(s[i], LOWERCASE) && !found_in(s[i], UPPERCASE))
			return (Word);
		if (s[++i] == '=')
			return (Assign);
	}
	return (Name);
}

/// @brief Take a string, label it and add it as a token to queue
/// @param q The parse queue
/// @param str The string to add to the queue
/// @return False if any malloc fails, True otherwise
t_bool	add_str_to_queue(t_queue **q, char *str)
{
	t_bool	illegal;
	t_queue	*tmp;

	if (!q)
		return (False);
	if (!str)
		return (True);
	tmp = queue_end(*q);
	illegal = !is_legal_control_op(str, tmp);
	tmp = new_node(str);
	if (!tmp)
		free_queue(q);
	else
	{
		tmp->type = token_type(tmp->s);
		if (illegal)
			tmp->type = Illegal;
		queue(q, tmp);
	}
	free(str);
	if (!*q)
		return (False);
	return (True);
}
