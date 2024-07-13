/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_checks.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 15:58:04 by pipolint          #+#    #+#             */
/*   Updated: 2024/07/13 12:04:16 by pipolint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

int	dup_and_check(int fd1, int fd2, t_exec *exec)
{
	if (dup2(fd1, fd2) == -1)
	{
		if (exec)
			exec->last_status = EXIT_FAILURE;
		perror(NULL);
		return (-1);
	}
	return (1);
}

int	pipe_and_check(int *fds, t_exec *exec)
{
	if (pipe(fds) == -1)
	{
		if (exec)
			exec->last_status = EXIT_FAILURE;
		fds[READEND] = -1;
		fds[WRITEEND] = -1;
		perror(NULL);
		return (-1);
	}
	return (1);
}

int	close_and_check(int fd, t_exec *exec)
{
	if (fd < 0)
		return (1);
	if (close(fd) == -1)
	{
		if (exec)
			exec->last_status = EXIT_FAILURE;
		perror(NULL);
		fd = -1;
		return (-1);
	}
	fd = -1;
	return (1);
}

int	open_and_check(int *fd, char *filename, int mode, t_exec *exec)
{
	if (mode == 0)
		*fd = open(filename, O_RDONLY);
	else if (mode == 1)
		*fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	else if (mode == 2)
		*fd = open(filename, O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (*fd == -1)
	{
		if (exec)
			exec->last_status = 127;
		perror(filename);
		return (-1);
	}
	return (1);
}
