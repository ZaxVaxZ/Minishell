/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 15:54:35 by pipolint          #+#    #+#             */
/*   Updated: 2024/04/17 19:38:22 by pipolint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "get_next_line.h"

t_bool	heredoc_child(t_cmd *cmd, t_exec *exec, int *fds)
{
	char	*line;

	while (1)
	{
		write(1, "> ", 2);
		line = get_next_line(STDIN_FILENO);
		if (!ft_strncmp(line, cmd->input, ft_strlen(line) - 1)
			&& ft_strlen(line) - 1 == ft_strlen(cmd->input))
			break ;
		if (write(fds[1], line, ft_strlen(line)) == -1)
			return (False);
		free(line);
	}
	close(fds[1]);
	if (line)
		free(line);
	return (True);
}

t_bool	heredoc_parent(int *fds, t_exec *exec)
{
	wait(NULL);
	if (dup_and_check(fds[0], STDIN_FILENO, exec) == -1)
		return (False);
	close(fds[0]);
	return (True);
}

t_bool	heredoc(t_env **env, t_cmd *cmd, t_exec *exec, int *fds)
{
	if (pipe_and_check(fds, exec) == -1)
		return (False);
	if (heredoc_child(cmd, exec, fds) == False)
		return (False);
	if (heredoc_parent(fds, exec) == False)
		return (False);
	return (True);
}
