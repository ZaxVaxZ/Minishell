/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 05:23:38 by marvin            #+#    #+#             */
/*   Updated: 2024/04/02 16:57:23 by pipolint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env_list.h"
#include <stdio.h>
#include "ft_printf.h"

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

static void	add_sorted(t_env **env, t_env *node)
{
	t_env	*travel;

	if (!*env)
		*env = node;
	else if (ft_strncmp((*env)->key, node->key, -1) > 0)
	{
		node->next = *env;
		*env = node;
	}
	else
	{
		travel = *env;
		while (travel && travel->next
			&& ft_strncmp(travel->next->key, node->key, -1) < 0)
			travel = travel->next;
		node->next = travel->next;
		travel->next = node;
	}
}

static t_env	*clone_env(t_env *env)
{
	t_env	*tmp;
	t_env	*newenv;

	newenv = NULL;
	while (env)
	{
		tmp = new_env_node(env->key, env->value, env->exported);
		if (!tmp)
		{
			free_env(&newenv);
			return (NULL);
		}
		add_sorted(&newenv, tmp);
		env = env->next;
	}
	return (newenv);
}

void	print_sorted_env(t_env *env)
{
	t_env	*head;
	t_env	*clone;

	clone = clone_env(env);
	head = clone;
	while (clone)
	{
		if (clone->exported)
			ft_printf("%s=%s\n", clone->key, clone->value);
		clone = clone->next;
	}
	free_env(&head);
}
