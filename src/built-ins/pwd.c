/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 03:55:54 by marvin            #+#    #+#             */
/*   Updated: 2024/04/03 15:42:21 by pipolint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

t_bool	pwd(t_env *env)
{
	int		ret;
	char	*tmp;

	tmp = getcwd(NULL, 0);
	ret = ft_printf("%s\n", tmp);
	if (tmp)
		free(tmp);
	return (ret != -1);
}
