# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ocalamar <ocalamar@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/10/07 11:05:52 by rchallie          #+#    #+#              #
#    Updated: 2021/03/16 15:41:46 by ocalamar         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = CUB

SRCS1 = main.c 					\
		parser/parser_map.c 	\
		utils/check_map.c		\
		utils/utils.c			\


# NAME = libft_with_gnl.a
LIBS =	libft_with_gnl.a \
		libmlx.dylib	\

CC = gcc

CC_FLAGS = -Wall -Wextra -Werror

MLX_FLAGS = -framework OpenGL -framework Appkit

1: 
		$(CC) $(SRCS1) $(LIBS) $(MLX_FLAGS) -o $(NAME) && ./$(NAME) ./maps/test.cub

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

