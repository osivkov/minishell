# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: osivkov <osivkov@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/22 11:30:56 by osivkov           #+#    #+#              #
#    Updated: 2024/10/22 17:38:10 by osivkov          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libftprintf.a 

CC = cc
CFLAGS = -Wall -Wextra -Werror

SRC = ft_printf.c ft_printf_utils.c ft_printf_format.c \
		ft_printf_format_num.c ft_printf_utils_num.c
OBJ = $(SRC:.c=.o)

all: $(NAME)
$(NAME): $(OBJ)
	ar rcs $(NAME) $(OBJ)

%.o: %.c ft_printf.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

#bonus:

.PHONY: all clean fclean re bonus