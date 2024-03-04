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

t_bool	is_legal(char *str, t_queue *tmp)
{
	if (!tmp && is_control_op(str))
		return (False);
	if (!tmp)
		return (True);
	if (is_control_op(str) && str[0] != LP && (tmp->type == Op_logic
		|| tmp->type == Op_pipe || tmp->type == Op_redir || tmp->type == Bracket_open))
		return (False);
	if ((str[0] == INF || str[0] == OUF) && tmp->type == Op_redir)
		return (False);
	return (True);
}