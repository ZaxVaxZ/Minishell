/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehammoud <ehammoud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/14 16:41:40 by pipolint          #+#    #+#             */
/*   Updated: 2024/07/16 13:02:58 by ehammoud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

typedef enum e_bool
{
	False,
	True
}	t_bool;

typedef enum e_token
{
	Word,
	Name,
	Assign,
	Op_redir,
	Op_logic,
	Op_pipe,
	Variable,
	Semicolon,
	Sq_open,
	Sq_closed,
	Dq_open,
	Dq_closed,
	Bracket_open,
	Bracket_closed,
	Whitespace,
	Illegal
}	t_token;

typedef struct s_queue
{
	char			*s;
	t_token			type;
	struct s_queue	*next;
}	t_queue;

typedef struct s_cmd
{
	int				last_fd;
	int				status;
	int				in_fd;
	int				out_fd;
	int				params_cnt;
	int				infile_cnt;
	int				outfile_cnt;
	char			rep;
	char			**infiles;
	int				*in_flags;
	int				*out_flags;
	char			**outfiles;
	char			**params;
	t_bool			heredoc;
	t_bool			or_op;
	int				before;
	int				after;
	t_queue			**parse;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_env
{
	char			*key;
	char			*value;
	t_bool			exported;
	struct s_env	*next;
}	t_env;

typedef struct s_msh
{
	char	*line;
	char	*cwd;
	int		status;
	void	*interrupt;
	t_queue	*q;
	t_cmd	*cmds;
	t_env	*env;
}	t_msh;

#endif