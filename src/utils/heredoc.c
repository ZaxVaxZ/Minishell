/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 15:54:35 by pipolint          #+#    #+#             */
/*   Updated: 2024/04/18 11:41:50 by pipolint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "get_next_line.h"

t_bool	heredoc_child(t_cmd *cmd, t_exec *exec, int *fds)
{
	char	*line;

	close(fds[0]);
	while (1)
	{
		write(1, "> ", 2);
		line = get_next_line(STDIN_FILENO);
		if (!ft_strncmp(line, cmd->input, ft_strlen(line) - 1)
			&& ft_strlen(line) - 1 == ft_strlen(cmd->input))
			break ;
		if (write(fds[1], line, ft_strlen(line)) == -1)
		{
			exec->last_status = EXIT_FAILURE;
			exit(EXIT_FAILURE);
		}
		free(line);
	}
	close(fds[1]);
	if (line)
		free(line);
	exit(0);
}

t_bool	heredoc_parent(int *fds, t_exec *exec)
{
	wait(NULL);
	close(fds[1]);
	if (dup_and_check(fds[0], STDIN_FILENO, exec) == -1)
		return (False);
	close(fds[0]);
	return (True);
}

t_bool	heredoc(t_cmd *cmd, t_exec *exec, int *fds)
{
	pid_t	p;

	if (pipe_and_check(fds, exec) == -1)
		return (False);
	p = fork();
	if (p < 0)
	{
		perror(NULL);
		return (False);
	}
	if (p == 0)
	{
		if (heredoc_child(cmd, exec, fds) == False)
			return (False);
	}
	if (heredoc_parent(fds, exec) == False)
		return (False);
	return (True);
}
