/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 22:53:31 by marvin            #+#    #+#             */
/*   Updated: 2024/04/04 04:46:32 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include <stdlib.h>
# include "libft.h"
# include "parser.h"
# include "builtins.h"
# include "cmd_list.h"

typedef struct s_exec
{
	int	status_depth;
	int	curr_depth;
	int	last_status;
	int	overall_status;
}	t_exec;

t_bool	clean_whitespace(t_queue *q);
int		resolve_builtin(t_env **env, t_cmd *cmd);
int		execute_command(t_env **env, t_cmd **cmd, int *status);

#endif
