/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehammoud <ehammoud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 17:43:37 by ehammoud          #+#    #+#             */
/*   Updated: 2024/07/16 13:47:59 by ehammoud         ###   ########.fr       */
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

extern int	g_signum;

int		free_up(char *cmd_line, t_queue **q, t_cmd **cmds);
int		handle_cmd_line(t_env **envp, t_main *m);

char	*get_line(void)
{
	char	*new;
	char	*cmd;
	char	*buf;

	cmd = readline("minishell > ");
	new = NULL;
	if (!cmd)
		return (NULL);
	new = ft_strjoin_chr(cmd, '\n', "");
	if (!new)
		return (NULL);
	if (cmd)
		free(cmd);
	return (new);
	return (NULL);
}

t_bool	shllvlhandle(t_env **env)
{
	char	*tmp;

	if (!get_var(*env, "SHLVL"))
		add_var(env, "SHLVL", ft_itoa(1));
	else
	{
		tmp = ft_itoa(ft_atoi(get_var(*env, "SHLVL")) + 1);
		if (!tmp)
			return (False);
		set_var(env, "SHLVL", tmp, True);
		free(tmp);
	}
	return (True);
}

int	main(int ac, char **av, char **env)
{
	t_main		m;
	int			cmd_ret;
	char		*zero;

	g_signum = -1;
	m.env = to_env_list(env);
	zero = ft_strdup("0");
	add_var(&m.env, "?", zero);
	free(zero);
	signal(SIGINT, sig_handle);
	signal(SIGQUIT, SIG_IGN);
	if (!shllvlhandle(&m.env))
	{
		free_env(&m.env);
		return (1);
	}
	m.status = 0;
	while (True)
	{
		write(1, TEXT_RESET, ft_strlen(TEXT_RESET));
		if (g_signum != SIGINT && isatty(0))
			m.line = get_line();
		if (set_sig(&m.env) == False)
			break ;
		if (!m.line)
			break ;
		cmd_ret = handle_cmd_line(&m.env, &m);
		if (cmd_ret == -2 || cmd_ret == -5)
			break ;
	}
	free_env(&m.env);
	clear_history();
	exit(m.status);
	(void)ac;
	(void)av;
}
