/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 15:54:35 by pipolint          #+#    #+#             */
/*   Updated: 2024/07/24 17:19:27 by pipolint         ###   ########.fr       */
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
	if (write(h->fds[WRITEEND], *ret, ft_strlen(*ret)) == -1)
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

void	heredoc_sigint(int sig)
{
	g_signum = sig;
	write(1, "\n", 1);
	close(STDIN_FILENO);
}

void	heredoc_child(t_heredoc *h)
{
	char		*line;
	char		*tmp;

	signal(SIGINT, heredoc_sigint);
	signal(SIGQUIT, SIG_IGN);
	close(h->fds[READEND]);
	g_signum = -1;
	while (1)
	{
		line = readline("> ");
		if (g_signum == SIGINT)
			heredoc_exit(h);
		tmp = line;
		line = ft_strjoin_chr(line, '\n', "");
		if (tmp)
			free(tmp);
		if (should_break_heredoc(h, line))
			break ;
		write_exp_str(h, line);
		free(line);
	}
	close(h->fds[WRITEEND]);
	if (line)
		free(line);
	close(h->exec->std_in);
	close(h->exec->std_out);
	free_and_exit(h->m, -1);
}

void	nothing(int sig)
{
	(void)sig;
}

t_bool	heredoc_parent(t_main *m, t_cmd **cmd, int *fds, t_exec *exec)
{
	int		ex;
	char	*tmp;

	close(fds[WRITEEND]);
	signal(SIGINT, nothing);
	signal(SIGQUIT, nothing);
	waitpid(exec->last_pid, &ex, 0);
	signal(SIGINT, sig_handle);
	signal(SIGQUIT, SIG_IGN);
	if (WEXITSTATUS(ex) == 252)
	{
		close(fds[WRITEEND]);
		close(fds[READEND]);
		exec->last_status = 1;
		(*cmd)->status = exec->last_status;
		tmp = ft_itoa(1);
		if (!tmp)
			return (False);
		set_var(&m->env, "?", tmp, False);
		(*cmd)->heredoc_passed = 0;
		free(tmp);
		return (False);
	}
	(*cmd)->in_fd = dup(fds[READEND]);
	if ((*cmd)->in_fd == -1)
		return (False);
	fprintf(stderr, "in fd %d\n", (*cmd)->in_fd);
	close(fds[READEND]);
	if (ex == EXIT_FAILURE)
		return (False);
	return (True);
}

t_bool	heredoc_loop(t_main *m, t_cmd *cmd, t_exec *exec, t_env **env)
{
	t_heredoc	h;
	int			heredoc_fds[2];

	h.i = -1;
	h.m = m;
	h.cmd = cmd;
	h.exec = exec;
	h.env = env;
	h.fds = heredoc_fds;
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
	return (True);
}

t_bool	heredoc(t_heredoc *h)
{
	pid_t	p;

	if (pipe_and_check(h->fds, h->exec) == -1)
		return (False);
	fprintf(stderr, "readend pid: %d\n", h->fds[READEND]);
	fprintf(stderr, "writeend pid: %d\n", h->fds[WRITEEND]);
	p = fork();
	if (p < 0)
	{
		perror(NULL);
		return (False);
	}
	h->exec->last_pid = p;
	if (p == 0)
		heredoc_child(h);
	return (heredoc_parent(h->m, &h->cmd, h->fds, h->exec));
}
