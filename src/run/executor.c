/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 22:53:20 by marvin            #+#    #+#             */
/*   Updated: 2024/04/01 18:58:42 by pipolint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "../tmp/tmp_utils.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

void	clean_whitespace(t_queue *q)
{
	while (q->next)
	{
		if (q->type == Assign && (q->type == Assign || q->type == Op_redir)
			&& q->next && q->next->type == Word)
			q->next->type = Name;
		if (q->next->type == Whitespace)
			delete_next(&q);
		if (!q->next)
			return ;
		q = q->next;
	}
}

t_bool	redirect(t_cmd *cmd)
{
	int	i;

	i = -1;
	if (cmd->input)
	{
		cmd->in_fd = open(cmd->input, O_RDONLY);
		if (cmd->in_fd == -1)
			return (write_error("Couldn't open infile\n"));
		if (dup2(cmd->in_fd, STDIN_FILENO) == -1)
			return (write_error("Couldn't dup STDIN with infile\n"));
		close(cmd->in_fd);
	}
	while (++i < cmd->outfile_cnt)
	{
		if (cmd->out_flags[i] == 1)
			cmd->out_fd = open(cmd->outfiles[i], O_CREAT | O_APPEND | O_RDWR, 0644);
		else if (!cmd->out_flags[i])
			cmd->out_fd = open(cmd->outfiles[i], O_CREAT | O_TRUNC | O_RDWR, 0644);
		if (cmd->out_fd == -1)
			return (write_error("Couldn't open outfile\n"));
		if (dup2(cmd->out_fd, STDOUT_FILENO) == -1)
			return (write_error("Couldn't dup outfiles\n"));
		close(cmd->out_fd);
	}
	return (True);
}

int	resolve_builtin(t_cmd *cmd, t_env **env)
{
	if (!cmd || !cmd->params || !cmd->params[0])
		return (0);
	if (!ft_strncmp(cmd->params[0], "exit", -1))
		return (-1);
	else if (!ft_strncmp(cmd->params[0], "echo", -1))
	{
		if (cmd->params[1] && !ft_strncmp(cmd->params[1], "-n", 2)
			&& !echo(cmd->params + 2, True))
			return (-1);
		else if (!echo(cmd->params + 1, False))
			return (-1);
	}
	else if (!ft_strncmp(cmd->params[0], "cd", -1))
	{
		if (cmd->params[1]
			&& !cd(get_var(*env, "PWD"), get_var(*env, "HOME"), env))
			return (-1);
		else if (!cd(get_var(*env, "PWD"), cmd->params[1], env))
			return (-1);
	}
	else if (!ft_strncmp(cmd->params[0], "env", -1) && !print_env(*env))
		return (-1);
	else if (!ft_strncmp(cmd->params[0], "unset", -1))
		delete_var(env, cmd->params[1]);
	else if (!ft_strncmp(cmd->params[0], "pwd", -1)
		&& ft_printf("%s\n", get_var(*env, "PWD")) == -1)
		return (-1);
	else if (!ft_strncmp(cmd->params[0], "export", -1)
		&& !export_cmd(env, cmd->params + 1))
		return (-1);
	else
		return (0);
	return (1);
}

// /// @brief searches for command in the env path variable
// /// @param env the environment with the path variable
// /// @param cmd the command
// /// @return returns path if found, else NULL
char	*search_path(t_env **env, t_cmd *cmd)
{
 	char	*path;
 	char	**paths;
 	char	*com;
 	int		i;

 	path = get_var(*env, "PATH");
 	if (!path)
 		return (NULL);
 	paths = ft_split(path, ':');
 	if (!paths)
 	{
		write_error("Couldn't find path\n");
 		return (NULL);
 	}
 	i = -1;
 	while (paths[++i])
 	{
 		com = ft_strjoin_chr(paths[i], '/', cmd->params[0]);
 		if (!com)
 			return (NULL);
 		if (!access(com, X_OK | F_OK))
 			return (com);
 	}
 	return (NULL);
}

t_bool	pipe_cmd(t_cmd *cmd, t_env **env)
{
	int		pipes[2];
	pid_t	s;
	
	if (cmd->after != Op_pipe)
		return (True);
	if (pipe(pipes) < 0)
		return (write_error("Couldn't open pipes\n"));
	
}

t_bool	execute(t_env **env, t_cmd *cmd)
{
	if (cmd->input || (cmd->outfile_cnt && cmd->outfiles && cmd->outfiles[0]))
	{
		if (redirect(cmd) == False)
			return (False);
	}
	if (execve(search_path(env, cmd), cmd->params, to_char_arr(env)) == -1)
	{
		write_error("Execution error\n");
		return (False);
	}
	return (True);
}

t_bool	execute_command(t_env **env, t_cmd **cmd)
{
	pid_t	id;
	t_cmd	*tmp;

	tmp = *cmd;
	while (tmp)
	{
		if (resolve_builtin(tmp, env) != 1)
		{
			id = fork();
			if (id == 0)
			{
				if (execute(env, tmp) == False)
				{
					write_error("Couldn't execute command\n");
					exit(EXIT_FAILURE);
				}
			}
			else
				wait(&tmp->status);
		}
		tmp = tmp->next;
	}
	return (True);
}
