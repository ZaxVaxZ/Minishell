/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tmp_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 22:25:25 by marvin            #+#    #+#             */
/*   Updated: 2024/03/31 17:00:35 by pipolint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tmp_utils.h"

t_bool	write_error(char *str)
{
	write(2, str, ft_strlen(str));
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

int	count_params(char **arr)
{
	int	i;

	i = 0;
	while (arr && arr[i])
		i++;
	return (i);
}

int	get_max(t_cmd *cmds)
{
	int	x;

	if (!cmds)
		return (-1);
	x = count_params(cmds->params);
	while (cmds)
	{
		if (count_params(cmds->params) > x)
			x = count_params(cmds->params);
		cmds = cmds->next;
	}
	return (x);
}
#include <stdio.h>
void	print_commands(t_cmd *cmds)
{
	printf("|-|%-10.10s", " Command");
	for (int i=1;i<get_max(cmds);i++)
		printf("|%-8.8s %d", " Params", i);
	printf("|%-10.10s|%-10.10s|%-10.10s|%-10.10s|\n", "  After", "  Input", " Output 1", " Output 2");
	while (cmds)
	{
		printf("|%d", cmds->depth);
		for (int i = 0; i<get_max(cmds); i++)
		{
			if (cmds->params[i])
				printf("|%-10.10s", cmds->params[i]);
			else
				printf("|%-10.10s", "");
		}
		if (cmds->heredoc)
			printf("|%-10.10s|%-10.10s", type_to_str(cmds->after), "heredoc");
		else if (cmds->input)
			printf("|%-10.10s|%-10.10s", type_to_str(cmds->after), cmds->input);
		else
			printf("|%-10.10s|%-10.10s", type_to_str(cmds->after), "   NULL");
		if (cmds->outfiles)
			printf("|%-10.10s|%-10.10s|\n", cmds->outfiles[0], cmds->outfiles[cmds->outfile_cnt - 1]);
		else
			printf("|%-10.10s|%-10.10s|\n", "   NULL", "   NULL");
		cmds = cmds->next;
	}
	//int	x;
	//int	i;
	// ls >out >>out2 >> out3 -a ; ((a && echo -a || ((ls >b<a))<q>a)<a<a<a>s>s>e);
	//if (!cmds)
	//{
	//	ft_printf("There are no commands!\n\n");
	//	return ;
	//}
	//x = get_max(cmds);
	//if (x > 9)
	//	x = 9;

	//ft_printf("|-");
	//i = 0;
	//while (i++ < x + 4)
	//	ft_printf("|----------");

	//ft_printf("|\n|-|%-10.10s|%-10.10s", "  Before", " Command");
	//i = 1;
	//while (i < x)
	//	ft_printf("|Parameter%d", i++);
	//ft_printf("|%-10.10s|%-10.10s|%-10.10s|\n|-",
	//	"  After", "  Input", "  Output");
	//i = 0;
	//while (cmds)
	//{
	//	while (i++ < x + 4)
	//		ft_printf("|----------");
	//	ft_printf("|\n|%d|%-10.10s|%-10.10s", cmds->depth, type_to_str(cmds->before),
	//		cmds->params[0]);
	//	i = 0;
	//	while (++i < x && cmds->params[i])
	//		ft_printf("|%-10.10s", cmds->params[i]);
	//	//ft_printf("|%-10.10s|%-10.10s|%-10.10s|\n|-", type_to_str(cmds->after),
	//	//	cmds->input, "");
	//	 ft_printf("|%-10.10s|%-10.10s|%-10.10s|\n|-", type_to_str(cmds->after),
	//	 	cmds->input, "Empty for now");
	//	for (int k = 0; cmds->ovrw_outs[k]; k++)
	//		printf("%s\n", cmds->ovrw_outs[k]);
	//	cmds = cmds->next;
	//}
	////i = 1;
	////while (i < x && cmds->params[i])
	////	ft_printf("|%-10.10s", cmds->params[i++]);
	////while (i++ < x)
	////	ft_printf("|%-10.10s", "");
	////ft_printf("|\n|-|%-10.10s|%-10.10s", type_to_str(cmds->before),
	////	cmds->params[0]);
	//// ft_printf("|%-10.10s|%-10.10s|%-10.10s|\n|-", type_to_str(cmds->after),
	//// 	cmds->input, cmds->output);
	//while (i++ < x + 4)
	//	ft_printf("|----------");
	//ft_printf("|\n");
}
