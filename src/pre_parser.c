/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_parser.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehammoud <ehammoud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 17:43:35 by ehammoud          #+#    #+#             */
/*   Updated: 2024/03/02 20:38:24 by ehammoud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_token	metacharacter_type(char *s)
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
	return (Illegal);
}

t_token	token_type(char *s)
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

t_bool	grab_assign(t_queue **q, char **s)
{
	int		wlen;
	t_bool	found_eq;

	if (!s || !*s || !**s || found_in(**s, DIGIT))
		return (True);
	found_eq = False;
	wlen = 0;
	while ((*s)[wlen] && (*s)[wlen] != SPACE && (*s)[wlen] != TAB
		&& !is_meta_char(*s + wlen))
	{
		if (!found_eq && !found_in((*s)[wlen], UPPERCASE)
			&& !found_in((*s)[wlen], LOWERCASE)
			&& !found_in((*s)[wlen], DIGIT) && (*s)[wlen] != UNDERSCORE)
			break ;
		wlen++;
		if (!found_eq && (*s)[wlen] == EQUAL)
			found_eq = True;
	}
	if (!found_eq)
		return (True);
	if (!add_str_to_queue(q, ft_substr(*s, 0, wlen)))
		return (False);
	*s += wlen;
	return (True);
}

t_bool	parse_assigns(t_queue **q, char **s)
{
	char	*prev_s;

	if (!s || !*s || !**s || !q)
		return (True);
	if (*q && queue_end(*q)->type != Assign && !is_control_op(queue_end(*q)->s))
		return (True);
	prev_s = NULL;
	while (prev_s != *s)
	{
		while (**s == SPACE || **s == TAB)
			(*s)++;
		prev_s = *s;
		if (!grab_assign(q, s))
			return (False);
	}
	return (True);
}

t_bool	parse_word(t_queue **q, char **s, t_bool legal_name_only)
{
	int		wlen;
	t_bool	valid_name;

	wlen = 0;
	valid_name = True;
	if (found_in((*s)[wlen], DIGIT))
		valid_name = False;
	while ((!legal_name_only || valid_name) && (*s)[wlen] && (*s)[wlen] != SPACE
		&& (*s)[wlen] != TAB && (!is_meta_char(*s + wlen)
		|| ((*s)[wlen] == AND && (*s)[wlen + 1] != AND)))
	{
		if (!found_in((*s)[wlen], DIGIT) && !found_in((*s)[wlen], LOWERCASE)
			&& (*s)[wlen] != UNDERSCORE && !found_in((*s)[wlen], UPPERCASE)
			&& valid_name)
		{
			valid_name = False;
			if (legal_name_only)
				break ;
		}
		wlen++;
	}
	if ((*s)[wlen] == DS && ((*s)[wlen + 1] == SPACE || (*s)[wlen + 1] == TAB))
		wlen++;
	if (!wlen && legal_name_only)
		wlen = 1;
	if (!add_str_to_queue(q, ft_substr(*s, 0, wlen)))
		return (False);
	if (wlen && valid_name)
		queue_end(*q)->type = Name;
	else if (wlen)
		queue_end(*q)->type = Word;
	*s += wlen;
	return (True);
}
