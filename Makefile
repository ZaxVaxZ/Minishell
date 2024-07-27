### []+--------={ Commands and Flags }=--------+[] ###
CC		=	cc
CFLAGS	=	-Wall -Wextra -Werror

### []+--------={ Directories }=--------+[] ###
INC		=	./inc
SFD		=	./src
UTL		=	$(SFD)/utils
LFT		=	$(UTL)/libft
GNL		=	$(UTL)/get_next_line
PRF		=	$(UTL)/printf
PRS		=	$(UTL)/parser
CMD		=	$(UTL)/cmd_utils
ENV		=	$(UTL)/env_utils
HRD		=	$(UTL)/heredoc_utils
BIF		=	$(SFD)/built-ins
EXE		=	$(SFD)/run

### []+--------={ Source Files }=--------+[] ###
SRCS	=	$(LFT)/ft_strlen.c $(LFT)/ft_strdup.c $(LFT)/ft_strncmp.c		\
			$(LFT)/ft_substr.c $(LFT)/ft_strchr.c $(LFT)/ft_strjoin.c		\
			$(LFT)/ft_strjoin_chr.c $(LFT)/ft_split.c $(LFT)/ft_itoa.c		\
			$(LFT)/ft_strtrim.c $(LFT)/ft_atoi.c $(SFD)/minishell_utils.c	\
			$(GNL)/get_next_line.c $(GNL)/get_next_line_utils.c				\
			$(PRF)/ft_mods_utils.c $(PRF)/ft_printaddr.c $(PRF)/ft_printf.c	\
			$(PRF)/ft_printhex.c $(PRF)/ft_printstr.c $(PRF)/ft_printchar.c	\
			$(PRF)/ft_handlestr.c $(PRF)/ft_extra_utils.c					\
			$(PRF)/ft_printuint.c $(PRF)/ft_printint.c						\
			$(BIF)/unset.c $(BIF)/pwd.c $(BIF)/echo.c $(BIF)/export.c		\
			$(BIF)/exit.c $(BIF)/env.c $(BIF)/cd.c							\
			$(UTL)/queues.c $(ENV)/env_list.c $(ENV)/env_tools.c			\
			$(ENV)/env_utils.c $(CMD)/cmd_list.c $(CMD)/cmd_tools.c			\
			$(CMD)/cmd_utils.c	$(HRD)/pipe_utils.c							\
			$(PRS)/parser.c $(PRS)/parser_utils.c $(PRS)/parser_extras.c	\
			$(PRS)/parser_checkers.c $(PRS)/parser_cleanup.c 				\
			$(EXE)/run_builtins.c $(EXE)/run_utils.c						\
			$(SFD)/minishell.c	$(PRS)/parser_utils2.c						\
			$(HRD)/heredoc.c $(HRD)/pipe_checks.c $(EXE)/last_exec.c		\
			$(UTL)/signals.c $(UTL)/free_and_exit.c $(EXE)/exec_utils.c		\
			$(CMD)/cmd_in_out.c $(HRD)/heredoc_utils.c

### []+--------={ Object files and Program name }=--------+[] ###
OBJS	=	$(SRCS:.c=.o)
NAME	=	minishell
BONUS	=	minishell_bonus

all: $(NAME)

bonus: $(BONUS)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(LIBS) -L/opt/vagrant/embedded/lib/ -I/opt/vagrant/embedded/include/readline $(OBJS) -o $@ -lreadline -lhistory

$(BONUS): $(OBJS)
	@$(CC) $(CFLAGS) $(LIBS) -L/opt/vagrant/embedded/lib/ -I/opt/vagrant/embedded/include/readline $(OBJS) -o $@ -lreadline -lhistory

%.o: %.c
	@$(CC) -I$(INC) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJS)

fclean: clean
	@rm -rf $(NAME)
	@rm -rf $(BONUS)

re: fclean all

.PHONY: all clean fclean re
