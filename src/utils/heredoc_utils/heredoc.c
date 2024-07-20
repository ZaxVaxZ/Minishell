/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 15:54:35 by pipolint          #+#    #+#             */
/*   Updated: 2024/07/20 16:50:43 by pipolint         ###   ########.fr       */
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

void	heredoc_sigquit(int sig)
{
	(void)sig;
	write(1, "\b\b  \b\b", 6);
}

void	heredoc_sigint(int sig)
{
	g_signum = SIGINT;
	write(1, "\n", 1);
	close(STDIN_FILENO);
}

void	heredoc_child(t_heredoc *h)
{
	int			i;
	int			j;
	char		*line;
	char		*var;
	char		*tmp;

	signal(SIGINT, heredoc_sigint);
	signal(SIGQUIT, heredoc_sigquit);
	close(h->fds[READEND]);
	g_signum = -1;
	if (h->cmd->before == PIPE_OP)
	{
		if (dup_and_check(h->exec->std_in, STDIN_FILENO, h->exec) == -1)
			return ;
		if (close_and_check(h->exec->std_in, h->exec) == -1)
			return ;
	}
	while (1)
	{
		line = readline("> ");
		if (g_signum == SIGINT)
			heredoc_exit(h);
		tmp = line;
		line = ft_strjoin_chr(line, '\n', "");
		free(tmp);
		if (!line || (!ft_strncmp(line, h->cmd->infiles[h->i], ft_strlen(line) - (line[ft_strlen(line) - 1] == '\n'))
			&& ft_strlen(h->cmd->infiles[h->i]) == ft_strlen(line) - (line[ft_strlen(line) - 1] == '\n')))
			break ;
		i = 0;
		while (line[i])
		{
			if (line[i] != DS)
				write(h->fds[WRITEEND], line + i, 1);
			else
			{
				if (!is_valid_var_char(line[i + 1]))
				{
					write(h->fds[WRITEEND], line + i++, 1);
					continue ;
				}
				else if (found_in(line[i + 1], DIGIT))
				{
					i += 2;
					continue ;
				}
				j = ++i;
				while (is_valid_var_char(line[j]))
					j++;
				var = ft_substr(line, i, j - i);
				if (var && get_var(*(h->env), var))
					write(h->fds[WRITEEND], get_var(*(h->env), var), ft_strlen(get_var(*(h->env), var)));
				if (var)
					free(var);
				i = j - 1;
			}
			i++;
		}
		free(line);
	}
	close(h->fds[WRITEEND]);
	if (line)
		free(line);
	close(h->exec->std_in);
	close(h->exec->std_out);
	if (h->cmd->after == PIPE_OP || h->cmd->before == PIPE_OP)
	{
		close(h->exec->fds[READEND]);
		close(h->exec->fds[WRITEEND]);
	}
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

	signal(SIGINT, nothing);
	signal(SIGQUIT, nothing);
	waitpid(exec->last_pid, &ex, 0);
	signal(SIGINT, sig_handle);
	signal(SIGQUIT, SIG_IGN);
	if (WEXITSTATUS(ex) == 252)
		return (False);
	if (WIFSIGNALED(ex) && WTERMSIG(ex) == SIGINT)
	{
		exec->last_status = 1;
		(*cmd)->status = exec->last_status;
		write(1, "\n", 1);
		tmp = ft_itoa(1);
		if (!tmp)
			return (False);
		set_var(&m->env, "?", tmp, False);
		free(tmp);
		if (!(*cmd)->next || ((*cmd)->next && !(*cmd)->next->heredoc))
		{
			close(fds[WRITEEND]);
			close(fds[READEND]);
		}
		return (False);
	}
	close(fds[WRITEEND]);
	(*cmd)->in_fd = dup(fds[READEND]);
	if ((*cmd)->in_fd == -1)
		return (False);
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
	return (heredoc_parent(h->m, &h->cmd, h->fds, h->exec));
}
