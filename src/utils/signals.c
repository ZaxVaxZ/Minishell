/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehammoud <ehammoud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 20:19:48 by pipolint          #+#    #+#             */
/*   Updated: 2024/07/16 20:23:16 by ehammoud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "signals.h"

extern int	g_signum;

void	sig_handle(int sig)
{
	g_signum = sig;
	if (sig == SIGINT)
	{
		g_signum = sig;
		rl_replace_line("", 0);
		write(1, TEXT_RESET"\n", ft_strlen(TEXT_RESET) + 1);
		rl_on_new_line();
		rl_redisplay();
	}
}

t_bool	set_sig(t_main *m)
{
	char	*tmp;

	if (g_signum == SIGINT)
	{
		tmp = ft_itoa(1);
		if (!tmp || !set_var(&(m->env), "?", tmp, False))
			free_and_exit(m, ERR_MEM);
		free(tmp);
	}
	g_signum = -1;
	return (True);
}
