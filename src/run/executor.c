/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 22:53:20 by marvin            #+#    #+#             */
/*   Updated: 2024/03/18 16:07:08 by pipolint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

void	clean_whitespace(t_queue *q)
{
	while (q->next)
	{
		if (q->next->type == Whitespace)
			delete_next(&q);
		q = q->next;
	}
}

// int	resolve_builtin(t_cmd *cmd, t_env **env)
// {
// 	if (!cmd || !cmd->params || !cmd->params[0])
// 		return (0);
// 	if (!ft_strncmp(cmd->params[0], "exit", -1))
// 		return (-1);
// 	else if (!ft_strncmp(cmd->params[0], "echo", -1))
// 	{
// 		if (cmd->params[1] && !ft_strncmp(cmd->params[1], "-n", 2)
// 			&& !echo(cmd->params + 2, True))
// 			return (-1);
// 		else if (!echo(cmd->params + 1, False))
// 			return (-1);
// 	}
// 	else if (!ft_strncmp(cmd->params[0], "cd", -1))
// 	{
// 		if (cmd->params[1]
// 			&& !cd(get_var(*env, "PWD"), get_var(*env, "HOME"), env))
// 			return (-1);
// 		else if (!cd(get_var(*env, "PWD"), cmd->params[1], env))
// 			return (-1);
// 	}
// 	else if (!ft_strncmp(cmd->params[0], "env", -1) && !print_env(*env))
// 		return (-1);
// 	else if (!ft_strncmp(cmd->params[0], "unset", -1))
// 		delete_var(env, cmd->params[1]);
// 	else if (!ft_strncmp(cmd->params[0], "pwd", -1)
// 		&& ft_printf("%s\n", get_var(*env, "PWD")) == -1)
// 		return (-1);
// 	else if (!ft_strncmp(cmd->params[0], "export", -1)
// 		&& !export_var(env, cmd->params[1]))
// 		return (-1);
// 	else
// 		return (0);
// 	return (1);
// }

// /// @brief searches for command in the env path variable
// /// @param env the environment with the path variable
// /// @param cmd the command
// /// @return returns path if found, else NULL
// char	*search_path(t_env **env, t_cmd *cmd)
// {
// 	char	*path;
// 	char	**paths;
// 	char	*com;
// 	int		i;

// 	path = get_var(*env, "PATH");
// 	if (!path)
// 		return (NULL);
// 	paths = ft_split((const char *)path, ':');
// 	if (!paths)
// 	{
// 		printf("Couldn't find paths\n");
// 		return (NULL);
// 	}
// 	i = -1;
// 	while (paths[++i])
// 	{
// 		com = ft_strjoin_chr(paths[i], '/', cmd->params[0]);
// 		if (!com)
// 			return (NULL);
// 		if (!access(com, X_OK | F_OK))
// 			return (com);
// 	}
// 	return (NULL);
// }

// void	execute_command(t_env **env, t_cmd *cmd, t_queue **q)
// {
// 	int		status;
// 	char	*valid;
// 	pid_t	test;

// 	valid = search_path(env, cmd);
// 	if (!valid)
// 	{
// 		printf("Couldn't find command\n");
// 		return ;
// 	}
// 	test = fork();
// 	if (test == 0)
// 	{
// 		status = execve(valid, cmd->params, to_char_arr(env));
// 		if (status == -1)
// 		{
// 			write(2, "execution error", 15);
// 			return ;
// 		}
// 	}
// 	else
// 		wait(NULL);
// }
