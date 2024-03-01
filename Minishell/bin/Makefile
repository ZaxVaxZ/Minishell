CC = cc
CFLAGS = -Wall -Wextra -Werror
INC = ./../inc/
SFD = ./../src
GNL = $(SFD)/get_next_line
SRCS = $(GNL)/get_next_line.c $(GNL)/get_next_line_utils.c
SRCS += $(SFD)/parser.c $(SFD)/pre_parser.c
SRCS += $(SFD)/minishell.c
OBJS = $(SRCS:.c=.o)
NAME = minishell

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(OBJS) -o $@

%.o: %.c
	$(CC) -I$(INC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
