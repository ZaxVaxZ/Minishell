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

t_bool	parse_op(t_queue **q, char **s, char op, int max_occurs)
{
	int	occurs;

	occurs = op_occur(op, *s);
	if (occurs == 1 && op == AND)
		return (True);
	if (occurs > max_occurs)
		occurs = max_occurs;
	if (!add_str_to_queue(q, ft_substr(*s, 0, occurs)))
		return (False);
	*s += occurs;
	if (op == DS && occurs > 0 && (**s == SPACE || **s == TAB))
		queue_end(*q)->type = Word;
	else if (op == DS && occurs > 0 && !is_control_op(*s))
		parse_word(q, s, True);
	return (True);
}

int	find_word_end(char **s, t_bool *valid_name, t_bool var_name)
{
	int	wlen;

	wlen = 0;
	*valid_name = True;
	if (found_in((*s)[wlen], DIGIT))
		*valid_name = False;
	while (is_allowed_in_word(*s + wlen, valid_name, var_name))
	{
		*valid_name = is_valid_var_char(*s[wlen]);
		if (!valid_name && var_name)
			break ;
		wlen++;
	}
	return (wlen);
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
