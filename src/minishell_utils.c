/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 11:44:13 by pipolint          #+#    #+#             */
/*   Updated: 2024/07/24 17:15:11 by pipolint         ###   ########.fr       */
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

int	add_to_history(t_main *m)
{
	char	*trimmed_str;

	if (m->q)
	{
		trimmed_str = ft_strtrim(m->line, "\n");
		if (!trimmed_str)
			return (-1);
		add_history(trimmed_str);
		free(trimmed_str);
	}
	return (1);
}

int	syntax_error_cleanup(t_main *m, int syntax_error)
{
	char	*tmp;

	if (syntax_error)
	{
		tmp = ft_itoa(258);
		free(m->line);
		if (!tmp)
			return (-1);
		set_var(&m->env, "?", tmp, False);
		free(tmp);
		return (0);
	}
	return (1);
}

int	execute_exit(t_main *m, int ret)
{
	if (!m->status && ft_strncmp(get_var(m->env, "?"), "0", -1))
	{
		free_up_cmd_mem(m);
		exit(ft_atoi(get_var(m->env, "?")));
	}
	free_up_cmd_mem(m);
	return (ret);
}

void	set_status(t_main *m, int *ret)
{
	char	*stat;

	stat = ft_itoa(m->status);
	if (stat)
	{
		if (!set_var(&m->env, "?", stat, False))
			*ret = -2;
		free(stat);
	}
	else
		*ret = -2;
}

int	handle_cmd_line(t_main *m)
{
	int		ret;
	int		p_cleanup;

	if (!m->line || !*m->line || m->line[0] == NL)
		return (free_up_cmd_mem(m));
	m->q = parse(m->line);
	if (add_to_history(m) == -1)
		return (-1);
	p_cleanup = parse_clean_up(m, &m->q);
	if (p_cleanup == -2)
		return (0);
	if (syntax_error_cleanup(m, p_cleanup == 1) <= 0)
		return (1);
	clean_whitespace(m);
	if (!build_commands(m))
		return (1);
	ret = execute_commands(m);
	if (ret == -5)
		return (execute_exit(m, ret));
	set_status(m, &ret);
	free_up_cmd_mem(m);
	return (ret);
}
