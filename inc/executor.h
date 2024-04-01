/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 22:53:31 by marvin            #+#    #+#             */
/*   Updated: 2024/04/01 17:54:23 by pipolint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include <stdlib.h>
# include "libft.h"
# include "parser.h"
# include "builtins.h"
# include "cmd_list.h"

void	clean_whitespace(t_queue *q);
// int		resolve_builtin(t_cmd *cmd, t_env **env);
// t_bool	build_command(t_queue *q, t_cmd *cmd);
// char	*search_path(t_env **env, t_cmd *cmd);
t_bool	execute_command(t_env **env, t_cmd **cmd);

#endif
