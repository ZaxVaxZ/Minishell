/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/02 13:07:12 by pipolint          #+#    #+#             */
/*   Updated: 2024/07/24 21:20:05 by pipolint         ###   ########.fr       */
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
# include "ft_printf.h"
# include "executor.h"

t_bool	cd(t_main *m, char *pwd, char *dir);
t_bool	unset(t_main *m, char **params);
t_bool	pwd(t_main *m);
t_bool	print_env(t_main *m);
t_bool	echo(t_main *m, char **s);
t_bool	export_cmd(t_main *m, char **variable);
t_bool	valid_identifier(char *id);

#endif
