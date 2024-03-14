/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/02 13:31:22 by pipolint          #+#    #+#             */
/*   Updated: 2024/03/13 15:28:46 by pipolint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

void	cd(t_env *b, char *dir)
{
	b->old_pwd = b->pwd;
	if (chdir(dir) == -1)
		perror(NULL);
	b->pwd = getenv("PWD");
	printf("Old PWD: %s\n", b->old_pwd);
	printf("Current PWD: %s\n", b->pwd);
}
