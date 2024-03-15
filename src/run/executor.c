/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 22:53:20 by marvin            #+#    #+#             */
/*   Updated: 2024/03/15 17:00:18 by pipolint         ###   ########.fr       */
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
t_bool	free_and_return(t_queue **q, t_command **cmd, char **temp)
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
	return (True);
}

//t_bool	execute(t_queue *q, t_command *cmd)
//{
//	int		i;
//	char	*tmp;

//	(void)q;
//	clean_whitespace(q);
//	cmd->params = malloc(sizeof(char *) * (count_words(q) + 1));
//	cmd->input = NULL;
//	cmd->output = NULL;
//	i = 0;
//	while (q && q->type != Op_logic && q->type != Semicolon
//		&& q->type != Op_pipe)
//	{
//		if (q->type == Assign && q->next && q->next->type == Word)
//		{
//			tmp = ft_substr(q->s, 0, ft_strlen(q->s) - 1);
//			if (!tmp)
//				return (free_and_return(&q, &cmd, &tmp));
//			setenv(tmp, q->next->s, 1);
//			q = q->next;
//		}
//		else if (q->type == Op_redir)
//		{
//			if (!ft_strncmp(q->s, "<", 1))
//			{
//				cmd->input = ft_strdup(q->next->s);
//				if (!cmd->input)
//					return (free_and_return(&q, &cmd, &tmp));
//			}
//			if (!ft_strncmp(q->s, "<", 1))
//			{
//				cmd->input = ft_strdup(q->next->s);
//				if (!cmd->input)
//					return (free_and_return(&q, &cmd, &tmp));
//			}
//		}
//		else if (q->type == Word)
//		{
//			cmd->params[i] = ft_strdup(q->s);
//			if (!cmd->params[i++])
//				return (free_and_return(&q, &cmd, &tmp));
//		}
//		q = q->next;
//	}
//	cmd->params[i] = NULL;
//	return (0);
//}
