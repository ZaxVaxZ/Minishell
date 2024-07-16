/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehammoud <ehammoud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 20:23:38 by pipolint          #+#    #+#             */
/*   Updated: 2024/07/16 19:46:47 by ehammoud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H

# include "general.h"
# include "env_list.h"
# include "libft.h"

int	g_signum;

void	sig_handle(int signal);
t_bool	set_sig(t_main *m);
void	sig_heredoc(int signal);

#endif
