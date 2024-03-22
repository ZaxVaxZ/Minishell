/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 22:53:20 by marvin            #+#    #+#             */
/*   Updated: 2024/03/18 16:07:08 by pipolint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

int	count_words(t_queue *q)
{
	int		count;
	t_queue	*prev;

	count = 0;
	prev = NULL;
	while (q && q->type != Op_logic && q->type != Semicolon
		&& q->type != Op_pipe)
	{
		if (q->type == Word && (!prev
				|| (prev->type != Op_redir && prev->type != Assign)))
			count++;
		prev = q;
		q = q->next;
	}
	return (count);
}

void	clean_whitespace(t_queue *q)
{
	while (q->next)
	{
		if (q->next->type == Whitespace)
			delete_next(&q);
		q = q->next;
	}
}
t_bool	free_and_return(t_queue **q, t_cmd **cmd, char **temp)
{
	int	i;

	if ((*cmd)->input)
		free((*cmd)->input);
	if ((*cmd)->output)
		free((*cmd)->output);
	if (temp)
		free(temp);
	if ((*cmd)->params)
	{
		i = -1;
		while ((*cmd)->params[++i])
			free((*cmd)->params[i]);
		free((*cmd)->params);
	}
	if (q)
		free_queue(q);
	return (False);
}

t_bool	resolve_builtin(t_cmd *cmd, t_env **env)
{
	if (!ft_strncmp(cmd->params[0], "exit", -1))
		exiting(0);
	else if (!ft_strncmp(cmd->params[0], "echo", -1))
	{
		if (!ft_strncmp(cmd->params[1], "-n", 2) && cmd->params[2])
			echo(cmd->params + 2, 1);
		else if (cmd->params[1])
			echo(cmd->params + 1, 0);
	}
	else if (!ft_strncmp(cmd->params[0], "cd", -1) && cmd->params[1])
	{
		cd(get_var(*env, "PWD"), cmd->params[1], env);
	}
	else if (!ft_strncmp(cmd->params[0], "env", -1))
	{
		print_env(*env);
	}
	else if (!ft_strncmp(cmd->params[0], "unset", -1))
	{
		delete_var(env, cmd->params[1]);
	}
	else if (!ft_strncmp(cmd->params[0], "pwd", -1))
	{
		printf("%s\n", get_var(*env, "PWD"));
	}
	else if (!ft_strncmp(cmd->params[0], "export", -1))
	{
		//export(env, cmd->params[1],)
	}
	else
		return (False);
	return (True);
}

t_bool	build_commands(t_queue *q, t_cmd *cmd)
{
	int		i;
	char	*tmp;

	cmd->params = malloc(sizeof(char *) * (count_words(q) + 1));
	cmd->input = NULL;
	cmd->output = NULL;
	i = 0;
	while (q && q->type != Op_logic && q->type != Semicolon
		&& q->type != Op_pipe)
	{
		// if (q->type == Assign && q->next && q->next->type == Word)
		// {
		// 	tmp = ft_substr(q->s, 0, ft_strlen(q->s) - 1);
		// 	if (!tmp)
		// 		return (free_and_return(&q, &cmd, &tmp));
		// }
		// else if (q->type == Op_redir)
		// {
		// 	if (!ft_strncmp(q->s, "<", 1) && q->next)
		// 	{	
		// 		cmd->input = ft_strdup(q->next->s);
		// 		if (!cmd->input)
		// 			return (free_and_return(&q, &cmd, &tmp));
		// 	}
		// 	if (!ft_strncmp(q->s, ">", 1) && q->next)
		// 	{
		// 		cmd->output = ft_strdup(q->next->s);
		// 		if (!cmd->output)
		// 			return (free_and_return(&q, &cmd, &tmp));
		// 	}
		// }
		// else if (q->type == Word)
		if (q->type == Word)
		{
			cmd->params[i] = ft_strdup(q->s);
			if (!cmd->params[i++])
				return (free_and_return(&q, &cmd, &tmp));
		}
		q = q->next;
	}
	cmd->params[i] = NULL;
	return (0);
}

/// @brief searches for command in the env path variable
/// @param env the environment with the path variable
/// @param cmd the command
/// @return returns path if found, else NULL
char	*search_path(t_env **env, t_cmd *cmd)
{
	char	*path;
	char	**paths;
	char	*com;
	int		i;

	path = get_var(*env, "PATH");
	if (!path)
		return (NULL);
	paths = ft_split((const char *)path, ':');
	if (!paths)
	{
		printf("Couldn't find paths\n");
		return (NULL);
	}
	i = -1;
	while (paths[++i])
	{
		com = ft_strjoin_chr(paths[i], '/', cmd->params[0]);
		if (!com)
			return (NULL);
		if (!access(com, X_OK | F_OK))
			return (com);
	}
	return (NULL);
}

void	execute_command(t_env **env, t_cmd *cmd, t_queue **q)
{
	int		status;
	char	*valid;
	pid_t	test;

	valid = search_path(env, cmd);
	if (!valid)
	{
		printf("Couldn't find command\n");
		return ;
	}
	test = fork();
	if (test == 0)
	{
		status = execve(valid, cmd->params, to_char_arr(env));
		if (status == -1)
		{
			write(2, "execution error", 15);
			return ;
		}
	}
	else
		wait(NULL);
	(void)q;
}