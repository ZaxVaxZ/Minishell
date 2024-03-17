/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 17:43:37 by ehammoud          #+#    #+#             */
/*   Updated: 2024/03/15 17:04:30 by pipolint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include "parser.h"
#include "queues.h"
#include "builtins.h"
#include "executor.h"
#include "env_list.h"
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void	prints(char *s, int lim)
{
	int i=0;
	if (!s)
		write(1, "NULL", 4 + (0 * (i += 4)));
	while (s && s[i] && i < lim)
	{
		if (i == lim - 1 && (*s == ' ' || *s == '\t' || *s == '\n'))
			write(1, "\\", 1 + (0 * i++));
		else if (*s == '\t')
			write(1, "\\t", 2 + (0 * (i+++--lim)));
		else if (*s == '\n')
			write(1, "\\n", 2 + (0 * (i+++--lim)));
		else
			write(1, s + i++, 1);
	}
	while (i++ < lim)
		write(1, " ", 1);
}

char	*type_to_str(t_token type)
{
	switch(type)
	{
		case Word: return "Word";
		case Name: return "Name";
		case Assign: return "Assign";
		case Op_pipe: return "Op_pipe";
		case Op_redir: return "Op_redir";
		case Op_logic: return "Op_logic";
		case Variable: return "Variable";
		case Semicolon: return "Semicolon";
		case Sq_open: return "Sq_open";
		case Sq_closed: return "Sq_closed";
		case Dq_open: return "Dq_open";
		case Dq_closed: return "Dq_closed";
		case Bracket_open: return "Bracket_open";
		case Bracket_closed: return "Bracket_closed";
		case Whitespace: return "Whitespace";
		case Illegal: return "Illegal";
	}
	return "Unrecognized";
}

void	print_queue(t_queue *queue)
{
	write(1, "\n>> The Queue:\n", 15);
	if (!queue)
	{
		write(1, "The queue is empty!\n\n", 21);
		return ;
	}
	write(1, "|----------|----------|\n", 24);
	write(1, "|", 1);
	prints("  String", 10);
	write(1, "|", 1);
	prints("   Type", 10);
	write(1, "|\n", 2);
	write(1, "|----------|----------|\n", 24);
	while (queue)
	{
		write(1, "|", 1);
		prints(queue->s, 10);
		write(1, "|", 1);
		prints(type_to_str(queue->type), 10);
		write(1, "|\n", 2);
		queue = queue->next;
	}
	write(1, "|----------|----------|\n\n", 25);
}

void	handle(int sig)
{
	write(1, "\n", 1);
	if (sig == SIGINT)
		exit (0);
}

// void	execute(t_queue *q)
// {
// 	t_blt	b;
//
// 	b.pwd = getenv("PWD");
// 	while (q)
// 	{
// 		if (!ft_strncmp(q->s, "echo", 4))
// 			echo((q->next && q->next->next ? q->next->next->s : "No string"), !ft_strncmp(q->next->s, "-n", -1));
// 		else if (!ft_strncmp(q->s, "cd", 2))
// 			cd(&b, q->next && q->next->next ? q->next->next->s : NULL);
// 		//else if (!ft_strncmp(q->s, "pwd", 3))
// 			//working_dir(&b);
// 		else if (!ft_strncmp(q->s, "exit", 4))
// 			exiting(1);
// 		else if (!ft_strncmp(q->s, "export", 6))
// 			putenv(q->next && q->next->next ? q->next->next->s : "");
// 		q = q->next;
// 	}
// }

int	main(int ac, char **av, char **env)
{
	(void)ac; (void)av; (void)env;
	char	*cmd_line;
	t_queue	*q;

	(void)q; (void)cmd_line;
	signal(SIGINT, handle);
	printf("%s> ", getenv("PWD"));
	cmd_line = get_next_line(0);
	t_env *envp;
	envp = to_env_list(env);
	print_env(envp);
	// print_list(envp);
	t_command cmds[1024];
	int i = 0;
	while (cmd_line)
	{
		q = parse(cmd_line);
		// print_queue(q);
		if (parse_clean_up(&q))
		{
			free(cmd_line);
			return (1);
		}
		clean_whitespace(q);
		// print_queue(q);
		execute(q, &cmds[i++]);
		// for (int j = 0; cmds[i - 1].params[j]; j++)
		// 	printf("%s\n", cmds[i - 1].params[j]);
		resolve_builtin(&cmds[i - 1], &envp);
		free_env(&envp);
		free_queue(&q);
		free(cmd_line);
		return (0);
		printf("%s> ", getenv("PWD"));
		cmd_line = get_next_line(0);
	}
	return (0);
}
