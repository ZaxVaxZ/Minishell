/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 17:43:37 by ehammoud          #+#    #+#             */
/*   Updated: 2024/07/16 14:53:33 by pipolint         ###   ########.fr       */
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

	cmd = readline("minishell$ ");
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

t_bool	set_default_env(t_main *m, char **env)
{
	char	*tmp;

	signal(SIGINT, sig_handle);
	signal(SIGQUIT, SIG_IGN);
	m->env = to_env_list(env);
	if (shllvlhandle(&m->env) == False)
		return (False);
	tmp = ft_strdup("0");
	if (!tmp)
		return (False);
	add_var(&m->env, "?", tmp);
	free(tmp);
	tmp = ft_strdup("minishell");
	if (!tmp)
		return (False);
	add_var(&m->env, "0", tmp);
	free(tmp);
	return (True);
}

int	main(int ac, char **av, char **env)
{
	t_main		m;
	int			cmd_ret;

	g_signum = -1;
	set_default_env(&m, env);
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
