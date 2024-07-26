/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tmp_utils.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 22:25:15 by marvin            #+#    #+#             */
/*   Updated: 2024/03/30 18:06:35 by pipolint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TMP_UTILS_H
# define TMP_UTILS_H

# include "queues.h"
# include "cmd_list.h"
# include "ft_printf.h"

char	*type_to_str(t_token type);
void	print_queue(t_queue *queue);
void	print_commands(t_cmd *cmds);
t_bool	write_error(char *str);

#endif