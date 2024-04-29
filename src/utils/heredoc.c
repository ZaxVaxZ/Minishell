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

char	*expand_variable(char *line, t_env **env, t_queue **q)
{
	t_queue	*node;
	char	**words;
	char	*trimmed;
	int		i;
	int		j;
	int		k;
	char	*var;
	int		newline;

	i = -1;
	newline = 0;
	words = ft_split(line, ' ');
	var = NULL;
	trimmed = NULL;
	while (words[++i])
	{
		j = -1;
		while (words[i][++j])
		{
			newline = words[i][j] == '\n';
			k = j;
			if (words[i][j] == '$')
			{
				j++;
				if (words[i][j] == '$')
				{
					var = ft_strdup(get_var(*env, "$"));
					node = new_node(var);
					queue(q, node);
				}
				else
				{
					k = j;
					while (is_valid_var_char(words[i][j]))
					j++;
					if (words[i][j] == '\n')
					newline = 1;
					trimmed = malloc(sizeof(char) * ((j - k) + 1));
					j = k;
					k = 0;
					while (is_valid_var_char(words[i][j]))
						trimmed[k++] = words[i][j++];
					trimmed[k] = '\0';
					var = ft_strdup(get_var(*env, trimmed));
					if (var)
					{
						node = new_node(var);
						queue(q, node);
					}
				}
				if (words[i][j] == '$')
					j--;
			}
			else if (words[i][j] && words[i][j] != '\n')
			{
				while (words[i][j] && words[i][j] != '$')
					j++;
				trimmed = malloc(sizeof(char) * ((j - k) + 1));
				j = k;
				k = 0;
				while (words[i][j] && words[i][j] != '$')
				{
					if (words[i][j] == '\n')
						newline = 1;
					trimmed[k++] = words[i][j++];
				}
				trimmed[k] = '\0';
				node = new_node(trimmed);
				queue(q, node);
				if (words[i][j] == '$')
					j--;
			}
			if (newline)
			{
				node = new_node("\n");
				queue(q, node);
			}
			if (words[i + 1])
			{
				node = new_node(" ");
				queue(q, node);
			}
			if (trimmed)
				free(trimmed);
			if (words[i][j] == '\0')
				break;
		}
	}
	ft_freeup(words);
	char *ret = malloc(sizeof(char));
	ret[0] = '\0';
	for (t_queue *t = *q; t; t = t->next)
	{
		ret = ft_strjoin(ret, t->s);
	}
	// fprintf(stderr, "Final %s\n", ret);
}

void	handle(int sig)
{
	(void)sig;
}

void	heredoc_child(t_cmd *cmd, t_exec *exec, int *fds, int i, t_env **env)
{
	t_queue	*q;
	char	*line;
	char	*ret;

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
		ret = expand_variable(line, env, &q);
		// if (write(fds[1], ret, ft_strlen(ret)) == -1)
		// {
		// 	exec->last_status = EXIT_FAILURE;
		// 	exit(EXIT_FAILURE);
		// }
		free(ret);
		free(line);
	}
	for (t_queue *t = q; t; t = t->next)
	{
		if (write(fds[1], t->s, ft_strlen(t->s)) == -1)
		{
			exec->last_status = EXIT_FAILURE;
			exit(EXIT_FAILURE);
		}
	}
	free_queue(&q);
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
