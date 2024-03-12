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

typedef enum e_token
{
	Word,
	Name,
	Assign,
	Op_redir,
	Op_logic,
	Op_pipe,
	Variable,
	Semicolon,
	Sq_open,
	Sq_closed,
	Dq_open,
	Dq_closed,
	Bracket_open,
	Bracket_closed,
	Whitespace,
	Illegal
}	t_token;

typedef struct s_queue
{
	char			*s;
	int				opened;
	t_token			type;
	struct s_queue	*next;
}	t_queue;

t_queue	*new_node(char *s);
char	*dequeue(t_queue **q);
t_queue	*queue_end(t_queue *q);
void	free_queue(t_queue **q);
void	queue(t_queue **q, t_queue *node);

#endif