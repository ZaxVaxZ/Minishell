/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tester_minishell.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 17:43:37 by ehammoud          #+#    #+#             */
/*   Updated: 2024/04/05 20:28:38 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include "parser.h"
#include "queues.h"
#include "builtins.h"
#include "executor.h"
#include "cmd_list.h"
#include "env_list.h"
#include "ft_printf.h"
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./../tmp/tmp_utils.h"

static void	handle(int sig)
{
	write(1, "\n", 1);
	if (sig == SIGINT)
		exit (0);
}

static int	handle_cmd_line(char *cmd_line, t_env *envp)
{
	t_queue	*q;
	t_cmd	*cmds;

	if (!cmd_line || !*cmd_line)
	{
		if (cmd_line)
			free(cmd_line);
		return (0);
	}
	cmds = NULL;
	q = parse(cmd_line);
	if (parse_clean_up(&q, envp))
	{
		free(cmd_line);
		return (1);
	}
	// print_queue(q);
	clean_whitespace(q);
	ft_printf("\n%s", cmd_line);
	if (!build_commands(&q, &cmds, &envp))
		return (1);
	// execute_commands(&envp, &cmds);
	// print_queue(q);
	print_commands(cmds);
	ft_printf("\n");
	free_queue(&q);
	free_cmd(&cmds);
	free(cmd_line);
	return (0);
}

int	main(int ac, char **av, char **env)
{
	char		*cmd_line;
	t_env		*envp;

	(void)ac;
	(void)av;
	signal(SIGINT, handle);
	if (isatty(0))
		ft_printf("%s> ", getenv("PWD"));
	cmd_line = get_next_line(0);
	envp = to_env_list(env);
	while (cmd_line)
	{
		ft_printf("%s", cmd_line);
		if (cmd_line[0] != '#' && handle_cmd_line(cmd_line, envp))
			exit(1);
		else if (cmd_line[0] == '#')
			free(cmd_line);
		if (isatty(0))
			ft_printf("%s> ", getenv("PWD"));
		cmd_line = get_next_line(0);
	}
	free_env(&envp);
	return (0);
}
