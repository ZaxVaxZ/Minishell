/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 17:43:37 by ehammoud          #+#    #+#             */
/*   Updated: 2024/04/06 05:58:40 by codespace        ###   ########.fr       */
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

static void	stop_kill(int signal)
{
	char	*tmp;

	if (signal == SIGINT)
	{
		tmp = getcwd(NULL, 0);
		if (isatty(0))
			ft_printf("\n%s> ", tmp);
		free(tmp);
	}
}

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

static int	handle_cmd_line(char *cmd_line, t_env *envp, int *status)
{
	int		ret;
	char	*tmp;
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
	ret = execute_commands(&envp, &cmds, status);
	tmp = ft_itoa(*status);
	if (tmp)
	{
		set_var(&envp, "?", tmp, False);
		free(tmp);
	}
	else
		ret = -1;
	free_up(cmd_line, &q, &cmds);
	// print_commands(cmds);
	return (ret);
}

int	main(int ac, char **av, char **env)
{
	char	*tmp;
	char	*cmd_line;
	t_env	*envp;
	int		status;
	int		ret;

	(void)ac;
	(void)av;
	tmp = getcwd(NULL, 0);
	if (isatty(0))
		ft_printf("%s> ", tmp);
	signal(SIGINT, stop_kill);
	signal(SIGQUIT, stop_kill);
	cmd_line = get_next_line(0);
	envp = to_env_list(env);
	add_var(&envp, "PWD", tmp);
	free(tmp);
	while (cmd_line)
	{
		if (handle_cmd_line(cmd_line, envp, &status) < 0)
			break ;
		tmp = getcwd(NULL, 0);
		if (isatty(0))
			ft_printf("%s> ", tmp);
		free(tmp);
		cmd_line = get_next_line(0);
	}
	free_env(&envp);
	ft_printf("\n");
	exit(status);
}
