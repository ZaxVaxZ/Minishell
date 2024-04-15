/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 22:53:31 by marvin            #+#    #+#             */
/*   Updated: 2024/04/15 08:30:40 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include <stdlib.h>
# include <sys/wait.h>
# include "libft.h"
# include "parser.h"
# include "builtins.h"
# include "cmd_list.h"
# include "queues.h"
# include "../tmp/tmp_utils.h"
# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>
# include "get_next_line.h"

# define NON 0
# define OR_OP 1
# define AND_OP 2
# define PIPE_OP 3
# define SEMICOLON 4
# define DO_NOT_EXECUTE 0
# define WAIT_THEN_EXEC 1
# define IMMEDIATE_EXEC 2
# define CHILD_PROCESS 0
# define SUCCESS 0

typedef struct s_exec
{
	int		ret;
	int		curr_depth;
	int		status_depth;
	int		overall_status;
	int		last_status;
	int		last_op;
	pid_t	last_pid;
	t_cmd	*cmd_head;
}	t_exec;

t_bool	clean_whitespace(t_queue *q);
int		resolve_builtin(t_env **env, t_cmd *cmd);
int		execute_commands(t_env **env, t_cmd **cmd, int *status);
t_bool	clean_whitespace(t_queue *q);
int		resolve_builtin(t_env **env, t_cmd *cmd);
t_bool	redirect(t_cmd *cmd);
int		exec_type(t_exec *exec, t_cmd **cmd);
char	*search_path(t_env **env, t_cmd *cmd);
t_bool	should_exec(t_exec *exec, t_cmd *cmd);
void	wait_for_children(t_exec *exec);
int		after_to_op(t_cmd *cmd);

#endif
