/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehammoud <ehammoud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 13:21:55 by ehammoud          #+#    #+#             */
/*   Updated: 2024/03/01 16:47:22 by ehammoud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

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

void	add_to_queue(t_queue **q, char *str)
{
	t_queue	*tmp;

	tmp = new_node(str);
	if (!tmp)
	{
		free_queue(*q);
		*q = NULL;
		free(str);
		return ;
	}
	queue(q, tmp);
	free(str);
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