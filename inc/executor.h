/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 22:53:31 by marvin            #+#    #+#             */
/*   Updated: 2024/03/18 15:58:28 by pipolint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include <stdlib.h>
# include "libft.h"
# include "parser.h"
# include "builtins.h"

typedef enum e_cmd_type
{
	Cd,
	Pwd,
	Env,
	Echo,
	Exit,
	Unset,
	Export,
	Not_builtin
}	t_cmd_type;

typedef struct s_command
{
	int					in_fd;
	int					out_fd;
	char				*input;
	char				*output;
	char				**params;
	int					status;
	struct s_command	*next;
}	t_command;

void	clean_whitespace(t_queue *q);
t_bool	resolve_builtin(t_command *cmd, t_env **env);
t_bool	build_commands(t_queue *q, t_command *cmd);
char	*search_path(t_env **env, t_command *cmd);
void	execute_command(t_env **env, t_command *cmd, t_queue **q);

#endif
