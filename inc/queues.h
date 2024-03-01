/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   queues.h										   :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: codespace <codespace@student.42.fr>		+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2024/01/25 16:50:17 by codespace		 #+#	#+#			 */
/*   Updated: 2024/01/25 16:50:17 by codespace		###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#ifndef QUEUES_H
# define QUEUES_H

# include <stdlib.h>

typedef struct s_queue
{
	char			*s;
	struct s_queue	*next;
}	t_queue;

t_queue	*new_node(char *s);
void	queue(t_queue **q, t_queue *node);
char	*dequeue(t_queue **q);
void	free_queue(t_queue **q);

#endif