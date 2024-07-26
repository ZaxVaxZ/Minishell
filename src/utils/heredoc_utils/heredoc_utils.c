/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 14:59:06 by pipolint          #+#    #+#             */
/*   Updated: 2024/07/26 13:50:06 by pipolint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "general.h"

static int	get_and_write_var(t_heredoc *h, char *line, int i)
{
	int		j;
	char	*var;
	size_t	var_len;

	j = i + 1;
	while (is_valid_var_char(line[j]))
		j++;
	var = ft_substr(line, i, j - i);
	if (!var)
		free_and_exit(h->m, ERR_MEM);
	if (get_var(*(h->env), var))
	{
		var_len = ft_strlen(get_var(*(h->env), var));
		if (write(h->fds[WRITEEND], get_var(*(h->env), var), var_len) == -1)
			free_and_exit(h->m, ERR_WRT);
	}
	if (var)
		free(var);
	return (j);
}

void	write_exp_str(t_heredoc *h, char *line)
{
	int		i;
	int		ret;

	i = -1;
	while (line[++i])
	{
		if (line[i] != DS)
		{
			if (write(h->fds[WRITEEND], line + i, 1) == -1)
				free_and_exit(h->m, ERR_WRT);
		}
		else
		{
			if (!is_valid_var_char(line[i + 1]))
			{
				if (write(h->fds[WRITEEND], line + i++, 1) == -1)
					free_and_exit(h->m, ERR_WRT);
			}
			ret = (found_in(line[i + 1], DIGIT));
			i += 1 + ret;
			if (!ret)
				i = get_and_write_var(h, line, i) - 1;
		}
	}
}

t_bool	handle_heredoc_sigint(t_main *m, t_cmd **cmd, int *fds, t_exec *exec)
{
	char	*tmp;

	close(fds[WRITEEND]);
	close(fds[READEND]);
	exec->last_status = 1;
	(*cmd)->status = exec->last_status;
	tmp = ft_itoa(1);
	if (!tmp)
		free_and_exit(m, ERR_MEM);
	set_var(&m->env, "?", tmp, False);
	(*cmd)->heredoc_passed = 0;
	free(tmp);
	return (False);
}

int	should_break_heredoc(t_heredoc *h, char *line)
{
	if (!line || (!ft_strncmp(line, h->cmd->infiles[h->i],
				ft_strlen(line) - (line[ft_strlen(line) - 1] == '\n'))
			&& ft_strlen(h->cmd->infiles[h->i]) == ft_strlen(line)
			- (line[ft_strlen(line) - 1] == '\n')))
	{
		close(h->fds[WRITEEND]);
		return (1);
	}
	return (0);
}
