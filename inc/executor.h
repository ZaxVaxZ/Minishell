/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 22:53:31 by marvin            #+#    #+#             */
/*   Updated: 2024/03/14 22:53:31 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

typedef struct s_command
{
	char	*input;
	char	**params;
	char	*output;
	int		status;
}	t_command;

# include <stdlib.h>
# include "libft.h"
# include "parser.h"

#endif