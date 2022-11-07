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



void    ft_error(std::string msg, t_parsing *datas)
{
	(void) datas;
	std::cout << RED << msg << NONE << std::endl;
	exit (1);
}




void	ft_adresse_IP(struct sockaddr_in &their_addr)
{
	char buff[INET6_ADDRSTRLEN] = {0};

	std::cout << GREEN "Adresse IP : " WHITE << (inet_ntop(their_addr.sin_family, (void*)&(their_addr.sin_addr), buff, INET6_ADDRSTRLEN)) << NONE ;
	std::cout << GREEN "  Port : " WHITE << ntohs(their_addr.sin_port) << NONE << std::endl;
}

void	ft_init(t_parsing *datas)
{
	datas->client_get_post = "";
	datas->client_path = "";
	datas->sec_fetch_dest = "";

	datas->content_type = "";
	datas->status = "200 webser42_OK :)";
	datas->file_500 = "<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n    <meta charset=\"UTF-8\">\n    <meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\n    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n    <title>500</title>\n</head>\n<body>\n    <a href=\"./index.html\">\"Aller à la page d'accueil\" </a>\n    <h1 style=\"color: red;\">This page is temporarily unavailable</h1>\n</body>\n</html>";
	datas->list_request_received = "";
}

static void	ft_exit(int var)
{
	(void)var;
	std::cout << "\b\b  " << std::endl;
	std::cout << RED << "Exit to CTRL-C" << NONE << std::endl;
	std::cout << GREEN << "You left webserve_42, bye :)" << NONE << std::endl;
	exit (0);
}


int main()
{
	//std::cout << ft_read_file("HTML/index.html");
	
	//return (0);
	// infos server
	int					fd_socket;
	struct sockaddr_in  server;
	int					result;
	std::string			create_send;
	t_parsing			datas;
	

	// send to client
	int	len;
	int	bytes_sent;

	ft_init(&datas);
	fd_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // mettre 0 pour IPPROTO_TCP !
	if (fd_socket == -1)
		ft_error("Error : socket", &datas);
	std::cout << YELLOW "socket OK :: fd_socket = " WHITE << fd_socket << NONE << std::endl;

	server.sin_port = htons(MY_PORT);
	server.sin_family = AF_INET;
	// INADDR_ANY  ==> toutes les sources sont acceptés 127.x.x.x 
	server.sin_addr.s_addr = INADDR_ANY;  //server.sin_addr.s_addr = htonl(INADDR_ANY); // server.sin_addr.s_addr = inet_addr(MY_IP);
	bzero(&(server.sin_zero), 8);
	std::cout << GREEN "Port : " WHITE << ntohs(server.sin_port) << NONE << std::endl;  // test

	result = bind(fd_socket, (struct sockaddr *)&server, sizeof(struct sockaddr));  // voir les liens entre bind et connect  !!!!!!
	if (result == -1)
		ft_error("Error : bind", &datas);


	result = listen(fd_socket, SOMAXCONN); // choix par le système du nbr de connexions appropriés
	if (result == -1)
		ft_error("Error : listen", &datas);

	//fcntl(fd_socket, F_SETFL, O_NONBLOCK); // non bloquant -- je pense que cela va permettre de parcourrir toute une liste de port en l'intégrant dans une bloucle

	std::cout << YELLOW "Server demarre sur le port " WHITE << ntohs(server.sin_port) << NONE << std::endl;
	char	buffer1[SIZE_RECV] = { 0 };
	int		iLastRecievedBufferLen = 0;
	int					new_fd;
	struct sockaddr_in	their_addr = {0};

	for (;;)
	{
		iLastRecievedBufferLen = 0;
		for (int i = 0; i < SIZE_RECV; i++)
			buffer1[i] = 0;

		their_addr.sin_port = 0;
		their_addr.sin_family = 0;	
		their_addr.sin_addr.s_addr = 0;
		bzero(&(their_addr.sin_zero), 8);
		

		signal(SIGINT, ft_exit);
		socklen_t			sin_size;
		sin_size = sizeof(struct sockaddr_in);
		new_fd = accept(fd_socket, (struct sockaddr *)&their_addr, &sin_size);
		if (new_fd == -1)
			ft_error("Error : accept", &datas);
		std::cout << YELLOW "accept :: new_fd = " WHITE << new_fd << NONE << std::endl;

		ft_adresse_IP(their_addr);
		
		iLastRecievedBufferLen = recv(new_fd, buffer1, SIZE_RECV - 1, 0);
		datas.buffer = std::string(buffer1);
		std::cout << WHITE "\nBuffer1 Client : \n" CYANE << datas.buffer << NONE << std::endl;
		//std::cout << WHITE "\nBuffer1 Client : \n" CYANE << buffer1 << NONE << std::endl;
		if (!datas.buffer.empty())
		{
			create_send = ft_created_reponse(&datas);  ////-----------------------------------------------
			bytes_sent = send(new_fd, create_send.c_str(), create_send.size(), 0); 
		}
		
		datas.status = "200 webser42_OK :)";
		if (bytes_sent == -1)
			ft_error("Error : send", &datas);
		result = shutdown (new_fd, 2);
		if (result == -1)
			ft_error("Error : shutdown", &datas);
		std::cout << GREEN "Shutdown new_fd" NONE << std::endl;
		result = close(new_fd);
		if (result == -1)
			ft_error("Error : close new_fd", &datas);
		std::cout << GREEN "Close new_fd" NONE << std::endl;
	}	
	result = close(fd_socket);
	if (result == -1)
		ft_error("Error : close socket", &datas);
	std::cout << GREEN "Close fd_socket" NONE << std::endl;
	return (0);
}


/*   ***** Animation wait   *********     */
/*
	std::string	dot[3] = {".  ", ".. ", "..."};
	int			n = 0;
	for(;;)
	{
		std::cout << "\rWaiting on a connection" << dot[n++] << std::flush;
		if (n == 3)
			n = 0;

	}
*/


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
