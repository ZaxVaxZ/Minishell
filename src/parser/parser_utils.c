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
