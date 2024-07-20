/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/14 16:41:40 by pipolint          #+#    #+#             */
/*   Updated: 2024/07/20 15:36:42 by pipolint         ###   ########.fr       */
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

typedef struct s_main
{
	char	*line;
	char	*cwd;
	int		status;
	void	*interrupt;
	t_queue	*q;
	t_cmd	*cmds;
	t_env	*env;
}	t_main;

typedef struct s_exec
{
	int		ret;
	int		curr_depth;
	int		status_depth;
	int		overall_status;
	int		last_status;
	int		last_op;
	int		*exit_status;
	int		std_in;
	int		std_out;
	int		*fds;
	t_env	**env;
	pid_t	last_pid;
	t_cmd	**cmd_head;
}	t_exec;

typedef struct s_heredoc
{
	t_main	*m;
	t_env 	**env;
	t_cmd 	*cmd;
	t_exec 	*exec;
	int 	*fds;
	int		i;
}	t_heredoc;


#endif