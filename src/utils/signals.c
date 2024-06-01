/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehammoud <ehammoud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 20:19:48 by pipolint          #+#    #+#             */
/*   Updated: 2024/06/01 17:08:04 by ehammoud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "signals.h"

extern int	g_signum;

void	sig_handle(int signal)
{
	char	*tmp;

	if (signal == SIGINT)
	{
		g_signum = signal;
		rl_replace_line("", 0);
		write(1, "\n", 1);
		rl_on_new_line();
		rl_redisplay();
		// if (isatty(0))
		// {
		// 	// tmp = return_cwd(NULL);
		// 	rl_replace_line("", 0);
		// 	write(1, "\n", 1);
		// 	rl_on_new_line();
		// 	rl_redisplay();
		// 	// free(tmp);
		// }
	}
}

void	sig_heredoc(int sig)
{
	g_signum = sig;
	return ;
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
		// g_signum = -1;
	}
	if (g_signum == SIGINT)
	{
		tmp = ft_itoa(1);
		if (!tmp)
			return (False);
		if (!set_var(enviro, "?", tmp, False))
			return (False);
		free(tmp);
		// g_signum = -1;
	}
	return (True);
}
