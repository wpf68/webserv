# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pwolff <pwolff@student.42mulhouse.fr>>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/10/23 11:26:41 by pwolff            #+#    #+#              #
#    Updated: 2022/10/23 11:26:41 by pwolff           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #



NAME		=	webserv

SRCS		= 	src/main.cpp src/request.cpp src/reponse.cpp \
				src/ft_formulaire_get_post.cpp
	
OBJS				= $(SRCS:.cpp=.o)
OBJS_DIR			= objs/
OBJECTS_PREFIXED 	= $(addprefix $(OBJS_DIR), $(OBJS))

CC		= g++
#CC		= clang++
FLAGS	= -Wall -Wextra -Werror -std=c++98

RED		=	\033[1;31m
YELLOW	=	\033[1;33m
RESET	=	\033[0m
CLEAR	=	\x1B[2J\x1B[H
NONE	=	\033[0;37m
WHITE	=	\033[1;37m
RED		=	\033[1;31m
GREEN	=	\033[1;32m
CYANE	=	\033[1;36m
ORANGE	=	\033[1,38m
YELLOW	=	\033[1;33m
BLUE	=	\033[1,34m
GREY	=	\033[1,37m
ROSE	=	\033[1,35m



$(OBJS_DIR)%.o : %.cpp
	@mkdir -p $(OBJS_DIR)
	@mkdir -p $(OBJS_DIR)src
	$(CC) $(CC_FLAGS) -c $< -o $@
#	@printf	"\033[2K\r${BLU}[BUILD]${RST} '$<' $(END)"
#	@mkdir -p $(OBJS_DIR)srcsi

#all: present $(NAME)
all: $(NAME)
	@echo "$(CYANE)----------done------------$(RESET)"

present:
	clear
	
$(NAME): $(SRCS) $(OBJECTS_PREFIXED)
	@$(CC) -o $(NAME) $(OBJECTS_PREFIXED) $(FLAGS)

clean:
	rm -rf $(OBJS_DIR)
	@echo "$(GREEN)Cleaned$(RESET)"

fclean: clean
	rm -rf $(NAME)

re: fclean all

test:	re
	./${NAME}

linux:	all
	valgrind ./${NAME}

.PHONY:		all clean fclean re test present linux