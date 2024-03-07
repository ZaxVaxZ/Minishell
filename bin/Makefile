# Commands and Flags:
CC		=	cc
CFLAGS	=	-Wall -Wextra -Werror -fsanitize=address -g3

# Folders:
INC		=	./../inc/
SFD		=	./../src
LFT		=	$(SFD)/libft
GNL		=	$(SFD)/get_next_line
BIF		=	$(SFD)/built-ins
PRS		=	$(SFD)/parser

# Source Files:
SRCS	+=	$(LFT)/ft_strlen.c $(LFT)/ft_strdup.c $(LFT)/ft_strncmp.c \
			$(LFT)/ft_substr.c $(LFT)/ft_strchr.c $(LFT)/ft_split.c
SRCS	+=	$(GNL)/get_next_line.c $(GNL)/get_next_line_utils.c
SRCS	+=	$(BIF)/echo.c
SRCS	+=	$(PRS)/parser.c $(PRS)/utils.c $(PRS)/extras.c $(PRS)/quotes.c \
			$(PRS)/checkers.c
SRCS	+=	$(SFD)/minishell.c $(SFD)/queues.c

# Object and Program files
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
