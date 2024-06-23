/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehammoud <ehammoud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 15:54:35 by pipolint          #+#    #+#             */
/*   Updated: 2024/06/03 17:07:19 by ehammoud         ###   ########.fr       */
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

void	uwu(int sig)
{
	write(1, "\b\b  \b\b", 6);
}
// void	uwu2(int sig)
// {
// 	write(1, "a\b\bb", 4);
// }

void	heredoc_child(t_heredoc *h)
{
	int			i;
	int			j;
	char		*line;
	char		*var;

	signal(SIGINT, sig_heredoc);
	signal(SIGQUIT, uwu);
	close(h->fds[0]);
	g_signum = -1;
	while (1)
	{
		write(1, "> ", 2);
		line = get_next_line(STDIN_FILENO);
		if (!line || !ft_strncmp(line, h->cmd->infiles[h->i], ft_strlen(line) - (line[ft_strlen(line) - 1] == '\n'))
			&& ft_strlen(h->cmd->infiles[h->i]) == ft_strlen(line) - (line[ft_strlen(line) - 1] == '\n'))
			break ;
		i = 0;
		while (line[i])
		{
			if (line[i] != DS)
				write(h->fds[1], line + i, 1);
			else
			{
				if (!is_valid_var_char(line[i + 1]))
				{
					write(h->fds[1], line + i++, 1);
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
					write(h->fds[1], get_var(*(h->env), var), ft_strlen(get_var(*(h->env), var)));
				if (var)
					free(var);
				i = j - 1;
			}
			i++;
		}
		free(line);
	}
	close(h->fds[1]);
	if (line)
		free(line);
	exit(h->exec->last_status);
}

t_bool	heredoc_parent(t_cmd **cmd, int *fds, t_exec *exec)
{
	int	exit;

	// signal(SIGQUIT, uwu);
	// signal(SIGINT, sig_handle);
	waitpid(exec->last_pid, &exec->last_status, 0);
	exit = WEXITSTATUS(exec->last_status);
	close(fds[1]);
	(*cmd)->in_fd = dup(fds[0]);
	if ((*cmd)->in_fd == -1)
		return (False);
	close(fds[0]);
	if (exit == EXIT_FAILURE)
		return (False);
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
