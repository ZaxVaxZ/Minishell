/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 21:48:00 by marvin            #+#    #+#             */
/*   Updated: 2024/06/25 20:02:22 by pipolint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

void	exiting(char **params, t_exec *exec)
{
	if (!params)
		return ;
	if (!*params)
		return ;
	(*exec->exit_status) = ft_atoi(*params);
}
