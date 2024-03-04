CC = cc
CFLAGS = -Wall -Wextra -Werror -fsanitize=address -g3
INC = ./../inc/
SFD = ./../src
LFT = $(SFD)/libft
GNL = $(SFD)/get_next_line
SRCS += $(LFT)/ft_strlen.c $(LFT)/ft_strdup.c $(LFT)/ft_strncmp.c $(LFT)/ft_substr.c $(LFT)/ft_strchr.c $(LFT)/ft_split.c
SRCS += $(GNL)/get_next_line.c $(GNL)/get_next_line_utils.c
SRCS += $(SFD)/parser.c $(SFD)/pre_parser.c $(SFD)/parser_utils.c $(SFD)/parse_quotes.c
SRCS += $(SFD)/queues.c
SRCS += $(SFD)/minishell.c
OBJS = $(SRCS:.c=.o)
NAME = minishell

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
