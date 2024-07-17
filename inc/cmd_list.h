/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_list.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 05:47:46 by marvin            #+#    #+#             */
/*   Updated: 2024/07/17 20:47:27 by pipolint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMD_LIST_H
# define CMD_LIST_H

# include "queues.h"
# include "env_list.h"
# include "general.h"

t_cmd	*new_cmd_node(char **params, t_queue **q);
void	add_cmd_node(t_cmd **cmd, t_cmd *to_add);
t_bool	free_cmd_node(t_cmd *node);
t_bool	free_cmd(t_cmd **cmd);
void	delete_next_cmd(t_cmd **cmd);
t_bool	is_separator(t_queue *node);
t_bool	free_and_return(t_queue **q, t_env **env, t_cmd **cmds, t_cmd *cmd);
t_bool	build_commands(t_main *m);
t_bool	prep_cmd(t_queue *q, t_cmd **node);

#endif