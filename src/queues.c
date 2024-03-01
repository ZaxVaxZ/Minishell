/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   queue_essentials.c								 :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: codespace <codespace@student.42.fr>		+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2024/01/25 16:50:24 by codespace		 #+#	#+#			 */
/*   Updated: 2024/01/25 16:50:24 by codespace		###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "queues.h"

t_queue	*new_node(char *s)
{
	t_queue	*node;

	node = malloc(sizeof(t_queue));
	if (!node)
		return (NULL);
	node->s = ft_strdup(s);
	if (!node->s)
	{
		free(node);
		return (NULL);
	}
	node->next = NULL;
	return (node);
}

t_queue	*queue_end(t_queue *q)
{
	while (q && q->next)
		q = q->next;
	return (q);
}

void	queue(t_queue **q, t_queue *node)
{
	if (!q || !node)
		return ;
	if (!(*q))
		*q = node;
	else
		queue_end(*q)->next = node;
}

char	*dequeue(t_queue **q)
{
	char	*ret;
	t_queue	*tmp;

	if (!q || !(*q))
		return (NULL);
	tmp = *q;
	*q = tmp->next;
	ret = tmp->s;
	free(tmp);
	return (ret);
}

void	free_queue(t_queue **q)
{
	t_queue	*tmp;

	if (!q || !(*q))
		return ;
	while (*q)
	{
		tmp = *q;
		*q = (*q)->next;
		free(tmp->s);
		free(tmp);
	}
}
