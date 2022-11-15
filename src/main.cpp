/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pwolff <pwolff@student.42mulhouse.fr>>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/23 11:28:18 by pwolff            #+#    #+#             */
/*   Updated: 2022/10/23 11:28:18 by pwolff           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.h"

t_server	firefox;

void    ft_error(std::string msg, t_client *datas)
{
	(void) datas;
	std::cout << RED << msg << NONE << std::endl;
	exit (1);
}


void	ft_adresse_IP(struct sockaddr_in &their_addr)
{
	char buff[INET6_ADDRSTRLEN] = {0};

	std::cout << GREEN "Adresse IP : " WHITE << (inet_ntop(their_addr.sin_family,\
			(void*)&(their_addr.sin_addr), buff, INET6_ADDRSTRLEN)) << NONE ;
	std::cout << GREEN "  Port : " WHITE << ntohs(their_addr.sin_port) \
			<< NONE << std::endl;
}

t_client	ft_init_firefox(int i, t_parsing *parsing)
{
	int			result;
	t_client	datas;

	(void)parsing;

	datas.client_get_post = "";
	datas.client_path = "";
	datas.sec_fetch_dest = "";
	datas.buffer = "";
	datas.path_request = "";
	datas.content_type = "";
	datas.status = "200 webser42_OK :)";
	datas.file_500 = "<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n    <meta charset=\"UTF-8\">\n    <meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\n    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n    <title>500</title>\n</head>\n<body>\n    <a href=\"/index.html\">\"Aller à la page d'accueil\" </a>\n    <h1 style=\"color: red;\">This page is temporarily unavailable - 500 - </h1>\n</body>\n</html>";
	datas.list_request_received = "";
	datas.server.sin_family = AF_INET;
	bzero(&(datas.server.sin_zero), 8);
	datas.fd_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (datas.fd_socket == -1)
		ft_error("Error : socket", &datas);
	std::cout << YELLOW "socket OK :: fd_socket = " WHITE << datas.fd_socket \
			<< NONE << std::endl;
	datas.create_send = "";

// ****************************************************************************
//              Variables récupéraient du parsing t_parsing	parsing;
// ****************************************************************************
	datas.file_404 = "HTML/404.html";  //  parsing.at(i)->file_404;
	datas.server.sin_port = htons(MY_PORT + i);  // parsing.at(i)->my_port;
	datas.server.sin_addr.s_addr = INADDR_ANY;   // parsing.at(i)->my_ip;  (127.0.0.1)
	datas.name_server = "webserv/42"; // parsing.at(i)->name_server;
		//  test
		if (i == 1)
			datas.name_server = "webserv/4242424242";
	datas.root = "HTML"; // parsing.at(i)->root;
	datas.location = "/site_1"; // parsing.at(i)->location;
		//  test
		if (i == 1)
			datas.location = "/site_2";
// ****************************************************************************
// ****************************************************************************

	std::cout << GREEN "Port : " WHITE << ntohs(datas.server.sin_port) \
			<< NONE << std::endl;  // test
	result = bind(datas.fd_socket, (struct sockaddr *)&(datas.server), \
			sizeof(struct sockaddr));  
	if (result == -1)
		ft_error("Error : bind", &datas);
	result = listen(datas.fd_socket, SOMAXCONN); 
	if (result == -1)
		ft_error("Error : listen", &datas);
	std::cout << YELLOW "Server demarre sur le port " WHITE \
			<< ntohs(datas.server.sin_port) << NONE << std::endl;

	return (datas);
}

static void	ft_exit(int var)
{
	(void)var;
	int	result;

	std::cout << CYANE "\n=========  CLOSE SERVER  =======" << std::endl;
	for (int i = 0; i < firefox.nb_server; i++)
	{
		result = close(firefox.clients.at(i).fd_socket);
		if (result == -1)
			ft_error("Error : close socket", &firefox.clients.at(i));
		std::cout << GREEN "Close fd_socket : " \
				<< firefox.clients.at(i).fd_socket << NONE << std::endl;
	}
	std::cout << "\b\b  " << std::endl;
	std::cout << RED << "Exit to CTRL-C" << NONE << std::endl;
	std::cout << GREEN << "You left webserve_42, bye :)" << NONE << std::endl;
	exit (0);
}

static void	ft_parsing(t_parsing *parsing, std::string *conf)
{
	(void)conf;

	parsing->nb_server = 3;
}

int main(int argc, char **argv)
{
	(void)argc;
	(void)argv;

	std::string	dot[4] = {".  ", ".. ", "...", "...."};
	int			n_anim = 0;
	int			result;
	int			len;
	int			bytes_sent;
	struct timeval		timeout;
	fd_set 				readfds;

	std::string	conf = "default.conf";  // pour test, à get de argv

//	t_server	firefox;
	t_parsing	parsing;

	

	ft_parsing(&parsing, &conf);
	firefox.nb_server = parsing.nb_server;

	for (int i = 0; i < firefox.nb_server; i++)
		firefox.clients.push_back(ft_init_firefox(i, &parsing));
	std::cout << GREEN "Nb de clients dans firefox : " \
			<< firefox.clients.size() << NONE << std::endl;
	
//	t_client	datas;
	//ft_init(&datas, &firefox);
	
	char	buffer1[SIZE_RECV] = { 0 };
	int		iLastRecievedBufferLen = 0;
	int					new_fd;
	struct sockaddr_in	their_addr = {0};

	for (;;)
	{
		socklen_t	sin_size;

		signal(SIGINT, ft_exit);
		iLastRecievedBufferLen = 0;
		for (int i = 0; i < SIZE_RECV; i++)
			buffer1[i] = 0;
		their_addr.sin_port = 0;
		their_addr.sin_family = 0;	
		their_addr.sin_addr.s_addr = 0;
		bzero(&(their_addr.sin_zero), 8);
		sin_size = sizeof(struct sockaddr_in);		
		timeout.tv_sec = 1;   //  3h 					
		timeout.tv_usec = 0;
		FD_ZERO(&readfds);
		for (int i = 0; i < firefox.nb_server; i++)
			FD_SET(firefox.clients[i].fd_socket, &readfds);  // multi clients
		std::cout << "\rWaiting on a connection " << dot[n_anim++] << std::flush;
		if (n_anim == 4)
			n_anim = 0;
		result = select(firefox.nb_server + 3, &readfds, NULL, NULL, &timeout);

		if (result)
		{
			for (int i = 0; i < firefox.nb_server; i++)
			{
				if (FD_ISSET(firefox.clients[i].fd_socket, &readfds)) 
				{
					std::cout << GREEN "Port select : " WHITE << ntohs(firefox.clients[i].server.sin_port) << NONE << std::endl;  // test
					new_fd = accept(firefox.clients[i].fd_socket, (struct sockaddr *)&their_addr, &sin_size);
					if (new_fd == -1)
						ft_error("Error : accept", &firefox.clients[i]);
					std::cout << YELLOW "accept :: new_fd = " WHITE << new_fd << NONE << std::endl;

					ft_adresse_IP(their_addr);
					
					iLastRecievedBufferLen = recv(new_fd, buffer1, SIZE_RECV - 1, 0);
					firefox.clients[i].buffer = std::string(buffer1);
					std::cout << WHITE "\nBuffer1 Client : \n" CYANE << firefox.clients[i].buffer << NONE << std::endl;
					if (!firefox.clients[i].buffer.empty())
					{
						firefox.clients[i].create_send = ft_created_reponse(&firefox.clients[i]);							
						bytes_sent = send(new_fd, firefox.clients[i].create_send.c_str(), firefox.clients[i].create_send.size(), 0); 
						result = shutdown (new_fd, 2);
						if (result == -1)
							ft_error("Error : shutdown", &firefox.clients[i]);
					}
					
					firefox.clients[i].status = "200 webser42_OK :)";
					if (bytes_sent == -1)
						ft_error("Error : send", &firefox.clients[i]);
					
					std::cout << GREEN "Shutdown new_fd" NONE << std::endl;
					result = close(new_fd);
					if (result == -1)
						ft_error("Error : close new_fd", &firefox.clients[i]);
					std::cout << GREEN "Close new_fd" NONE << std::endl;
				}
			}
		}
							

	}	
	return (0);
}


/*         *********    donne IP adresse d'un Host name   **********    */
/*         *********    google.com   ==>   216.58.213.78  **********    */
/*
	#include <stdio.h> 
	#include <stdlib.h> 
	#include <errno.h> 
	#include <netdb.h> 
	#include <sys/types.h>
	#include <netinet/in.h> 

	#include <sys/socket.h>
	#include <arpa/inet.h>

	int main(int argc, char *argv[])
	{
		struct hostent *h;


		if (argc != 2) {  // Vérification d'erreurs de la ligne de commande 
			fprintf(stderr,"usage: getip adresse\n");
			exit(1);
		}

		if ((h=gethostbyname(argv[1])) == NULL) {  // récupérer infos de l'hôte 
			herror("gethostbyname");
			exit(1);
		}

		printf("Host name  : %s\n", h->h_name);
		printf("IP adresse : %s\n",inet_ntoa(*((struct in_addr *)h->h_addr)));

		char *ip;

		ip = (inet_ntoa(*((struct in_addr *)h->h_addr)));
		printf("adresse : %s\n", ip);

		struct sockaddr_in  ina;
		ina.sin_addr.s_addr = inet_addr(ip);
		printf("IP long : %d\n", ina.sin_addr.s_addr);
		printf("IP long to ASCII : %s\n", inet_ntoa(ina.sin_addr));

		return 0;
	}
*/
