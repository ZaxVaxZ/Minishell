/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 17:43:37 by ehammoud          #+#    #+#             */
/*   Updated: 2024/05/01 20:54:26 by pipolint         ###   ########.fr       */
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
//#include <signal.h>
#include "signals.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./../tmp/tmp_utils.h"
#include <readline/readline.h>
#include <readline/history.h>

//int	g_signum;

//void	sig_handle(int signal)
//{
//	char	*tmp;
//	char	*line;

//	g_signum = signal;
//	if (signal == SIGINT)
//	{
//		tmp = return_cwd(NULL);
//		if (isatty(0))
//		{
//			//rl_replace_line("", 0);
//			rl_redisplay();
//			rl_on_new_line();
//		}
//		free(tmp);
//	}
//}

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

static int	handle_cmd_line(char *cmd_line, t_env *envp, t_msh *m)
{
	int		ret;
	char	*tmp;
	t_queue	*q;
	t_cmd	*cmds;
	char	*trimmed;

	if (!cmd_line || !*cmd_line || cmd_line[0] == NL)
		return (free_up(cmd_line, NULL, NULL));
	cmds = NULL;
	q = parse(cmd_line);
	if (q)
	{
		trimmed = ft_strtrim(cmd_line, "\n");
		if (!trimmed)
			return (-1);
		add_history(trimmed);
		free(trimmed);
	}
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
	ret = execute_commands(&envp, cmds, &m->status);
	if (ret == -5)
		exit(m->status);
	tmp = ft_itoa(m->status);
	if (tmp)
	{
		if (!set_var(&envp, "?", tmp, False))
			ret = -1;
		free(tmp);
	}
	else
		ret = -1;
	free_up(cmd_line, &q, &cmds);
	return (ret);
}

char	*return_cwd(char *old_cwd)
{
	char	*final;
	char	*cwd;
	int		i;

	if (old_cwd)
		free(old_cwd);
	cwd = getcwd(NULL, 0);
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
	if (!cmd)
		return (NULL);
	new = ft_strjoin_chr(cmd, '\n', "");
	if (!new)
		return (NULL);
	if (cmd)
		free(cmd);
	return (new);
}

//int	main(int ac, char **av, char **env)
//{
//	char		*line;
//	char		*cwd;
//	char		*tmp;
//	t_env		*enviro;
//	int			status;

//	cwd = return_cwd(NULL);
//	enviro = to_env_list(env);
//	signal(SIGINT, sig_handle);
//	signal(SIGQUIT, sig_handle);
//	g_signum = -1;
//	status = 0;
//	while (True)
//	{
//		line = get_line(cwd);
//		if (!line || handle_cmd_line(line, enviro, &status) == -1)
//			break ;
//		cwd = return_cwd(cwd);
//		if (set_sig(&enviro, &tmp) == False)
//			break ;
//	}
//	if (line)
//		free(line);
//	if (cwd)
//		free(cwd);
//	free_env(&enviro);
//	clear_history();
//}

int	main(int ac, char **av, char **env)
{
	t_msh		m;
	//t_env		*enviro;

	m.cwd = return_cwd(NULL);
	write(1, "test1", 5);
	m.env = to_env_list(env);
	write(1, "test2", 5);
	m.interrupt = signal(SIGINT, sig_handle);
	m.q = signal(SIGQUIT, sig_handle);
	g_signum = -1;
	m.status = 0;
	while (True)
	{
		m.line = get_line(m.cwd);
		if (!m.line || handle_cmd_line(m.line, m.env, &m) == -1)
			break ;
		m.cwd = return_cwd(m.cwd);
		if (set_sig(&m.env) == False)
			break ;
	}
	if (m.line)
		free(m.line);
	if (m.cwd)
		free(m.cwd);
	free_env(&m.env);
	clear_history();
}
