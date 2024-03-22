/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_list.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 05:47:46 by marvin            #+#    #+#             */
/*   Updated: 2024/03/22 05:47:46 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMD_LIST_H
# define CMD_LIST_H

# include "queues.h"

typedef struct s_cmd
{
	int				status;
	int				in_fd;
	int				out_fd;
	char			*input;
	char			*output;
	char			**params;
	t_bool			or_op;
	t_token			before;
	t_token			after;
	struct s_cmd	*next;
}	t_cmd;

t_cmd	*new_cmd_node(char **params);
void	add_cmd_node(t_cmd **cmd, t_cmd *to_add);
void	free_cmd_node(t_cmd *node);
t_bool	free_cmd(t_cmd **cmd);
int		cmd_size(t_cmd *cmd);

#endif