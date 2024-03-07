### []+--------={ Commands and Flags }=--------+[] ###
CC		=	cc
CFLAGS	=	-Wall -Wextra -Werror #-fsanitize=address -g3

### []+--------={ Directories }=--------+[] ###
INC		=	./../inc/
SFD		=	./../src
LFT		=	$(SFD)/libft
GNL		=	$(SFD)/get_next_line
BIF		=	$(SFD)/built-ins
PRS		=	$(SFD)/parser

### []+--------={ Source Files }=--------+[] ###
SRCS	+=	$(LFT)/ft_strlen.c $(LFT)/ft_strdup.c $(LFT)/ft_strncmp.c \
			$(LFT)/ft_substr.c $(LFT)/ft_strchr.c $(LFT)/ft_split.c
SRCS	+=	$(GNL)/get_next_line.c $(GNL)/get_next_line_utils.c
SRCS	+=	$(BIF)/echo.c
SRCS	+=	$(PRS)/parser.c $(PRS)/parser_utils.c $(PRS)/parser_extras.c \
			$(PRS)/parser_checkers.c $(PRS)/queues.c
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
