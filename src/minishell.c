/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehammoud <ehammoud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 17:43:37 by ehammoud          #+#    #+#             */
/*   Updated: 2024/03/26 14:29:08 by ehammoud         ###   ########.fr       */
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

	cmds = NULL;
	q = parse(cmd_line);
	if (parse_clean_up(&q))
	{
		free(cmd_line);
		return (1);
	}
	clean_whitespace(q);
	build_commands(&q, &cmds, &envp);
	// execute_command(&envp, &cmds);
	print_commands(cmds);
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
	ft_printf("%s> ", getenv("PWD"));
	cmd_line = get_next_line(0);
	envp = to_env_list(env);
	while (cmd_line)
	{
		if (handle_cmd_line(cmd_line, envp))
			exit(1);
		ft_printf("%s> ", getenv("PWD"));
		cmd_line = get_next_line(0);
	}
	free_env(&envp);
	return (0);
}
