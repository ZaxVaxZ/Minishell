/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_list.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehammoud <ehammoud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 05:47:46 by marvin            #+#    #+#             */
/*   Updated: 2024/03/26 14:26:50 by ehammoud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMD_LIST_H
# define CMD_LIST_H

# include "queues.h"
# include "env_list.h"

typedef struct s_cmd
{
	int				depth;
	int				status;
	int				in_fd;
	int				out_fd;
	char			*input;
	int				params_cnt;
	int				ovrw_cnt;
	int				apnd_cnt;
	char			**ovrw_outs;
	char			**apnd_outs;
	char			**params;
	t_bool			heredoc;
	t_bool			or_op;
	t_token			before;
	t_token			after;
	struct s_cmd	*next;
}	t_cmd;

t_cmd	*new_cmd_node(char **params);
void	add_cmd_node(t_cmd **cmd, t_cmd *to_add);
t_bool	free_cmd_node(t_cmd *node);
t_bool	free_cmd(t_cmd **cmd);
int		cmd_size(t_cmd *cmd);
t_bool	is_separator(t_queue *node);
t_bool	free_and_return(t_queue **q, t_env **env, t_cmd **cmds, t_cmd *cmd);
t_bool	build_commands(t_queue **queue, t_cmd **cmds, t_env **env);
t_bool	prep_cmd(t_queue *q, t_cmd **node);

#endif