/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 17:43:37 by ehammoud          #+#    #+#             */
/*   Updated: 2024/04/02 17:21:47 by pipolint         ###   ########.fr       */
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

static int	free_up(char *cmd_line, t_queue **q, t_cmd **cmds)
{
	if (cmd_line)
		free(cmd_line);
	if (q)
		free_queue(q);
	if (cmds)
		free_cmd(cmds);
	return (0);
}

static int	handle_cmd_line(char *cmd_line, t_env *envp)
{
	t_queue	*q;
	t_cmd	*cmds;

	if (!cmd_line || !*cmd_line || cmd_line[0] == NL)
		return (free_up(cmd_line, NULL, NULL));
	cmds = NULL;
	q = parse(cmd_line);
	if (parse_clean_up(&q, envp) == -2)
		return (0);
	if (parse_clean_up(&q, envp))
	{
		free_env(&envp);
		free(cmd_line);
		return (1);
	}
	clean_whitespace(q);
	if (!build_commands(&q, &cmds, &envp))
		return (1);
	print_commands(cmds);
	return (free_up(cmd_line, &q, &cmds));
}

int	main(int ac, char **av, char **env)
{
	char		*tmp;
	char		*cmd_line;
	t_env		*envp;

	(void)ac;
	(void)av;
	tmp = getcwd(NULL, 0);
	if (isatty(0))
		ft_printf("%s> ", tmp);
	cmd_line = get_next_line(0);
	envp = to_env_list(env);
	add_var(&envp, "PWD", tmp);
	free(tmp);
	while (cmd_line)
	{
		if (handle_cmd_line(cmd_line, envp))
			exit(1);
		tmp = getcwd(NULL, 0);
		if (isatty(0))
			ft_printf("%s> ", tmp);
		free(tmp);
		cmd_line = get_next_line(0);
	}
	free_env(&envp);
	return (0);
}
