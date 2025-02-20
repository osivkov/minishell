# Name of the executable
NAME = minishell

# Compiler and compiler flags
CC = cc
CFLAGS = -Wall -Wextra -Werror #-fPIE

# Directories for source files, headers, and object files
SRC_DIR = src
INCL_DIR = includes
OBJ_DIR = obj

# Directory for the libft library and the static library file
LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

# List of project source files
SRCS = $(SRC_DIR)/main/main.c \
		$(SRC_DIR)/utils/utils.c \
		$(SRC_DIR)/parser/lexer.c \
		$(SRC_DIR)/parser/parser.c \
		$(SRC_DIR)/parser/lexer_utils/lexer_utils.c\
		$(SRC_DIR)/parser/quotes.c \
		$(SRC_DIR)/parser/expand.c \
		$(SRC_DIR)/parser/type/heredoc.c \
		$(SRC_DIR)/signals/signals.c \
		# $(SRC_DIR)/executor/execute.c \
		# $(SRC_DIR)/executor/builtins.c \
		# $(SRC_DIR)/executor/pipes.c \
		# $(SRC_DIR)/executor/redirects.c \
		


# Convert source file paths into object file paths in the OBJ_DIR
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Default target: build the executable
all: $(NAME)

# Rule to link the final executable from object files and the libft library
$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) -I$(INCL_DIR) -I$(LIBFT_DIR) $(OBJS) $(LIBFT) -lreadline -lncurses  -o $(NAME)

# Rule to compile .c files into .o files, preserving directory structure
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I$(INCL_DIR) -I$(LIBFT_DIR) -c $< -o $@
# Create directory for the object file if it doesn't exist @mkdir -p $(dir $@)

# Build the libft static library by calling its Makefile
$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

# Clean: remove the object files directory
clean:
	rm -rf $(OBJ_DIR)
	$(MAKE) -C $(LIBFT_DIR) clean

# Fclean: clean object files and remove the executable, then clean libft
fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

# Rebuild: force clean and then build everything
re: fclean all

# Declare these targets as phony to avoid conflicts with files of the same name
.PHONY: all clean fclean re
