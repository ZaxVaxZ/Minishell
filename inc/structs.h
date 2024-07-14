/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehammoud <ehammoud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/14 17:38:38 by ehammoud          #+#    #+#             */
/*   Updated: 2024/07/14 18:04:49 by ehammoud         ###   ########.fr       */
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
	Op_input,
	Op_output,
	Op_heredoc,
	Op_append,
	Op_or,
	Op_and,
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

typedef struct s_env
{
	char			*key;
	char			*value;
	t_bool			exported;
	struct s_env	*next;
}	t_env;

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
	t_token			before;
	t_token			after;
	t_queue			**parsed;
	struct s_cmd	*next;
}	t_cmd;

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
	t_env 	**env;
	t_cmd 	*cmd;
	t_exec 	*exec; 
	int 	*fds;
	int		i;
}	t_heredoc;

typedef struct s_expand
{
	char	*res;
	char	*var;
	char 	*line; 
	t_env 	**env;
	char	*name;
	char 	**words;
	int		newline;
	char	*var_name;
	char	*delimiter;
}	t_expand;

typedef struct s_main
{
	char	*line;
	char	*cwd;
	int		status;
	void	*interrupt;
	
	t_env	*env;
	t_queue	*q;
	t_cmd	*cmds;
}	t_main;

#endif