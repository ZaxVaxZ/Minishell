### []+--------={ Commands and Flags }=--------+[] ###
CC		=	cc
CFLAGS	=	#-Wall -Wextra -Werror -O3 -fsanitize=address -g3

### []+--------={ Directories }=--------+[] ###
INC		=	./../inc
SFD		=	./../src
UTL		=	$(SFD)/utils
LFT		=	$(UTL)/libft
GNL		=	$(UTL)/get_next_line
PRF		=	$(UTL)/printf
BIF		=	$(SFD)/built-ins
PRS		=	$(SFD)/parser
EXE		=	$(SFD)/run

### []+--------={ Source Files }=--------+[] ###
SRCS	=	$(LFT)/ft_strlen.c $(LFT)/ft_strdup.c $(LFT)/ft_strncmp.c		\
			$(LFT)/ft_substr.c $(LFT)/ft_strchr.c $(LFT)/ft_strjoin.c		\
			$(LFT)/ft_strjoin_chr.c $(LFT)/ft_split.c						\
			$(GNL)/get_next_line.c $(GNL)/get_next_line_utils.c				\
			$(PRF)/ft_mods_utils.c $(PRF)/ft_printaddr.c $(PRF)/ft_printf.c	\
			$(PRF)/ft_printhex.c $(PRF)/ft_printstr.c $(PRF)/ft_printchar.c	\
			$(PRF)/ft_handlestr.c $(PRF)/ft_extra_utils.c					\
			$(PRF)/ft_printuint.c $(PRF)/ft_printint.c						\
			$(BIF)/unset.c $(BIF)/pwd.c $(BIF)/echo.c $(BIF)/export.c		\
			$(BIF)/exit.c $(BIF)/env.c $(BIF)/cd.c							\
			$(UTL)/queues.c $(UTL)/env_list.c $(UTL)/env_tools.c			\
			$(UTL)/env_utils.c $(UTL)/cmd_list.c ./tester_cmd_tools.c		\
			$(UTL)/cmd_utils.c												\
			$(PRS)/parser.c $(PRS)/parser_utils.c $(PRS)/parser_extras.c	\
			$(PRS)/parser_checkers.c $(PRS)/parser_cleanup.c				\
			$(EXE)/executor.c ./tester_minishell.c		./../tmp/tmp_utils.c

### []+--------={ Object files and Program name }=--------+[] ###
OBJS	=	$(SRCS:.c=.o)
NAME	=	minishell

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $@

%.o: %.c
	@$(CC) -I$(INC) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJS)

fclean: clean
	@rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
