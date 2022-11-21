/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pwolff <pwolff@student.42mulhouse.fr>>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/03 09:40:49 by pwolff            #+#    #+#             */
/*   Updated: 2022/11/03 09:40:49 by pwolff           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_H
# define WEBSERV_H

# ifndef COLOR
#  define COLOR
#  define CLEAR		"\x1B[2J\x1B[H"
#  define NONE		"\033[0;37m"
#  define WHITE		"\033[1;37m"
#  define RED		"\033[1;31m"
#  define GREEN		"\033[1;32m"
#  define CYANE		"\033[1;36m"
#  define ORANGE	"\033[1,38m"
#  define YELLOW	"\033[1;33m"
#  define BLUE		"\033[1,34m"
#  define GREY		"\033[1,37m"
#  define ROSE		"\033[1,35m"
#  define UNDER		"\033[4m"
#  define END		"\033[0m"
# endif

#include <sys/types.h> 
#include <sys/socket.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <netinet/ip.h>
#include <iostream>
#include <cstring>
#include <string>
#include <arpa/inet.h>
#include <fcntl.h>
#include <fstream>
#include <signal.h>
#include <ctime>

// pour multi client :
#include <sys/time.h>
#include <sys/select.h>
#include <vector>
#include <sstream>

#define MY_PORT 8003
#define MY_PORT2 8002  //  test multi clients
#define MY_IP "127.0.0.1"
#define NB_CONNECT 10
#define SIZE_RECV  104424

typedef struct s_client
{
	// client
	std::string client_get_post;
	std::string client_path;
	std::string sec_fetch_dest;
	std::string buffer;
	std::string path_request;

	// serveur
	std::string status;
	std::string content_type;
	std::string file_500;
	std::string name_server;
	std::string list_request_received;
	std::string	root;
	std::string location;
	std::vector<std::string>	repertory;
	int			nb_cookie;

	int			fd_socket;
	std::string	create_send;

	// parsing
	std::string 		file_404;
	struct sockaddr_in  server;


}   t_client;

typedef struct s_server
{
	std::vector<t_client>	clients;
	int 					nb_server;
}   t_server;

typedef struct s_parsing
{
	std::vector<std::string>	file_404;
	std::vector<int>            my_port;
	std::vector<std::string>    my_ip;
	std::vector<std::string>    name_server;
	std::vector<std::string>    root;
	std::vector<std::string>    location;
	
	
	std::vector<std::string>    methods;
	std::vector<std::string>    cgi_extension;
	std::vector<std::string>    cgi_bin;

	int                         nb_server; 
}   t_parsing;

// std::string	test_Sec_Fetch_Dest;
// t_server	firefox;

void	ft_adresse_IP(struct sockaddr_in &their_addr);
void    ft_error(std::string msg, t_client *datas);

// std::string get_reponse_space(std::string &buffer, std::string &request);
// std::string get_reponse_end_line(std::string &buffer, std::string &request);
// std::string get_reponse_image_end_line(std::string &buffer, std::string request);


std::string get_reponse(std::string &buffer, std::string request, std::string endl);
std::string ft_created_reponse(t_client *datas);
std::string ft_formulaire_get_post(std::string &temp, t_client *datas);
std::string ft_read_file(t_client *datas);
std::string ft_parsing_form(std::string temp, t_client *datas);
int 		ft_test_request_exist(t_client *datas, std::string &path_request);
std::string ft_created_body_reponse(t_client *datas);


#endif