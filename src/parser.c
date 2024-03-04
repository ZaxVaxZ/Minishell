/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehammoud <ehammoud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 13:21:55 by ehammoud          #+#    #+#             */
/*   Updated: 2024/03/02 20:12:53 by ehammoud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_bool	add_str_to_queue(t_queue **q, char *str)
{
	t_bool	illegal;
	t_queue	*tmp;

	if (!q)
		return (False);
	if (!str)
		return (True);
	tmp = queue_end(*q);
	illegal = *q && ((is_control_op(str) && str[0] != LP && (tmp->type == Op_logic
		|| tmp->type == Op_pipe || tmp->type == Op_redir || tmp->type == Bracket_open))
		|| ((str[0] == INF || str[0] == OUF) && tmp->type == Op_redir));
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
	return (True);
}

t_bool	parse_command(t_queue **q, char **s)
{
	char	*prev_s;

	if (q && *q && queue_end(*q)->type == Illegal)
		return (True);
	while (**s == SPACE || **s == TAB)
			(*s)++;
	if (!parse_assigns(q, s))
		return (False);
	prev_s = NULL;
	while (prev_s != *s)
	{
		while (**s == SPACE || **s == TAB)
			(*s)++;
		prev_s = *s;
		if (!parse_op(q, s, INF, 2))
			return (False);
		if (!parse_op(q, s, OUF, 2))
			return (False);
		if (!parse_op(q, s, DS, 1))
			return (False);
		if (!grab_word(q, s))
			return (False);
	}
	return (True);
}

t_bool	parse_control(t_queue **q, char **s)
{
	char	*prev_s;

	if (q && *q && queue_end(*q)->type == Illegal)
		return (True);
	prev_s = NULL;
	while (prev_s != *s && **s != NL)
	{
		while (**s == SPACE || **s == TAB)
			(*s)++;
		prev_s = *s;
		if (!parse_op(q, s, LP, 1))
			return (False);
		if (!parse_op(q, s, AND, 2))
			return (False);
		if (!parse_op(q, s, PIPE, 2))
			return (False);
		if (!parse_op(q, s, RP, 1))
			return (False);
		if (!parse_op(q, s, SC, 1))
			return (False);
	}
	return (True);
}

t_queue	*parse(char *s)
{
	char	*prev_s;
	t_queue	*q;

	if (!s)
		return (NULL);
	q = NULL;
	prev_s = NULL;
	while (prev_s != s)
	{
		prev_s = s;
		if (!parse_command(&q, &s))
			return (NULL);
		if (!parse_control(&q, &s))
			return (NULL);
	}
	return (q);
}


// int	add_str_to_queue(t_queue **q, char *str)
// {
// 	t_queue	*tmp;

// 	if (!str)
// 		return (0);
// 	tmp = queue_end(q);
// 	if (is_control_op(str) && str[0] != LP && (tmp->type == Op_logic
// 		|| tmp->type == Op_pipe || tmp->type == Bracket_open || tmp->type == Op_redir))
// 		return (0);
// 	if ((str[0] == INF || str[0] == OUF) && tmp->type == Op_redir)
// 		return (0);
// 	tmp = new_node(str);
// 	if (!tmp)
// 		free_queue(q);
// 	else
// 	{
// 		tmp->type = token_type(tmp->s);
// 		queue(q, tmp);
// 	}
// 	free(str);
// 	if (!(*q))
// 		return (-1);
// 	return (ft_strlen(str));
// }

// t_bool	extract_op(t_queue **q, char **s, char op, int max_occurs)
// {
// 	int		occurs;

// 	occurs = op_occur(op, s);
// 	if (occurs > max_occurs)
// 		occurs = max_occurs;
// 	if (occurs == 1 && op == '&')
// 		return (True);
// 	occurs = add_str_to_queue(q, ft_substr(*s, 0, occurs));
// 	if (occurs == -1)
// 		return (False);
// 	*s += occurs;
// 	return (True);
// }

// t_bool	parse_metacharacters(t_queue **q, char **str)
// {
// 	if (!extract_op(q, str, LP, 1))
// 		return (False);
// 	if (!extract_op(q, str, RP, 1))
// 		return (False);
// 	if (!extract_op(q, str, DS, 1))
// 		return (False);
// 	if (!extract_op(q, str, PIPE, 2))
// 		return (False);
// 	if (!extract_op(q, str, AND, 2))
// 		return (False);
// 	if (!extract_op(q, str, INF, 2))
// 		return (False);
// 	if (!extract_op(q, str, OUF, 2))
// 		return (False);
// 	return (True);
// }

// t_queue	*parse(char *s)
// {
// 	char	*prev_s;
// 	t_queue	*q;

// 	q = NULL;
// 	while (s)
// 	{
// 		while (*s == SPACE || *s == TAB)
// 			s++;
// 		prev_s = s;
// 		parse_metacharacters(&q, &s);
// 		if (s == prev_s)
// 			grab_word(&q, &s);
// 		// fix here
// 	}
// }