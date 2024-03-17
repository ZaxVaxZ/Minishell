/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/02 13:07:12 by pipolint          #+#    #+#             */
/*   Updated: 2024/03/15 16:52:57 by pipolint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <sys/types.h>
# include <sys/stat.h>
# include "general.h"
# include "libft.h"
# include "parser.h"
# include "env_list.h"

t_bool	echo(char **str, t_bool n);
t_bool	print_env(t_env *env);
void	pwd(t_env *env);
void	exiting(int code);
int		cd(char *pwd, char *dir);
void	unset(char *var_name, t_env **env);
void	export(t_env **env, char *variable);

#endif
