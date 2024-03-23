/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 05:23:38 by marvin            #+#    #+#             */
/*   Updated: 2024/03/23 20:41:17 by pipolint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env_list.h"
#include <stdio.h>

/* -----------------------
 * Functions in the file:
 *   to_env_list()
 *   to_char_arr()
 * -----------------------*/

/// @brief Turns an array of key=value pairs as strings into an env list
/// @param strs The list of strings containing key=value strings
/// @return The env list created from the strings. NULL if a malloc fails
t_env	*to_env_list(char **strs)
{
	int		i;
	char	*eqp;
	t_env	*tmp;
	t_env	*env;

	if (!strs)
		return (NULL);
	env = NULL;
	i = 0;
	while (strs[i])
	{
		tmp = malloc(sizeof(t_env));
		if (!tmp && !free_env(&env))
			return (NULL);
		eqp = ft_strchr(strs[i], '=');
		tmp->key = ft_substr(strs[i], 0, eqp - strs[i]);
		tmp->value = ft_substr(strs[i], eqp - strs[i] + 1, ft_strlen(eqp + 1));
		if ((!tmp->key || !tmp->value) && !free_env(&env))
			return (NULL);
		tmp->exported = True;
		tmp->next = NULL;
		add_env_node(&env, tmp);
		i++;
	}
	return (env);
}

/// @brief Turns an env list into an array of key=value pairs as strings
/// @param env The env list
/// @return The string list created from the env list. NULL if a malloc fails
char	**to_char_arr(t_env **env)
{
	int		i;
	char	**strs;
	t_env	*tmp;
	t_bool	fail;

	strs = malloc(sizeof(char *) * (env_size(*env) + 1));
	fail = (strs == NULL);
	tmp = *env;
	i = 0;
	while (tmp && !fail)
	{
		strs[i] = ft_strjoin_chr(tmp->key, '=', tmp->value);
		if (!strs[i++])
			fail = !free_env(env);
		tmp = tmp->next;
	}
	if (fail)
		return (NULL);
	strs[i] = NULL;
	return (strs);
}
