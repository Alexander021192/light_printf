# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ocalamar <ocalamar@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/10/07 11:05:52 by rchallie          #+#    #+#              #
#    Updated: 2021/03/07 11:11:06 by ocalamar         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS1 = mlx_mac.c 			\
		parser_map.c 	\
		utils.c			\


SRCS2 = mlx_mac.c 			\
		parser_map.c 	\
		utils_copy.c			\


# NAME = libft_with_gnl.a

CC = gcc

CC_FLAGS = -Wall -Wextra -Werror

MLX_FLAGS = -framework OpenGL -framework Appkit

1: 
		$(CC) $(SRCS1) libft_with_gnl.a libmlx.dylib -framework OpenGL -framework Appkit	&& ./a.out

2:
		$(CC) $(SRCS2) libft_with_gnl.a libmlx.dylib -framework OpenGL -framework Appkit	&& ./a.out

# $(OBJS_DIR)%.o : %.c
# 	@mkdir -p $(OBJS_DIR)
# 	@echo "Compiling: $<"
# 	@gcc $(CC_FLAGS) -c $< -o $@

# $(NAME): $(OBJECTS_PREFIXED)
# 	@ar r $(NAME) $(OBJECTS_PREFIXED)
# 	@echo "Libft Done !"

# all: $(NAME)

clean:
	rm -f *.o
	rm -f a.out

fclean: clean
	rm -f $(NAME)
	
re: fclean all

