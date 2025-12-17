# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cwong <cwong@student.hive.fi>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/10/20 11:01:30 by cwong             #+#    #+#              #
#    Updated: 2025/12/17 11:21:21 by nraatika         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC		:= cc

CFLAGS	:= -Wall -Wextra -Werror -O3 -march=native
DEBUG	:= -g

NAME	:= miniRT

SRC_DIR := src
SRCS	:= camera.c colors.c cylinder.c hooks.c lighting.c main.c \
		normal_vectors.c parse_objects.c parse_utils.c parsing.c \
		parsing_utils.c ray.c ray_makers.c ray_utils.c resize.c rotation.c \
		translation.c validate_scene.c validation_utils.c vector.c \
		vector_utils.c window.c 

OBJ_DIR := obj
OBJS    := $(patsubst %.c,$(OBJ_DIR)/%.o, $(SRCS))

LIBFT_PATH	:= ./libft
LIBFT		:= $(LIBFT_PATH)/libft.a

MLX_PATH	:= ./MLX/build/
MLX_NAME	:= libmlx42.a
MLX_BPATH	:= ./MLX/
MLX			:= $(MLX_PATH)$(MLX_NAME)

INC_DIR		:= includes
INC			:= -I./MLX/include/MLX42 -I./$(INC_DIR) -I$(LIBFT_PATH)
LIBS		:= -L$(MLX_PATH) -lmlx42 -lXext -lX11 -lm -lz -ldl -lglfw -pthread

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT) $(MLX)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBFT) $(MLX) -ldl -lglfw -lm

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

$(LIBFT):
	make -C $(LIBFT_PATH)

$(MLX):
		cd $(MLX_BPATH)	&& cmake -B build
		$(MAKE) -C $(MLX_PATH)

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
