/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 15:14:13 by pipolint          #+#    #+#             */
/*   Updated: 2024/03/15 17:03:43 by pipolint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H

# include <stdlib.h>
# include "general.h"
# include "libft.h"

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

t_env	*new_env_node(char *key, char *value);
t_bool	free_env(t_env **q);
void	add_to_env(t_env **env, t_env *to_add);
void	env_to_list(char **envp, t_env **e);
void	print_list(t_env *env);

#endif