/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 20:23:38 by pipolint          #+#    #+#             */
/*   Updated: 2024/07/18 14:39:11 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H

# include "general.h"
# include "env_list.h"
# include "libft.h"

extern int	g_signum;

void	sig_handle(int signal);
t_bool	set_sig(t_main *m);
void	sig_heredoc(int signal);

#endif
