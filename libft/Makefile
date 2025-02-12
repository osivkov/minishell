NAME = libft.a

SRC = ft_strlen.c ft_isalpha.c ft_isdigit.c \
      ft_isalnum.c ft_isascii.c ft_isprint.c \
      ft_memset.c ft_bzero.c ft_memcpy.c \
      ft_memmove.c ft_strlcpy.c ft_strlcat.c\
      ft_toupper.c ft_tolower.c ft_strchr.c \
      ft_strrchr.c ft_strncmp.c ft_memchr.c \
      ft_memcmp.c ft_strnstr.c ft_atoi.c \
      ft_calloc.c ft_strdup.c ft_substr.c \
      ft_strjoin.c ft_strtrim.c ft_split.c \
      ft_itoa.c ft_strmapi.c ft_striteri.c \
      ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c \
      ft_putnbr_fd.c \
      $(GNL_DIR)/get_next_line.c \
      $(GNL_DIR)/get_next_line_utils.c \
      $(PRINT_DIR)/ft_printf.c \
      $(PRINT_DIR)/ft_printf_format_num.c \
      $(PRINT_DIR)/ft_printf_format.c \
      $(PRINT_DIR)/ft_printf_utils_num.c \
      $(PRINT_DIR)/ft_printf_utils.c

BONUS_SRC = ft_lstnew_bonus.c ft_lstadd_front_bonus.c \
            ft_lstsize_bonus.c ft_lstlast_bonus.c \
            ft_lstadd_back_bonus.c ft_lstdelone_bonus.c \
            ft_lstclear_bonus.c ft_lstiter_bonus.c \
            ft_lstmap_bonus.c

GNL_DIR = get_next_line
PRINT_DIR = ft_printf
OBJ = $(SRC:.c=.o)
BONUS_OBJ = $(BONUS_SRC:.c=.o)

CC = cc
CFLAGS = -Wall -Wextra -Werror $(INCLUDES)
AR = ar rcs
INCLUDES = -I $(GNL_DIR) -I $(PRINT_DIR)

all: $(NAME)

$(NAME): $(OBJ)
	$(AR) $(NAME) $(OBJ)
	make -C $(PRINT_DIR)

bonus: $(OBJ) $(BONUS_OBJ)
	$(AR) $(NAME) $(OBJ) $(BONUS_OBJ)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJ) $(BONUS_OBJ)
	make clean -C $(PRINT_DIR)
	

fclean: clean
	rm -f $(NAME)
	make fclean -C $(PRINT_DIR)

re: fclean all

.PHONY: all clean fclean re bonus
