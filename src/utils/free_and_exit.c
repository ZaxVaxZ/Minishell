/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_and_exit.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehammoud <ehammoud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/14 18:14:42 by ehammoud          #+#    #+#             */
/*   Updated: 2024/07/16 13:57:31 by ehammoud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "general.h"
#include "ft_printf.h"
#include "queues.h"
#include "env_list.h"
#include "cmd_list.h"

void	handle_msg(int msg)
{
	int	ret;

	if (msg == SUCCESS)
		ret = ft_printf("exit\n");
	else if(msg == FAILURE)
		ret = write(2, "Unexpected Error.\n", 18);
	else if(msg == ERR_MEM)
		ret = write(2, "Unexpected Error: Failed to allocate memory.\n", 45);
	else if(msg == ERR_DUP)
		ret = write(2, "Unexpected Error: Failed to dup an fd.\n", 39);
	else if(msg == ERR_CLS)
		ret = write(2, "Unexpected Error: Failed to close a file.\n", 42);
	else if(msg == ERR_FRK)
		ret = write(2, "Unexpected Error: Failed to fork.\n", 34);
	else if(msg == ERR_PIP)
		ret = write(2, "Unexpected Error: Failed to pipe.\n", 34);
	if (msg == ERR_WRT || ret == -1)
		ret = write(2, "Unexpected Error: Failed to write.\n", 35);
}

void	free_and_exit(t_main *m, int msg)
{
	if (m->line)
		free(m->line);
	if (m->q)
		free_queue(&(m->q));
	if (m->env)
		free_env(&(m->env));
	if (m->cmds)
		free_cmd(&(m->cmds));
	handle_msg(msg);
	exit(m->status);
}
