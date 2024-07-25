/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 20:23:38 by pipolint          #+#    #+#             */
/*   Updated: 2024/07/25 09:58:56 by pipolint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H

# include "general.h"
# include <signal.h>
# include "env_list.h"
# include "libft.h"

extern int	g_signum;

void	sig_handle(int signal);
t_bool	set_sig(t_main *m);
void	sig_heredoc(int signal);
void	do_nothing(int sig);
void	heredoc_sigint(int sig);

#endif
