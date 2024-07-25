/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   general.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 12:43:36 by ehammoud          #+#    #+#             */
/*   Updated: 2024/07/25 16:29:22 by pipolint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GENERAL_H
# define GENERAL_H

# define BLACK   "\e[0;30m" // Black - Regular
# define RED     "\e[0;31m" // Red
# define GREEN   "\e[0;32m" // Green
# define YELLOW  "\e[0;33m" // Yellow
# define BLUE    "\e[0;34m" // Blue
# define PURPLE  "\e[0;35m" // Purple
# define CYAN    "\e[0;36m" // Cyan
# define WHITE   "\e[0;37m" // White

# define BLACK_BOLD  "\e[1;30m" // Black - Bold
# define RED_BOLD    "\e[1;31m" // Red
# define GREEN_BOLD  "\e[1;32m" // Green
# define YELLOW_BOLD "\e[1;33m" // Yellow
# define BLUE_BOLD   "\e[1;34m" // Blue
# define PURPLE_BOLD "\e[1;35m" // Purple
# define CYAN_BOLD   "\e[1;36m" // Cyan
# define WHITE_BOLD  "\e[1;37m" // White

# define BLACK_UNDERLINE   "\e[4;30m" // Black - Underline
# define RED_UNDERLINE     "\e[4;31m" // Red
# define GREEN_UNDERLINE   "\e[4;32m" // Green
# define YELLOW_UNDERLINE  "\e[4;33m" // Yellow
# define BLUE_UNDERLINE    "\e[4;34m" // Blue
# define PURPLE_UNDERLINE  "\e[4;35m" // Purple
# define CYAN_UNDERLINE    "\e[4;36m" // Cyan
# define WHITE_UNDELINE    "\e[4;37m" // White

# define BLACK_BACKGROUND  "\e[40m"   // Black - Background
# define RED_BACKGROUND    "\e[41m"   // Red
# define GREEN_BACKGROUND  "\e[42m"   // Green
# define YELLOW_BACKGROUND "\e[43m"   // Yellow
# define BLUE_BACKGROUND   "\e[44m"   // Blue
# define PURPLE_BACKGROUND "\e[45m"   // Purple
# define CYAN_BACKGROUND   "\e[46m"   // Cyan
# define WHITE_BACKGROUND  "\e[47m"   // White
# define TEXT_RESET        "\e[0m"    // Text Reset

# define UNDERSCORE '_'
# define LOWERCASE "abcdefghijklmnopqrstuvwxyz"
# define UPPERCASE "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
# define DIGIT "0123456789"
# define EQUAL '='
# define SPACE ' '
# define PIPE '|'
# define DASH '-'
# define TAB '\t'
# define AND '&'
# define INF '<'
# define OUF '>'
# define NL '\n'
# define SQ '\''
# define DQ '\"'
# define LP '('
# define RP ')'
# define SC ';'
# define DS '$'

# define READEND 0
# define WRITEEND 1
# define SUCCESS 0
# define FAILURE 1
# define ERR_MEM 2
# define ERR_DUP 3
# define ERR_WRT 4
# define ERR_CLS 5
# define ERR_FRK 6
# define ERR_PIP 7
# define ERR_CWD 8

# include <stdio.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "structs.h"

void	rl_replace_line(const char *text, int clear_undo);
char	*get_line(t_main *m);
void	handle_msg(int msg);
int		free_up_cmd_mem(t_main *m);
void	heredoc_exit(t_heredoc *h);
void	free_and_exit(t_main *m, int msg);
int		handle_cmd_line(t_main *m);
void	child_free_and_exit(t_env **env, t_exec *exec, int status);

#endif
