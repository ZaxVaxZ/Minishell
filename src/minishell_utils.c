/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 11:44:13 by pipolint          #+#    #+#             */
/*   Updated: 2024/07/12 12:13:28 by pipolint         ###   ########.fr       */
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
#include "signals.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./../tmp/tmp_utils.h"
#include <readline/readline.h>
#include <readline/history.h>

int	free_up(char *cmd_line, t_queue **q, t_cmd **cmds)
{\
	cmd_line = NULL;
	if (q)
		free_queue(q);
	if (cmds)
		free_cmd(cmds);
	return (0);
}

int	add_to_history(char *cmd_line, t_queue *q)
{
	char	*trimmed_str;

	if (q)
	{
		trimmed_str = ft_strtrim(cmd_line, "\n");
		if (!trimmed_str)
			return (-1);
		add_history(trimmed_str);
		free(trimmed_str);
	}
	return (1);
}

int	syntax_error_cleanup(t_env **envp, char *cmd_line, int syntax_error)
{
	char	*tmp;

	if (syntax_error)
	{
		tmp = ft_itoa(258);
		free(cmd_line);
		if (!tmp)
			return (-1);
		set_var(envp, "?", tmp, False);
		free(tmp);
		return (0);
	}
	return (1);
}

int	handle_cmd_line(char *cmd_line, t_env **envp, t_msh *m)
{
	int		ret;
	char	*tmp;
	t_queue	*q;
	t_cmd	*cmds;
	int		p_cleanup;

	if (!cmd_line || !*cmd_line || cmd_line[0] == NL)
		return (free_up(cmd_line, NULL, NULL));
	cmds = NULL;
	q = parse(cmd_line);
	if (add_to_history(cmd_line, q) == -1)
		return (-1);
	p_cleanup = parse_clean_up(&q, *envp);
	if (p_cleanup == -2)
		return (0);
	if (syntax_error_cleanup(envp, cmd_line, p_cleanup == 1) <= 0)
		return (1);
	clean_whitespace(q);
	//print_queue(q);
	if (!build_commands(&q, &cmds, envp))
		return (1);
	//for (t_cmd *t = cmds; t; t = t->next)
	//{
	//	const char *strs[] = {[AND_OP]= "and",
	//					[OR_OP]= "or",
	//					[PIPE_OP] = "pipe",
	//					[SEMICOLON] = "semicolon"};
	//	if (t->params)
	//		printf("command: %s Before: %s After: %s\n", t->params[0], strs[t->before], strs[t->after]);
	//	else
	//	{
	//		if (t->rep == LP)
	//			printf("LP\n");
	//		else if (t->rep == RP)
	//			printf("RP\n");
	//	}
	//}
	ret = execute_commands(envp, cmds, &m->status);
	if (ret == -5)
	{
		if (!m->status && ft_strncmp(get_var(*envp, "?"), "0", -1))
			exit(ft_atoi(get_var(*envp, "?")));
		free_up(cmd_line, &q, &cmds);
		return (ret);
	}
	tmp = ft_itoa(m->status);
	if (tmp)
	{
		if (!set_var(envp, "?", tmp, False))
			ret = -2;
		free(tmp);
	}
	else
		ret = -2;
	free_up(cmd_line, &q, &cmds);
	return (ret);
}
