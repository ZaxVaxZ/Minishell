/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 15:13:59 by pipolint          #+#    #+#             */
/*   Updated: 2024/03/15 17:09:01 by pipolint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env_list.h"

/* -----------------------
 * Functions in the file:
 *   new_env_node()
 *   add_env_node()
 *   env_size()
 *   free_env_node()
 *   free_env()
 * -----------------------*/

/// @brief Create a new environment list node
/// @param key Variable name as the key
/// @param value The value stored for the variable
/// @return Returns the created node. NULL if a malloc fails
t_env	*new_env_node(char *key, char *value, t_bool exported)
{
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->key = ft_strdup(key);
	if (!node->key)
	{
		free(node);
		return (NULL);
	}
	node->value = ft_strdup(value);
	if (!node->value)
	{
		if (node->key)
			free(key);
		free(node);
		return (NULL);
	}
	node->exported = exported;
	node->next = NULL;
	return (node);
}

/// @brief Adds an env node to an env list
/// @param env The env list
/// @param to_add The node to be added
void	add_env_node(t_env **env, t_env *to_add)
{
	t_env	*trav;

	if (!env || !to_add)
		return ;
	if (!*env)
	{
		*env = to_add;
		return ;
	}
	trav = (*env);
	while (trav->next)
		trav = trav->next;
	trav->next = to_add;
}

/// @brief Calculates and returns the size of the env list
/// @param env The env list
/// @return The number of nodes in the list. 0 If empty
int	env_size(t_env *env)
{
	int	size;

	size = 0;
	while (env)
	{
		size++;
		env = env->next;
	}
	return (size);
}

/// @brief Free an env node and its contents
/// @param node The env node
void	free_env_node(t_env *node)
{
	if (node && node->key)
		free(node->key);
	if (node && node->value)
		free(node->value);
	if (node)
		free(node);
}

/// @brief Free each node of an env list, and their contents
/// @param env The env list pointer passed by address
/// @return Returns False to be used as a return value for caller function
t_bool	free_env(t_env **env)
{
	t_env	*tmp;

	while (env && *env)
	{
		tmp = *env;
		*env = (*env)->next;
		free_env_node(tmp);
	}
	*env = NULL;
	return (False);
}
