/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cleanup.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 04:46:55 by marvin            #+#    #+#             */
/*   Updated: 2024/05/28 17:23:56 by pipolint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

/* -----------------------
 * Functions in the file:
 *   delete_next()
 *   join_quote()
 *   join_words()
 *   unpack_vars()
 *   parse_clean_up()
 * -----------------------*/

/// @brief Takes a node and deletes the node after it in the queue
/// @param q The node whose next will be deleted
void	delete_next(t_queue **q)
{
	t_queue	*tmp;

	if (!(*q)->next)
		return ;
	tmp = (*q)->next;
	(*q)->next = tmp->next;
	free(tmp->s);
	free(tmp);
}

/// @brief Collapse a Double or Single quote sequence into a full string
/// @param q The starting node with an open quote
/// @param type The type of quote the node has: Sq_open or Dq_open
/// @return False if any malloc fails, True otherwise
static t_bool	join_quote(t_queue *q, t_token type)
{
	char	*addition;

	if (!q || !q->next)
		return (True);
	q->type = Word;
	free(q->s);
	q->s = ft_strdup("");
	if (!q->s)
		return (False);
	while (q->next && ((type == Sq_open && q->next->type != Sq_closed)
			|| (type == Dq_open && q->next->type != Dq_closed)))
	{
		addition = q->next->s;
		if (q->next->type == Variable)
			delete_next(&q);
		if (q->next->type == Name)
			addition = getenv(q->next->s);
		q->s = ft_strjoin(q->s, addition);
		if (!q->s)
			return (False);
		delete_next(&q);
	}
	delete_next(&q);
	return (True);
}

/// @brief Joins all quotes and consecutive words when found
/// @param h The top node of the queue
/// @return False if any malloc fails, True otherwise
static t_bool	join_words(t_queue **h)
{
	t_queue	*q;

	q = *h;
	while (q)
	{
		if ((q->type == Sq_open || q->type == Dq_open)
			&& !join_quote(q, q->type))
			return (free_queue(h));
		q = q->next;
	}
	q = *h;
	while (q)
	{
		while (q->type == Word && q->next && q->next->type == Word)
		{
			q->s = ft_strjoin(q->s, q->next->s);
			if (!q->s)
				return (free_queue(h));
			delete_next(&q);
		}
		q = q->next;
	}
	return (True);
}

/// @brief Resolves variables to their value and handles open quotes or brackets
/// @param h Reference to the top node of the queue
/// @param q The top node passed by value to loop through the queue
/// @param open Array with the number of quotes or brackets left open
/// @return False if any malloc fails, True otherwise
static int	unpack_vars(t_queue **h, t_queue *q, int *open, t_env *env)
{
	char	*tmp;
	char	*dollar;
	int		variable;
	static int	switched_variable = 0;

	if (open[0])
		return (syntax_error(h, "'", True, False));
	if (open[1])
		return (syntax_error(h, "\"", True, False));
	if (open[2])
		return (syntax_error(h, ")", True, False));
	variable = 0;
	while (q)
	{
		if (q->type == Op_redir && !ft_strncmp("<<", q->s, -1) && switched_variable)
		{
			switched_variable = 0;
			return (0);
		}
		else if (q->type == Op_redir && !ft_strncmp("<<", q->s, -1) && !switched_variable)
		{
			if (q->type == Op_redir && !ft_strncmp("<<", q->s, -1) && q->next)
			{
				if (q->next->type == Variable)
				{
					variable = 1;
					delete_next(&q);
				}
				q = q->next;
			}
			while (q && q->next)
			{
				if (q->next->type == Variable)
				{
					variable = 1;
					delete_next(&q);
				}
				if (q->type != Whitespace && q->type != Variable)
					break ;
				else
					q = q->next;
			}
			if (variable)
			{
				q->type = Word;
				dollar = ft_strdup("$");
				tmp = ft_strjoin(dollar, q->s);
				free(q->s);
				q->s = tmp;
				switched_variable = 1;
			}
		}
		else if (q->type == Variable)
		{
			q->type = Word;
			free(q->s);
			if (get_var(env, q->next->s))
				q->s = ft_strdup(get_var(env, q->next->s));
			else
				q->s = ft_strdup("");
			if (!q->s)
				return (free_queue(h) - 1);
			delete_next(&q);
		}
		q = q->next;
	}
	return (0);
}

/// @brief Cleans up the queue post-parsing for an easier execution run
/// @param h The top node of the queue
/// @return 1 if there's a parsing issue, -1 if a malloc fails, 0 otherwise
int	parse_clean_up(t_queue **h, t_env *envp)
{
	int		open[3];
	int		ret;
	t_queue	*q;

	if (!h || !*h)
		return (-2);
	open[0] = 0;
	open[1] = 0;
	open[2] = 0;
	q = *h;
	if (queue_end(q)->type == Illegal)
		return (syntax_error(h, queue_end(q)->s, False, (!queue_end(q)->s)));
	while (q)
	{
		open[0] += (q->type == Sq_open) - (q->type == Sq_closed);
		open[1] += (q->type == Dq_open) - (q->type == Dq_closed);
		open[2] += (q->type == Bracket_open) - (q->type == Bracket_closed);
		q = q->next;
	}
	ret = unpack_vars(h, *h, open, envp);
	if (!join_words(h))
		return (-1);
	return (ret);
}
