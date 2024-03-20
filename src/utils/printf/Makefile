CC = gcc
CCFLAGS = -Wall -Wextra -Werror
AR = ar
ARFLAGS = -rcs
NAME = libftprintf.a
SRCS = ft_printaddr.c ft_printf.c ft_printint.c ft_printuint.c ft_extra_utils.c\
	   ft_handlestr.c ft_printchar.c ft_printhex.c ft_printstr.c ft_mods_utils.c
OBJS = $(SRCS:.c=.o)

all: $(NAME)

bonus: all

$(NAME): $(OBJS)
	$(AR) $(ARFLAGS) $(NAME) $(OBJS)

%.o: %.c
	$(CC) $(CCFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re