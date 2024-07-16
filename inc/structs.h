/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/14 16:41:40 by pipolint          #+#    #+#             */
/*   Updated: 2024/07/16 12:23:19 by pipolint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

typedef struct s_msh
{
	char	*line;
	char	*cwd;
	int		status;
	void	*interrupt;
	t_env	*env;
}	t_msh;

typedef enum e_bool
{
	False,
	True
}	t_bool;

typedef struct s_env
{
	char			*key;
	char			*value;
	t_bool			exported;
	struct s_env	*next;
}	t_env;

#endif