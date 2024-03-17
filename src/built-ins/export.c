/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/16 21:57:20 by marvin            #+#    #+#             */
/*   Updated: 2024/03/16 21:57:20 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

t_bool	export(t_env **env, char *variable, char *value)
{
	return (set_var(env, variable, value, 1));
}
