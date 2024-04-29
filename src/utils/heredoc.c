/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 15:54:35 by pipolint          #+#    #+#             */
/*   Updated: 2024/04/29 17:21:53 by pipolint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "get_next_line.h"
#include "general.h"

void	handle(int sig)
{
	(void)sig;
}

void	heredoc_child(t_cmd *cmd, t_exec *exec, int *fds, int i, t_env **env)
{
	char	*line;
	char	**words;
	char	*ret;
	int		word_i;

	close(fds[0]);
	exec->last_status = SUCCESS;
	while (1)
	{
		write(1, "> ", 2);
		line = get_next_line(STDIN_FILENO);
		ret = expand_variable(line, env, words, &word_i);
		if (!ft_strncmp(ret, cmd->infiles[i], ft_strlen(ret) - 1)
			&& ft_strlen(ret) - 1 == ft_strlen(cmd->infiles[i]))
			break ;
		if (write(fds[1], ret, ft_strlen(ret)) == -1)
		{
		 	exec->last_status = EXIT_FAILURE;
			break ;
		}
		free(ret);
		free(line);
	}
	close(fds[1]);
	if (line)
		free(line);
	exit(exec->last_status);
}

t_bool	heredoc_parent(t_cmd **cmd, int *fds, t_exec *exec)
{
	waitpid(exec->last_pid, &exec->last_status, 0);
	close(fds[1]);
	(*cmd)->in_fd = dup(fds[0]);
	if ((*cmd)->in_fd == -1)
		return (False);
	close(fds[0]);
	return (True);
}

t_bool	heredoc_loop(t_cmd *cmd, t_exec *exec, t_env **env)
{
	int	i;
	int	heredoc_fds[2];
	int	s;

	i = 0;
	if (cmd->heredoc)
	{
		while (i < cmd->infile_cnt)
		{
			if (cmd->in_flags[i])
				heredoc(cmd, exec, heredoc_fds, i, env);
			if (i != cmd->infile_cnt - 1)
			{
				if (close_and_check(cmd->in_fd, exec) == -1)
					return (False);
			}
			i++;
		}
	}
	return (True);
}

t_bool	heredoc(t_cmd *cmd, t_exec *exec, int *fds, int i, t_env **env)
{
	pid_t	p;

	if (pipe_and_check(fds, exec) == -1)
		return (False);
	signal(SIGQUIT, handle);
	signal(SIGINT, handle);
	p = fork();
	if (p < 0)
	{
		perror(NULL);
		return (False);
	}
	exec->last_pid = p;
	if (p == 0)
		heredoc_child(cmd, exec, fds, i, env);
	if (heredoc_parent(&cmd, fds, exec) == False)
		return (False);
	return (True);
}
