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
#include <arpa/inet.h>
#include <fcntl.h>
#include <fstream>

#define MY_PORT 8003
#define MY_IP "127.0.0.1"
#define NB_CONNECT 10

void    ft_error(std::string msg)
{
	std::cout << RED << msg << NONE << std::endl;
	exit (1);
}

std::string ft_read_file(std::string path)
{
	char		c;
	std::string	file;

	file = "";
	std::ifstream my_flux(path);
	if (!my_flux)
		ft_error("Error : open file");

	while (my_flux.get(c))
	{
		std::cout << c;
		file += c;
	}
	std::cout << "\n******************************\n" << std::endl;

	my_flux.close();
	return (file);
}

std::string ft_created_send(std::string path)
{
	std::string	create_send;
	std::string	file;

	file = ft_read_file(path);

	//"HTTP/1.1 200 OK\rContent-Length:24\rServer: webserv/1.0.0\r\n\nSalut Maxime et Wilhelm-"
	create_send = "HTTP/1.1 200 OK\r\n";
	create_send += "Content-Length: " + std::to_string(file.size()) + "\r\n";
//	create_send += "Content-Length:" + std::to_string(24) + "\r\n";
	create_send += "Content-Location: /\r\n";
	create_send += "Content-Type: text/html\r\n";
	create_send += "Date: Mon, 31 Oct 2022 17:15:12 GMT\r\n";
	create_send += "Server: webserv/42\r\n\n";
//	create_send += "Salut Maxime et Wilhelm-";
	create_send += file + "\r\n";
	std::cout << create_send << std::endl;


	return (create_send);
}

void	ft_adresse_IP(struct sockaddr_in &their_addr)
{
	char buff[INET6_ADDRSTRLEN] = {0};

	std::cout << GREEN "Adresse IP : " WHITE << (inet_ntop(their_addr.sin_family, (void*)&(their_addr.sin_addr), buff, INET6_ADDRSTRLEN)) << NONE ;
	std::cout << GREEN "  Port : " WHITE << ntohs(their_addr.sin_port) << NONE << std::endl;
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
	

	// send to client
	int	len;
	int	bytes_sent;


	fd_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // mettre 0 pour IPPROTO_TCP !
	if (fd_socket == -1)
		ft_error("Error : socket");
	std::cout << YELLOW "socket OK :: fd_socket = " WHITE << fd_socket << NONE << std::endl;

	server.sin_port = htons(MY_PORT);
	server.sin_family = AF_INET;
	// INADDR_ANY  ==> toutes les sources sont acceptés 127.x.x.x 
	server.sin_addr.s_addr = INADDR_ANY;  //server.sin_addr.s_addr = htonl(INADDR_ANY); // server.sin_addr.s_addr = inet_addr(MY_IP);
	bzero(&(server.sin_zero), 8);
	std::cout << GREEN "Port : " WHITE << ntohs(server.sin_port) << NONE << std::endl;  // test

	result = bind(fd_socket, (struct sockaddr *)&server, sizeof(struct sockaddr));  // voir les liens entre bind et connect  !!!!!!
	if (result == -1)
		ft_error("Error : bind");

	//result = listen(fd_socket, NB_CONNECT);
	result = listen(fd_socket, SOMAXCONN); // choix par le système du nbr de connexions appropriés
	if (result == -1)
		ft_error("Error : listen");

	//fcntl(fd_socket, F_SETFL, O_NONBLOCK); // non bloquant -- je pense que cela va permettre de parcourrir toute une liste de port en l'intégrant dans une bloucle

	std::cout << YELLOW "Server demarre sur le port " WHITE << ntohs(server.sin_port) << NONE << std::endl;

//	create_send = ft_created_send("HTML/test.html");
	create_send = ft_created_send("HTML/index.html");

	// infos client :
	for (;;)
	{
		int					new_fd;
		struct sockaddr_in	their_addr = {0}; // Informations d'adresse du client 
		socklen_t			sin_size;
		sin_size = sizeof(struct sockaddr_in);
		new_fd = accept(fd_socket, (struct sockaddr *)&their_addr, &sin_size);
		if (new_fd == -1)
			ft_error("Error : accept");
		std::cout << YELLOW "accept :: new_fd = " WHITE << new_fd << NONE << std::endl;

		ft_adresse_IP(their_addr);

		// -------------   reception des infos envoyés par le client  -----------------------
		char	buffer1[1024] = { 0 };
		int		iLastRecievedBufferLen = 0;

		iLastRecievedBufferLen = recv(new_fd, buffer1, 1023, 0);
		std::cout << WHITE "\nBuffer1 Client : \n" CYANE << buffer1 << NONE << std::endl;


		//  A formater correctement  //
		//bytes_sent = send(new_fd, buffer1, iLastRecievedBufferLen, 0);

	//	create_send = ft_created_send("HTML/index.html");
		bytes_sent = send(new_fd, create_send.c_str(), create_send.size(), 0); // passer 2h à pour trouver .cstr() !!!!!!!!!!!!

	//	bytes_sent = send(new_fd, "HTTP/1.1 200 OK\rContent-Length:24\rServer: webserv/1.0.0\r\n\nSalut Maxime et Wilhelm-", 87, 0);
		if (bytes_sent == -1)
			ft_error("Error : send");

		result = shutdown (new_fd, 2);
		if (result == -1)
			ft_error("Error : shutdown");
		std::cout << GREEN "Shutdown new_fd" NONE << std::endl;
		result = close(new_fd);
		if (result == -1)
			ft_error("Error : close socket");
		std::cout << GREEN "Close fd_socket" NONE << std::endl;
	}


	//  Deuxième envoi silmultané *************   ignoré !!!!!!!!!!!!!!!!!!!!//
//	bytes_sent = send(new_fd, "Salut Maxime et Wilhelm2", 24, 0);
//	if (bytes_sent == -1)
//		ft_error("Error : send");

	// -------------   reception des infos envoyés par le client après send ==> pas de nouvelles infos !!!  -----------------------
//	char	buffer[1024] = { 0 };
//	int		iLastRecievedBufferLen = 0;

//	iLastRecievedBufferLen = recv(new_fd, buffer, 1023, 0);
//	if (iLastRecievedBufferLen == -1)
//		ft_error("Error : revc");
//	std::cout << WHITE "\nBuffer Client : \n" CYANE << buffer << NONE << std::endl;

	
	result = close(fd_socket);
	if (result == -1)
		ft_error("Error : close socket");
	std::cout << GREEN "Close fd_socket" NONE << std::endl;
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
