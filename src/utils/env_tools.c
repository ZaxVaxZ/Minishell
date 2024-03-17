/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_tools.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/16 18:25:51 by marvin            #+#    #+#             */
/*   Updated: 2024/03/16 18:25:51 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env_list.h"

/* -----------------------
 * Functions in the file:
 *   set_var()
 *   get_var()
 *   delete_var()
 *   to_env_list()
 *   to_char_arr()
 * -----------------------*/

/// @brief Sets a variable's value. Creates the variable if it doesn't exist
/// @param env The env list
/// @param key The name of the variable
/// @param value The value to be assigned to the variable. NULL to keep it as is
/// @param exported 1/0 To set exported state to True/False. -1 to keep it as is
/// @return False if any malloc fails. True otherwise
t_bool	set_var(t_env **env, char *key, char *value, int exported)
{
	t_env	*tmp;

	if (!env || !key)
		return (True);
	if (!get_var(*env, key))
	{
		if (!value)
			return (True);
		tmp = new_env_node(key, value, exported + (exported < 0));
		if (!tmp)
			return (free_env(env));
		add_env_node(env, tmp);
		return (True);
	}
	tmp = *env;
	while (tmp && ft_strncmp(tmp->key, key, -1))
		tmp = tmp->next;
	free(tmp->value);
	if (value)
		tmp->value = ft_strdup(value);
	if (!tmp->value)
		return (free_env(env));
	tmp->exported = exported + (exported < 0);
	return (True);
}

/// @brief Get the value of a variable in the env list
/// @param env The env list
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
/// @param env The env list
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
		add_env_node(&env, tmp);
		if ((!tmp->key || !tmp->value) && !free_env(&env))
			return (NULL);
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
