### []+--------={ Commands and Flags }=--------+[] ###
CC		=	gcc
CFLAGS	=	-Wall -Wextra -Werror #-fsanitize=address -g3

### []+--------={ Directories }=--------+[] ###
INC		=	./../inc/
SFD		=	./../src
UTL		=	$(SFD)/utils
LFT		=	$(UTL)/libft
GNL		=	$(UTL)/get_next_line
BIF		=	$(SFD)/built-ins
PRS		=	$(SFD)/parser
EXE		=	$(SFD)/run

### []+--------={ Source Files }=--------+[] ###
SRCS	+=	$(LFT)/ft_strlen.c $(LFT)/ft_strdup.c $(LFT)/ft_strncmp.c \
			$(LFT)/ft_substr.c $(LFT)/ft_strchr.c $(LFT)/ft_strjoin.c
SRCS	+=	$(GNL)/get_next_line.c $(GNL)/get_next_line_utils.c
SRCS	+=	$(BIF)/cd.c $(BIF)/exit.c $(BIF)/echo.c
SRCS	+=	$(UTL)/queues.c
SRCS	+=	$(PRS)/parser.c $(PRS)/parser_utils.c $(PRS)/parser_extras.c \
			$(PRS)/parser_checkers.c $(PRS)/parser_cleanup.c
# SRCS	+=	$(EXE)/executor.c
SRCS	+=	$(SFD)/minishell.c

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
