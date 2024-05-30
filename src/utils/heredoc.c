/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 15:54:35 by pipolint          #+#    #+#             */
/*   Updated: 2024/05/30 18:09:47 by pipolint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "get_next_line.h"
#include "general.h"

extern int	g_signum;

int	check_and_write(t_heredoc *h, char **ret, char **line)
{
	if (!ft_strncmp(*ret, h->cmd->infiles[h->i], ft_strlen(*ret) - 1)
		&& ft_strlen(*ret) - 1 == ft_strlen(h->cmd->infiles[h->i]))
		return (0);
	if (write(h->fds[1], *ret, ft_strlen(*ret)) == -1)
	{
		h->exec->last_status = EXIT_FAILURE;
		return (-1);
	}
	if (*ret)
	{
		free(*ret);
		*ret = NULL;
	}
	if (*line)
	{
		free(*line);
		*line = NULL;
	}
	return (1);
}

void	heredoc_child(t_heredoc *h)
{
	char		*line;
	char		*ret;
	t_expand	exp;
	void		(*s)(int);

	close(h->fds[0]);
	h->exec->last_status = SUCCESS;
	g_signum = -1;
	s = signal(SIGINT, sig_heredoc);
	while (1)
	{
		write(1, "> ", 2);
		line = get_next_line(STDIN_FILENO);
		if (init_expand(&exp, line, h->env, h->cmd->infiles[h->i]) == -1)
			break ;
		ret = expand_variable(&exp);
		if (check_and_write(h, &ret, &line) <= 0)
			break ;
	}
	close(h->fds[1]);
	if (ret)
		free(ret);
	if (line)
		free(line);
	exit(h->exec->last_status);
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
	t_heredoc	h;
	int			heredoc_fds[2];

	h.i = -1;
	h.cmd = cmd;
	h.exec = exec;
	h.env = env;
	h.fds = heredoc_fds;
	if (cmd->heredoc)
	{
		while (++h.i < cmd->infile_cnt)
		{
			if (cmd->in_flags[h.i])
			{
				if (heredoc(&h) == False)
					return (False);
			}
			if (cmd->in_flags[h.i] && h.i != cmd->infile_cnt - 1)
			{
				if (close_and_check(cmd->in_fd, exec) == -1)
					return (False);
			}
		}
	}
	return (True);
}

t_bool	heredoc(t_heredoc *h)
{
	pid_t	p;

	if (pipe_and_check(h->fds, h->exec) == -1)
		return (False);
	p = fork();
	if (p < 0)
	{
		perror(NULL);
		return (False);
	}
	h->exec->last_pid = p;
	if (p == 0)
		heredoc_child(h);
	return (heredoc_parent(&h->cmd, h->fds, h->exec));
}
