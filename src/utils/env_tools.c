/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_tools.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/16 18:25:51 by marvin            #+#    #+#             */
/*   Updated: 2024/04/02 16:57:39 by pipolint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env_list.h"

/* -----------------------
 * Functions in the file:
 *   set_var()
 *   add_var()
 *   get_var()
 *   delete_var()
 *   export_var()
 * -----------------------*/

/// @brief Sets the value of a variable in the environment
/// @param env The environment variable list
/// @param key The variable name
/// @param value The value to be set for the variable. NULL to keep old value
/// @param exported Whether or not this variable should be exported to env
/// @return False if any malloc fails. True otherwise
t_bool	set_var(t_env **env, char *key, char *value, t_bool exported)
{
	t_env	*tmp;

	tmp = *env;
	while (tmp && ft_strncmp(tmp->key, key, -1))
		tmp = tmp->next;
	if (!tmp)
		return (True);
	if (value)
	{
		free(tmp->value);
		tmp->value = ft_strdup(value);
		if (!tmp->value)
			return (free_env(env));
	}
	tmp->exported = exported;
	return (True);
}

/// @brief Creates a variable and adds it to the env variable list.
/// @param env The environment variable list
/// @param key The name of the variable
/// @param value The value to be assigned to the variable
/// @return False if any malloc fails. True otherwise
t_bool	add_var(t_env **env, char *key, char *value)
{
	t_env	*tmp;

	if (!env || !key || !*key || !value)
		return (True);
	if (get_var(*env, key))
		return (set_var(env, key, value, False));
	tmp = new_env_node(key, value, False);
	if (!tmp)
		return (free_env(env));
	add_env_node(env, tmp);
	return (True);
}

/// @brief Get the value of a variable in the env list
/// @param env The environment variable list
/// @param key The variable name
/// @return The value of the variable if it exists. NULL otherwise
char	*get_var(t_env *env, char *key)
{
	while (env)
	{
		if (!ft_strncmp(env->key, key, -1))
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

/// @brief Deletes a variable from the env if it existed
/// @param env The environment variable list
/// @param key Name of the variable
void	delete_var(t_env **env, char *key)
{
	t_env	*prev;
	t_env	*curr;

	if (!env || !*env || !key || !*key)
		return ;
	prev = *env;
	if (!ft_strncmp(prev->key, key, -1))
	{
		*env = (*env)->next;
		free_env_node(prev);
		return ;
	}
	curr = prev->next;
	while (curr)
	{
		if (!ft_strncmp(curr->key, key, -1))
		{
			prev->next = curr->next;
			free_env_node(curr);
			return ;
		}
		prev = curr;
		curr = curr->next;
	}
}

/// @brief Exports an existing variable
/// @param e The environment variable list
/// @param str String provided to export command. Assignment or variable name
/// @return False if any malloc fails. True otherwise
t_bool	export_var(t_env **e, char *str)
{
	t_bool	issue;
	char	*eq;
	char	*key;
	char	*val;

	issue = False;
	eq = ft_strchr(str, '=');
	if (eq)
	{
		key = ft_substr(str, 0, eq - str);
		if (!key)
			return (free_env(e));
		val = ft_substr(str, eq - str + 1, ft_strlen(eq + 1));
		issue = (!val || !add_var(e, key, val) || !set_var(e, key, NULL, True));
		free(key);
		if (val)
			free(val);
		if (issue)
			return (free_env(e));
		return (True);
	}
	else if (get_var(*e, str))
		return (set_var(e, str, NULL, True));
	return (True);
}
