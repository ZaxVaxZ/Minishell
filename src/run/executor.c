/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 22:53:20 by marvin            #+#    #+#             */
/*   Updated: 2024/03/27 20:36:27 by pipolint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

void	clean_whitespace(t_queue *q)
{
	while (q->next)
	{
		if (q->next->type == Whitespace)
			delete_next(&q);
		if (!q->next)
			return ;
		q = q->next;
	}
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
 		&& !export_var(env, cmd->params[1]))
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
 		printf("Couldn't find paths\n");
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

//void	execute_command(t_env **env, t_cmd *cmd)
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
//}

//t_bool	pipe(t_cmd *cmd)
//{
	
//}

// t_bool	redirect(t_cmd *cmd)
// {
// 	if (cmd->input)
// 	{
// 		cmd->in_fd = open(cmd->input, O_RDONLY);
// 		if (cmd->in_fd == -1)
// 		{
// 			ft_printf("Couldn't open infile\n");
// 			return (False);
// 		}
// 		if (dup2(cmd->in_fd, STDIN_FILENO) == -1)
// 		{
// 			ft_printf("Couldn't dup with stdint\n");
// 			return (False);
// 		}
// 		ft_printf("Opened infile and duped with stdin\n");
// 	}
// 	if (cmd->output)
// 	{
// 		cmd->out_fd = open(cmd->output, O_CREAT | O_RDWR | O_TRUNC, 0777);
// 		if (cmd->out_fd == -1)
// 		{
// 			ft_printf("Couldn't open outfile\n");
// 			return (False);
// 		}
// 		if (dup2(cmd->out_fd, STDOUT_FILENO) == -1)
// 		{
// 			ft_printf("Couldn't dup with stdout\n");
// 			return (False);
// 		}
// 		ft_printf("Opened outfile and duped with stdout\n");
// 	}
// 	return (True);
// }

 t_bool	redirect(t_cmd *cmd)
 {
 	if (cmd->input)
 	{
 		cmd->in_fd = open(cmd->input, O_RDONLY);
 		if (cmd->in_fd == -1)
 		{
			perror(cmd->input);
 			return (False);
 		}
 		if (dup2(cmd->in_fd, STDIN_FILENO) == -1)
 		{
			perror(NULL);
 			return (False);
 		}
 	}
 	//if (cmd->ovrw_outs || cmd->apnd_outs)
 	//{
 	//	cmd->out_fd = open(cmd->ovrw_outs, O_CREAT | O_RDWR | O_TRUNC, 0777);
 	//	if (cmd->out_fd == -1)
 	//	{
 	//		ft_printf("Couldn't open outfile\n");
 	//		return (False);
 	//	}
 	//	if (dup2(cmd->out_fd, STDOUT_FILENO) == -1)
 	//	{
 	//		ft_printf("Couldn't dup with stdout\n");
 	//		return (False);
 	//	}
 	//	ft_printf("Opened outfile and duped with stdout\n");
 	//}
 	return (True);
 }

void	execute_command(t_env **env, t_cmd **cmd)
{
	pid_t	id;
	t_cmd	*tmp;
	int		i;

	tmp = *cmd;
	while (tmp)
	{
		if (resolve_builtin(tmp, env) != 1)
		{
			id = fork();
			if (id == 0)
			{
				i = 0;
				if (redirect(tmp) == False)
					exit(EXIT_FAILURE);
				if (execve(search_path(env, tmp), tmp->params, to_char_arr(env)) == -1)
				{
					printf("Error occured in execution\n");
					return ;
				}
			}
			else
				wait(&tmp->status);
		}
		tmp = tmp->next;
	}
}
