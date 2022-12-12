# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pwolff <pwolff@student.42mulhouse.fr>>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/10/23 11:26:41 by pwolff            #+#    #+#              #
#    Updated: 2022/11/22 21:09:00 by wilhelmfermey    ###   ########.fr        #
#                                                                              #
# **************************************************************************** #



NAME		=	webserv

SRCS		= 	src/main.cpp src/request.cpp src/reponse.cpp \
				src/ft_formulaire_get_post.cpp \
				src/created_body_reponse.cpp \
				src/init_content_type.cpp \
				src/parsing1.cpp src/parsing2.cpp src/parsing3.cpp \
				src/delete.cpp src/created_redir.cpp \
				src/created_page_code_HTML.cpp \
				src/init_code_HTTP.cpp src/upload_to_server.cpp \
				src/CGI.cpp src/utils.cpp
				

SRCS_BIN	=	Code_Source_Bin/main_CGI_c.cpp Code_Source_Bin/main_CGI_py.cpp

OBJS					= $(SRCS:.cpp=.o)
OBJS_BIN				= $(SRCS_BIN:.cpp=.o)
OBJS_DIR				= objs/
OBJECTS_PREFIXED 		= $(addprefix $(OBJS_DIR), $(OBJS))
#OBJECTS_PREFIXED_BIN 	= $(addprefix $(OBJS_DIR), $(OBJS_BIN))

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
	@printf "\033[0;33mGenerating webserv objects... %-38.38s\r" $@
	@$(CC) $(CC_FLAGS) -c $< -o $@
	
#	@printf	"\033[2K\r${BLU}[BUILD]${RST} '$<' $(END)"
#	@mkdir -p $(OBJS_DIR)srcsi

#all: present $(NAME)
all: $(NAME)
	@echo "\033[0m"
	@echo "\033[0m"
	@echo "$(CYANE)----------done------------  Enjoy :)$(RESET)"
	@echo "\033[0m"

present:
	clear
	
$(NAME): $(SRCS) $(SRCS_BIN) $(OBJECTS_PREFIXED)
	@$(CC) -o $(NAME) $(OBJECTS_PREFIXED) $(FLAGS)
	@$(CC) -o bin_c Code_Source_Bin/main_CGI_c.cpp $(FLAGS)
	@$(CC) -o bin_py Code_Source_Bin/main_CGI_py.cpp $(FLAGS)

clean:
	@echo "\033[0m"
	@echo "$(GREEN)Cleaned$(RESET)"
	@rm -rf $(OBJS_DIR)
	
	

fclean: clean
	@echo "\033[0;31mDeleting executable..."
	@rm -rf $(NAME)
	@rm -rf bin_py
	@rm -rf bin_c

re: fclean all

test:	re
	./${NAME} file.conf

linux:	all
	valgrind ./${NAME} file.conf

.PHONY:		all clean fclean re test present linux
