/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tmp_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 22:25:25 by marvin            #+#    #+#             */
/*   Updated: 2024/07/13 14:38:57 by pipolint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tmp_utils.h"

t_bool	write_error(char *str)
{
	if (str)
	{
		if (write(2, str, ft_strlen(str)) == -1)
			return (False);
	}
	return (False);
}

char	*type_to_str(t_token type)
{
	if (type == Word)
		return ("Word");
	if (type == Name)
		return ("Name");
	if (type == Assign)
		return ("Assign");
	if (type == Op_pipe)
		return ("Op_pipe");
	if (type == Op_redir)
		return ("Op_redir");
	if (type == Op_logic)
		return ("Op_logic");
	if (type == Variable)
		return ("Variable");
	if (type == Semicolon)
		return ("Semicolon");
	if (type == Sq_open)
		return ("Sq_open");
	if (type == Sq_closed)
		return ("Sq_closed");
	if (type == Dq_open)
		return ("Dq_open");
	if (type == Dq_closed)
		return ("Dq_closed");
	if (type == Bracket_open)
		return ("Bracket_open");
	if (type == Bracket_closed)
		return ("Bracket_closed");
	if (type == Whitespace)
		return ("Whitespace");
	if (type == Illegal)
		return ("Illegal");
	return ("Unrecognized");
}

void	print_queue(t_queue *queue)
{
	if (!queue)
	{
		write(1, "The queue is empty!\n\n", 21);
		return ;
	}
	write(1, "|----------|----------|\n", 24);
	ft_printf("|%-10.10s|%-10.10s|\n", "  String", "   Type");
	write(1, "|----------|----------|\n", 24);
	while (queue)
	{
		ft_printf("|%-10.10s|%-10.10s|\n", queue->s, type_to_str(queue->type));
		queue = queue->next;
	}
	write(1, "|----------|----------|\n\n", 25);
}

int	get_max(t_cmd *cmds)
{
	int	x;

	if (!cmds)
		return (-1);
	x = cmds->params_cnt;
	while (cmds)
	{
		if (cmds->params_cnt > x)
			x = cmds->params_cnt;
		cmds = cmds->next;
	}
	return (x);
}

void	print_commands(t_cmd *cmds)
{
	ft_printf("\n");
	ft_printf("|-|%-10.10s", " Command");
	int	mx = get_max(cmds);
	for (int i=1;i<mx;i++)
		ft_printf("|%-7.7s %2d", " Param", i);
	ft_printf("|%-10.10s|%-10.10s|%-10.10s|%-10.10s|\n", "  After", "  Input", " Output 1", " Output 2");
	while (cmds)
	{
		if (cmds->rep)
			ft_printf("|%c", cmds->rep);
		else
			ft_printf("|-");
		for (int i = 0; i<mx; i++)
		{
			if (cmds->params && cmds->params[i])
				ft_printf("|%-10.10s", cmds->params[i]);
			else
				ft_printf("|%-10.10s", "");
		}
		// if (cmds->heredoc)
			// ft_printf("|%-10.10s|%-10.10s", type_to_str(cmds->after), "heredoc");
		// else if (cmds->input)
			// ft_printf("|%-10.10s|%-10.10s", type_to_str(cmds->after), cmds->input);
		// else
			// ft_printf("|%-10.10s|%-10.10s", type_to_str(cmds->after), "   NULL");
		if (cmds->outfiles)
			ft_printf("|%-10.10s|%-10.10s|\n", cmds->outfiles[0], cmds->outfiles[cmds->outfile_cnt - 1]);
		else
			ft_printf("|%-10.10s|%-10.10s|\n", "   NULL", "   NULL");
		cmds = cmds->next;
	}
}
