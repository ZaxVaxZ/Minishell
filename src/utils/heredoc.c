/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 15:54:35 by pipolint          #+#    #+#             */
/*   Updated: 2024/04/26 20:29:29 by pipolint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "get_next_line.h"
#include "general.h"
#include "parser.h"

//void	expand_variable(char *line, t_env **env)
//{
//	int		i;
//	int		j;
//	int		k;
//	char	*var;

//	i = -1;
//	var = NULL;
//	while (line[++i])
//	{
//		j = i;
//		if (line[i] == '$')
//		{
//			j = i;
//			while (is_valid_var_char(line[++j]));
//			var = malloc(sizeof(char) * (j + 1));
//			var[j + 1] = '\0';
//			j = i;
//			k = -1;
//			while (is_valid_var_char(line[++j]))
//				var[++k] = line[j];
//		}
//		if (var)
//			fprintf(stderr, "Var: %s\n", var);
//		i += j;
//	}
//}

void	handle(int sig)
{
	(void)sig;
}

void	heredoc_child(t_cmd *cmd, t_exec *exec, int *fds, int i)
{
	char	*line;

	close(fds[0]);
	signal(SIGQUIT, handle);
	signal(SIGINT, handle);
	while (1)
	{
		write(1, "> ", 2);
		line = get_next_line(STDIN_FILENO);
		if (!ft_strncmp(line, cmd->infiles[i], ft_strlen(line) - 1)
			&& ft_strlen(line) - 1 == ft_strlen(cmd->infiles[i]))
			break ;
		expand_variable(line, NULL);
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
	signal(SIGQUIT, handle);
	signal(SIGINT, handle);
	exit(0);
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

t_bool	heredoc_loop(t_cmd *cmd, t_exec *exec)
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
				heredoc(cmd, exec, heredoc_fds, i);
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

t_bool	heredoc(t_cmd *cmd, t_exec *exec, int *fds, int i)
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
		heredoc_child(cmd, exec, fds, i);
	if (heredoc_parent(&cmd, fds, exec) == False)
		return (False);
	return (True);
}
