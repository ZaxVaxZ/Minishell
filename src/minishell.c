/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 17:43:37 by ehammoud          #+#    #+#             */
/*   Updated: 2024/07/08 22:52:39 by pipolint         ###   ########.fr       */
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
int		handle_cmd_line(char *cmd_line, t_env **envp, t_msh *m);

char	*return_cwd(char *old_cwd)
{
	char	*final;
	char	*cwd;
	int		i;

	if (old_cwd)
		free(old_cwd);
	cwd = getcwd(NULL, 0);
	if (cwd == NULL)
	{
		write(2, "Path too large\n", 15);
		return (NULL);
	}
	final = malloc(sizeof(char) * (ft_strlen(cwd) + 3));
	if (!final)
		return (NULL);
	i = -1;
	while (cwd[++i])
		final[i] = cwd[i];
	final[i++] = '>';
	final[i++] = ' ';
	final[i] = 0;
	free(cwd);
	return (final);
}

char	*get_line(char *cwd)
{
	char	*new;
	char	*cmd;

	cmd = readline(cwd);
	new = NULL;
	if (!cmd)
		return (NULL);
	new = ft_strjoin_chr(cmd, '\n', "");
	if (!new)
		return (NULL);
	if (cmd)
		free(cmd);
	return (new);
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
	t_msh		m;
	int			cmd_ret;
	char		*zero;

	g_signum = -1;
	m.cwd = return_cwd(NULL);
	m.env = to_env_list(env);
	zero = ft_strdup("0");
	add_var(&m.env, "?", zero);
	free(zero);
	m.interrupt = signal(SIGINT, sig_handle);
	m.q = signal(SIGQUIT, SIG_IGN);
	if (!shllvlhandle(&m.env))
	{
		free_env(&m.env);
		if (m.cwd)
			free(m.cwd);
		return (1);
	}
	m.status = 0;
	while (True)
	{
		write(1, TEXT_RESET, ft_strlen(TEXT_RESET));
		if (g_signum != SIGINT)
			m.line = get_line(m.cwd);
		if (set_sig(&m.env) == False)
			break ;
		if (!m.line)
			break ;
		cmd_ret = handle_cmd_line(m.line, &m.env, &m);
		if (cmd_ret == -2 || cmd_ret == -5)
			break ;
		m.cwd = return_cwd(m.cwd);
	}
	if (m.cwd)
		free(m.cwd);
	free_env(&m.env);
	clear_history();
	exit(m.status);
	(void)ac;
	(void)av;
}
