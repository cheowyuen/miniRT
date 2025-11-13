# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cwong <cwong@student.hive.fi>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/10/20 11:01:30 by cwong             #+#    #+#              #
#    Updated: 2025/11/13 15:30:28 by nraatika         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC		:= cc

CFLAGS	:= -Wall -Wextra -Werror -O3 -march=native
DEBUG	:= -g

NAME	:= miniRT

SRCS	:= $(wildcard src/*.c)

OBJ_DIR := obj
OBJS    := $(patsubst src/%.c,$(OBJ_DIR)/%.o,$(SRCS))

LIBFT_PATH	:= ./libft
LIBFT		:= $(LIBFT_PATH)/libft.a

MLX_PATH	:= ./MLX/build/
MLX_NAME	:= libmlx42.a
MLX_BPATH	:= ./MLX/
MLX			:= $(MLX_PATH)$(MLX_NAME)

#MLX42_PATH	:= ./MLX42
#MLX42		:= $(MLX42_PATH)/build/libmlx42.a

INC_DIR		:= includes
INC			:= -I./MLX/include/MLX42 -I./$(INC_DIR) -I$(LIBFT_PATH)
LIBS		:= -L$(MLX_PATH) -lmlx42 -lXext -lX11 -lm -lz -ldl -lglfw -pthread


all: $(NAME)

$(NAME): $(OBJS) $(LIBFT) $(MLX)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBFT) $(MLX) -ldl -lglfw -lm

$(OBJ_DIR)/%.o: src/%.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

$(LIBFT):
	make -C $(LIBFT_PATH)

$(MLX):		
		git clone -q --depth 1 --branch v2.4.1 --single-branch\
			https://github.com/codam-coding-college/MLX42.git MLX
		cd $(MLX_BPATH)	&& cmake -B build
		$(MAKE) -C $(MLX_PATH)


#$(MLX42):
#	cmake -S $(MLX42_PATH) -B $(MLX42_PATH)/build
#	cmake --build $(MLX42_PATH)/build

clean:
	$(MAKE) -C $(LIBFT_PATH) clean
	rm -rf $(OBJ_DIR)
	rm -rf $(MLX42_PATH)/build

fclean: clean
	$(MAKE) -C $(LIBFT_PATH) fclean
	rm -f $(NAME)

re: fclean all

debug: CFLAGS += $(DEBUG)
debug: $(NAME)

.PHONY: all clean fclean re
