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
#include <stdio.h>
#include <unistd.h>

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
	printf(">> The Queue:\n");
	if (!queue)
	{
		printf("The queue is empty!\n");
		return ;
	}
	printf("|%10.10s|%10.10s|", "String", "Type");
	while (queue)
		printf("|%10.10s|%10.10s|\n", queue->s, type_to_str(queue->type));
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