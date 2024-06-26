/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 21:48:00 by marvin            #+#    #+#             */
/*   Updated: 2024/06/26 13:26:10 by pipolint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

int	write_num_error(char *param, t_exec *exec)
{
	if (write(2, "exit: ", 6) == -1)
		return (-1);
	if (write(2, param, ft_strlen(param)) == -1)
		return (-1);
	if (write(2, ": numeric argument required\n", 28) == -1)
		return (-1);
	(*exec->exit_status) = 255;
	return (1);
}

int	exiting(char **params, t_exec *exec)
{
	int	code;
	int	i;

	if (!params)
		return (1);
	if (!*params)
		return (1);
	i = 0;
	while ((*params)[i])
	{
		if ((*params)[i] >= '0' && (*params)[i] <= '9')
			i++;
		else
			return (write_num_error(*params, exec));
	}
	if (*(params + 1))
	{
		if (write(2, "exit: too many arguments\n", 25) == -1)
			return (-2);
		return (-2);
	}
	code = ft_atoi(*params);
	(*exec->exit_status) = code;
	return (1);
}
