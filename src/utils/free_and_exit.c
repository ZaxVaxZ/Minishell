/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_and_exit.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/14 18:14:42 by ehammoud          #+#    #+#             */
/*   Updated: 2024/07/25 16:29:16 by pipolint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "general.h"
#include "ft_printf.h"
#include "queues.h"
#include "env_list.h"
#include "cmd_list.h"
#include "signals.h"

void	heredoc_exit(t_heredoc *h)
{
	free_and_return(&h->m->q, &h->m->env, &h->m->cmds, NULL);
	free(h->m->line);
	close(h->fds[WRITEEND]);
	close(h->exec->std_in);
	close(h->exec->std_out);
	exit(252);
}

void	handle_msg(int msg)
{
	int	ret;

	ret = 0;
	if (msg == -1)
		return ;
	else if (msg == SUCCESS)
		ret = ft_printf("exit\n");
	else if (msg == FAILURE)
		ret = write(2, "Unexpected Error.\n", 18);
	else if (msg == ERR_MEM)
		ret = write(2, "Unexpected Error: Failed to allocate memory.\n", 45);
	else if (msg == ERR_DUP)
		ret = write(2, "Unexpected Error: Failed to dup an fd.\n", 39);
	else if (msg == ERR_CLS)
		ret = write(2, "Unexpected Error: Failed to close a file.\n", 42);
	else if (msg == ERR_FRK)
		ret = write(2, "Unexpected Error: Failed to fork.\n", 34);
	else if (msg == ERR_PIP)
		ret = write(2, "Unexpected Error: Failed to pipe.\n", 34);
	else if (msg == ERR_CWD)
		ret = write(2, "Unexpected Error: Couldn't get CWD.\n", 36);
	if (msg == ERR_WRT || ret == -1)
		ret = write(2, "Unexpected Error: Failed to write.\n", 35);
}

int	free_up_cmd_mem(t_main *m)
{
	if (m->line)
	{
		free(m->line);
		m->line = NULL;
	}
	if (m->q)
		free_queue(&(m->q));
	if (m->cmds)
		free_cmd(&(m->cmds));
	return (0);
}

void	free_and_exit(t_main *m, int msg)
{
	handle_msg(msg);
	if (m->line)
		free(m->line);
	if (m->q)
		free_queue(&(m->q));
	if (m->env)
		free_env(&(m->env));
	if (m->cmds)
		free_cmd(&(m->cmds));
	exit(m->status);
}

void	child_free_and_exit(t_env **env, t_exec *exec, int status)
{
	free_env(env);
	free_cmd(exec->cmd_head);
	exit(status);
}
