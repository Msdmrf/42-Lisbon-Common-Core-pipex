# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: migusant <migusant@student.42lisboa.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/02 16:11:44 by migusant          #+#    #+#              #
#    Updated: 2025/08/04 21:06:18 by migusant         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# **************************************************************************** #
#                              PROJECT SETTINGS                                #
# **************************************************************************** #

NAME = pipex
BONUS_NAME = pipex_bonus
LIBFT = libft/libft.a

# **************************************************************************** #
#                              COMPILER SETTINGS                               #
# **************************************************************************** #

CC = cc
CFLAGS = -Wall -Wextra -Werror
RM = rm -f

# **************************************************************************** #
#                             DIRECTORY STRUCTURE                              #
# **************************************************************************** #

SRC_DIR = sources/
OBJ_DIR = objects/
INC_DIR = includes/
LIBFT_DIR = libft/

# **************************************************************************** #
#                                  COLORS                                      #
# **************************************************************************** #

GREEN = \033[0;32m
RED = \033[0;31m
YELLOW = \033[0;33m
BLUE = \033[0;34m
RESET = \033[0m

# **************************************************************************** #
#                              SOURCE FILES                                    #
# **************************************************************************** #

COMMON_FILES = pipex_utils.c \
				process_handlers.c \
				path_finder.c \
				error_handling.c

SRC_FILES = pipex.c \
				$(COMMON_FILES)

BONUS_SRC_FILES = pipex_bonus.c \
				here_doc.c \
				here_doc_utils.c \
				multiple_pipes.c \
				multiple_pipes_utils.c \
				multiple_pipes_process.c \
				$(COMMON_FILES)

SRC = $(addprefix $(SRC_DIR), $(SRC_FILES))
OBJ = $(addprefix $(OBJ_DIR), $(SRC_FILES:.c=.o))

BONUS_SRC = $(addprefix $(SRC_DIR), $(BONUS_SRC_FILES))
BONUS_OBJ = $(addprefix $(OBJ_DIR), $(BONUS_SRC_FILES:.c=.o))

# **************************************************************************** #
#                                 TARGETS                                      #
# **************************************************************************** #

all: $(LIBFT) $(NAME)

bonus: $(LIBFT) $(BONUS_NAME)

$(LIBFT):
	@echo "$(YELLOW)Building libft...$(RESET)"
	@make -C $(LIBFT_DIR)

$(NAME): $(OBJ)
	@echo "$(YELLOW)Building $(NAME)...$(RESET)"
	@$(CC) $(OBJ) -L$(LIBFT_DIR) -lft -o $(NAME)
	@echo "$(GREEN)$(NAME) successfully created!$(RESET)"

$(BONUS_NAME): $(BONUS_OBJ)
	@echo "$(YELLOW)Building $(NAME) with bonus features...$(RESET)"
	@$(CC) $(BONUS_OBJ) -L$(LIBFT_DIR) -lft -o $(NAME)
	@echo "$(GREEN)$(NAME) with bonus features successfully created!$(RESET)"
	@touch $(BONUS_NAME)

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	@mkdir -p $(OBJ_DIR)
	@echo "$(YELLOW)Compiling: $<$(RESET)"
	@$(CC) $(CFLAGS) -I$(INC_DIR) -I$(LIBFT_DIR)/includes -c $< -o $@

# **************************************************************************** #
#                              CLEANING RULES                                  #
# **************************************************************************** #

clean:
	@if [ -d "$(OBJ_DIR)" ]; then \
		$(RM) -r $(OBJ_DIR); \
		echo "$(RED)Object files have been cleaned!$(RESET)"; \
		echo "$(YELLOW)└── Removed directory: $(OBJ_DIR)$(RESET)"; \
	fi
	@$(RM) $(BONUS_NAME)
	@make -C $(LIBFT_DIR) clean

fclean: clean
	@if [ -f "$(NAME)" ]; then \
		echo "$(RED)Everything has been cleaned!$(RESET)"; \
		$(RM) $(NAME); \
		echo "$(YELLOW)└── Removed executable: $(NAME)$(RESET)"; \
	fi
	@make -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all bonus clean fclean re