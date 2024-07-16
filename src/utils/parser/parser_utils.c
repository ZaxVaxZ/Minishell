 /* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/02 20:22:48 by ehammoud          #+#    #+#             */
/*   Updated: 2024/03/26 14:35:44 by pipolint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

/* -----------------------
 * Functions in the file:
 *   found_in()
 *   op_occur()
 *   syntax_error()
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

/// @brief Print a message for the syntax error then free the queue
/// @param q The parse queue, passed to be freed
/// @param token The token string that caused the syntax error
/// @param missing If the syntax error is due to a missing quote or bracket
/// @return Returns 1 to signal to the main that a syntax error occurred
int	syntax_error(t_queue **q, char *token, t_bool missing, t_bool at_end)
{
	if (missing)
		write(2, "syntax error due to missing token `", 35);
	else if (!at_end)
		write(2, "syntax error near unexpected token `", 36);
	else
		write(2, "syntax error near unexpected token `", 36);
	if (!at_end)
		write(2, token, ft_strlen(token));
	else
		write(2, "newline", 7);
	write(2, "`\n", 2);
	free_queue(q);
	return (1);
}

/// @brief Classify the string as what type of token it is
/// @param s The string to classify
/// @return The type of token the string represents, most general is Word.
static t_token	token_type(char *s)
{
	if (!s || found_in(s[0], DIGIT))
		return (Word);
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
	while (*s && is_valid_var_char(*s))
	{
		if (*(++s) == '=')
			return (Assign);
	}
	return (Word);
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
	if (!str || !*str)
	{
		if (str)
			free(str);
		return (True);
	}
	illegal = !is_legal_control_op(str, *q);
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
	return (*q != NULL);
}
