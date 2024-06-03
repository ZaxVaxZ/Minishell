/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 20:19:48 by pipolint          #+#    #+#             */
/*   Updated: 2024/06/03 12:53:19 by pipolint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "signals.h"

extern int	g_signum;

void	sig_handle(int signal)
{
	char	*tmp;

	g_signum = signal;
	if (signal == SIGINT)
	{
		g_signum = signal;
		rl_replace_line("", 0);
		write(1, "\n", 1);
		rl_on_new_line();
		rl_redisplay();
	}
}

void	sig_heredoc(int sig)
{
	if (sig == SIGINT)
		exit(EXIT_FAILURE);
}

t_bool	set_sig(t_env **enviro)
{
	char	*tmp;

	if (g_signum == SIGQUIT)
	{
		tmp = ft_itoa(131);
		if (!tmp)
			return (False);
		if (!set_var(enviro, "?", tmp, False))
			return (False);
		free(tmp);
	}
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
