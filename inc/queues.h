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
# include "libft.h"
# include "structs.h"

t_queue	*new_node(char *s);
char	*dequeue(t_queue **q);
t_queue	*queue_end(t_queue *q);
t_bool	free_queue(t_queue **q);
void	queue(t_queue **q, t_queue *node);
void	print_queue(t_queue *queue);

#endif