/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 20:19:48 by pipolint          #+#    #+#             */
/*   Updated: 2024/06/25 19:17:57 by pipolint         ###   ########.fr       */
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
		write(1, "\n", 1);
		rl_on_new_line();
		rl_redisplay();
	}
}

t_bool	set_sig(t_env **enviro)
{
	char	*tmp;

	if (g_signum == SIGINT)
	{
		tmp = ft_itoa(1);
		if (!tmp)
			return (False);
		if (!set_var(enviro, "?", tmp, False))
			return (False);
		free(tmp);
	}
	g_signum = -1;
	return (True);
}
