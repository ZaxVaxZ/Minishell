/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehammoud <ehammoud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 13:21:55 by ehammoud          #+#    #+#             */
/*   Updated: 2024/03/01 20:57:28 by ehammoud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

/*
blank:  A space or tab.
word:   A sequence of characters considered as a single unit by the shell.  Also known as a token.
name:   A word consisting only of alphanumeric characters and underscores, and beginning with an alphabetic character or an underscore.  Also referred to as an identifier.
metacharacter:
		A character that, when unquoted, separates words.  One of the following:
		|  & ; ( ) < > space tab
control operator:
		A token that performs a control function.  It is one of the following symbols:
		|| & && ; ;; ( ) | <newline>
*/

/// @brief adds the closing bracket to the queue if the opening bracket exists
/// @param q the pointer to the queue
/// @param c the closing bracket
void	close_exists(t_queue **q, char c)
{
	t_queue	*node;
	t_queue	*trav;

	node = malloc(sizeof(t_queue));
	if (!node)
		return ;
	trav = (*q);
	if (c == RP)
	{
		while (trav)
		{
			if (c == LP)
			{
				node->s = ft_strdup(LP);
				queue(q, node);
				break ;
			}
			trav = trav->next;
		}
	}
}

t_bool	add_str_to_queue(t_queue **q, char *str)
{
	t_queue	*tmp;

	if (!str)
		return (True);
	tmp = new_node(str);
	if (!tmp)
		free_queue(q);
	else
	{
		tmp->type = token_type(tmp->s);
		queue(q, tmp);
	}
	free(str);
	return (*q != NULL);
}

t_bool	extract_op(t_queue **q, char *s, char op, int max_occurs)
{
	int		occurs;

	occurs = op_occur(op, s);
	// if (occurs != -1 && occurs > allowed_occurs)
	// {
	// 	free(s);
	// 	free_queue(q);
	// 	return (False);
	// }
	if (!add_str_to_queue(q, ft_substr(s, 0, occurs)))
		return (False);
	return (True);
}

t_bool	add_to_queue(t_queue **q, char *str)
{
	if (!extract_op(q, str, PIPE, 2))
		return (False);
	if (!extract_op(q, str, AND, 2))
		return (False);
	if (!extract_op(q, str, INF, 2))
		return (False);
	if (!extract_op(q, str, OUF, 2))
		return (False);
	if (!extract_op(q, str, LP, 1))
		return (False);
	if (!extract_op(q, str, RP, 1))
		return (False);
}

t_queue	*parse(char *s)
{
	int		i;
	char	**strs;
	t_queue	*q;

	q = NULL;
	strs = ft_split(s, "\t ");
	if (!strs)
		return (NULL);
	i = 0;
	while (strs[i])
	{
		add_to_queue(&q, strs[i]);
		i++;
	}
	return (q);
}