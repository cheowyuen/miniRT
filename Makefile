# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cwong <cwong@student.hive.fi>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/10/20 11:01:30 by cwong             #+#    #+#              #
#    Updated: 2025/10/20 11:33:39 by cwong            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC		:= cc

CFLAGS	:= -Wall -Wextra -Werror -Iincludes

NAME	:= miniRT

SRCS	:= $(wildcard src/*.c)

OBJ_DIR := obj
OBJS    := $(patsubst src/%.c,$(OBJ_DIR)/%.o,$(SRCS))

LIBFT_PATH	:= ./libft
LIBFT		:= $(LIBFT_PATH)/libft.a

MLX42_PATH	:= ./MLX42
MLX42		:= $(MLX42_PATH)/build/libmlx42.a

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT) $(MLX42)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBFT) $(MLX42) -ldl -lglfw -lm

$(OBJ_DIR)/%.o: src/%.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT):
	make -C $(LIBFT_PATH)

$(MLX42):
	cmake -S $(MLX42_PATH) -B $(MLX42_PATH)/build
	cmake --build $(MLX42_PATH)/build

clean:
	$(MAKE) -C $(LIBFT_PATH) clean
	rm -rf $(OBJ_DIR)
	rm -rf $(MLX42_PATH)/build

fclean: clean
	$(MAKE) -C $(LIBFT_PATH) fclean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
