/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehammoud <ehammoud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 17:43:37 by ehammoud          #+#    #+#             */
/*   Updated: 2024/03/01 13:36:15 by ehammoud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include "parser.h"
#include "queues.h"
#include <signal.h>
#include <unistd.h>

void	prints(char *s, int lim)
{
	int i=0;
	while (s[i] && i < lim)
		write(1, s + i++, 1);
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
		case Bracket_open: return "Bracket_open";
		case Bracket_closed: return "Bracket_closed";
		case Illegal: return "Illegal";
	}
	return "Unrecognized";
}

void	print_queue(t_queue *queue)
{
	write(1, "\n>> The Queue:\n", 14);
	if (!queue)
	{
		write(1, "The queue is empty!\n\n", 20);
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
	write(1, "|----------|----------|\n\n", 24);
}

void	handle(int sig)
{
	if (sig == SIGINT)
		exit (0);
}

int	main(int ac, char **av, char **env)
{
	char	*cmd_line;
	t_queue	*q;

	(void)ac; (void)av; (void)env;
	signal(SIGINT, handle);
	write(1, "C:/User/ehammoud/> ", 19);
	cmd_line = get_next_line(0);
	while (cmd_line)
	{
		q = parse(cmd_line);
		print_queue(q);
		free_queue(&q);
		free(cmd_line);
		write(1, "C:/User/ehammoud/> ", 19);
		cmd_line = get_next_line(0);
	}
	return (0);
}