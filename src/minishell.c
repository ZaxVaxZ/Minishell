/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 17:43:37 by ehammoud          #+#    #+#             */
/*   Updated: 2024/07/20 15:35:55 by pipolint         ###   ########.fr       */
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

char	*get_line(t_main *m)
{
	char	*new;
	char	*cmd;

	if (write(1, TEXT_RESET, ft_strlen(TEXT_RESET)) == -1)
		free_and_exit(m, ERR_WRT);
	cmd = readline("JesterShell # ");
	new = NULL;
	if (!cmd)
		return (NULL);
	new = ft_strjoin_chr(cmd, '\n', "");
	if (!new)
		free_and_exit(m, ERR_WRT);
	if (cmd)
		free(cmd);
	return (new);
}

void	env_handle(t_main *m)
{
	char	*tmp;

	if (!get_var(m->env, "SHLVL"))
		tmp = ft_itoa(1);
	else
		tmp = ft_itoa(ft_atoi(get_var(m->env, "SHLVL")) + 1);
	if (!tmp || !set_var(&(m->env), "SHLVL", tmp, True))
		free_and_exit(m, ERR_MEM);
	free(tmp);
	tmp = ft_strdup("0");
	if (!tmp || !add_var(&(m->env), "?", tmp))
		free_and_exit(m, ERR_MEM);
	free(tmp);
	tmp = ft_strdup("JesterShell");
	if (!tmp || !add_var(&(m->env), "0", tmp)
		|| !set_var(&(m->env), "SHELL", tmp, True))
		free_and_exit(m, ERR_MEM);
	free(tmp);
	tmp = getcwd(NULL, 0);
	if (!tmp)
		free_and_exit(m, ERR_CWD);
	if (!set_var(&(m->env), "PWD", tmp, True))
		free_and_exit(m, ERR_MEM);
	free(tmp);
}

void	init_main_struct(t_main *m, char **env)
{
	signal(SIGINT, sig_handle);
	signal(SIGQUIT, SIG_IGN);
	m->cmds = NULL;
	m->line = NULL;
	m->cwd = NULL;
	m->q = NULL;
	m->status = SUCCESS;
	m->env = to_env_list(env);
	env_handle(m);
}

int	main(int ac, char **av, char **env)
{
	t_main		m;
	int			cmd_ret;

	g_signum = -1;
	init_main_struct(&m, env);
	while (True)
	{
		if (g_signum != SIGINT && isatty(0))
			m.line = get_line(&m);
		if (set_sig(&m) == False)
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
