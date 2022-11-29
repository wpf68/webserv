/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pwolff <pwolff@student.42mulhouse.fr>>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/23 11:28:18 by pwolff            #+#    #+#             */
/*   Updated: 2022/11/22 21:14:36 by wilhelmfermey    ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.h"

t_server							firefox;
std::string							test_Sec_Fetch_Dest;
std::map<std::string, std::string>	var_content_type;

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

t_client	ft_init_firefox(int i, std::vector<s_parsing> parsing)
{
	int			result;
	t_client	datas;

	std::cout << CYANE << "SERVER N_" << WHITE << i << NONE << std::endl;

	datas.client_get_post = "";
	datas.client_path = "";
	datas.sec_fetch_dest = "";
	datas.buffer = "";
	datas.path_request = "";
	datas.content_type = "";
	
	datas.file_500_bis = "<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n    <meta charset=\"UTF-8\">\n    <meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\n    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n    <title>500</title>\n</head>\n<body>\n    <a href=\"/index.html\">\"Aller à la page d'accueil\" </a>\n    <h1 style=\"color: red;\">This page is temporarily unavailable - 500 - </h1>\n</body>\n</html>";
	datas.list_request_received = "";
	datas.server.sin_family = AF_INET;
	bzero(&(datas.server.sin_zero), 8);
	datas.fd_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (datas.fd_socket == -1)
		ft_error("Error : socket", &datas);
	std::cout << YELLOW "socket OK :: fd_socket = " WHITE << datas.fd_socket \
			<< NONE << std::endl;
	datas.create_send = "";
	datas.repertory.clear();
	datas.nb_cookie = 0;

// ****************************************************************************
//              Variables récupéraient du parsing t_parsing	parsing;
// ****************************************************************************
	datas.file_404 = parsing[i].error["404"];  //							error 404
	std::ifstream test_file_404(datas.file_404);
	if (!test_file_404)
	{
		std::cout << RED "Path file 404 no valid -- file 404 by default !!" NONE << std::endl;
		datas.file_404 = "HTML/404.html";
	}	
	else
		test_file_404.close();

	datas.file_500 = parsing[i].error["500"];
	std::ifstream test_file_500(datas.file_500);
	if (!test_file_500)
	{
		datas.file_500 = "HTML/500.html";
		std::cout << RED "Path file 500 no valid -- file 500 by default !!" NONE << std::endl;
	}
	else
		test_file_500.close();
	datas.server.sin_port = htons(stoi(parsing[i].my_port));  // 			port
//	datas.server.sin_addr.s_addr = parsing[i].my_ip.c_str();   //							addresse
//	inet_aton("127.0.0.1", &datas.server.sin_addr.s_addr);
//	datas.server.sin_addr.s_addr = INADDR_ANY;   //							addresse
	datas.server.sin_addr.s_addr = inet_addr(parsing[i].my_ip.c_str());   //							addresse

	datas.name_server = parsing[i].name_server; // 							name serveur
	datas.size = parsing[i].size; //										size
	datas.root = parsing[i].location[0].root; // 							root
//	if (datas.root == "")
//		datas.root = parsing[i].location[0].root;
//	datas.location = "";
	//datas.location = parsing[i].location[0].root; //								location

	datas.location = parsing[i].location;
	// for (int j = 0; j < parsing[i].location.size(); j++)
	// 	{
	// 		std::string val1 = parsing[i].location[j].req_client;
	// 		datas.location[j].req_client.push_back(parsing[i].location[j].req_client);
	// 		datas.location[j].root = parsing[i].location[j].root;
	// 		datas.location[j].path_index = parsing[i].location[j].path_index;
	// 		datas.location[j].dir_listing = parsing[i].location[j].dir_listing;
	// 		datas.location[j].methods = parsing[i].location[j].methods;

	// 		// std::map<std::string, std::string>::iterator it = parsing[i].location[j].redir.begin();
	// 		// for (; it != parsing[i].location[j].redir.end(); it++ )
	// 		// 		std::cout << it->first << ' ' << it->second << std::endl;
	// 		// std::cout << "Cgi :" <<std::endl;
	// 		// std::map<std::string, std::string>::iterator it2 = parsing[i].location[j].cgi.begin();
	// 		// for (; it2 != parsing[i].location[j].cgi.end(); it2++ )
	// 		// 	std::cout << it2->first << ' ' << it2->second << std::endl;
	// 		// std::cout << std::endl;
	// 	}


// ****************************************************************************
// ****************************************************************************
	datas.status = "200 " + datas.name_server + "_OK :)";
	std::cout << GREEN "Port : " WHITE << ntohs(datas.server.sin_port) \
			<< NONE << std::endl;  // test
	result = bind(datas.fd_socket, (struct sockaddr *)&(datas.server), \
			sizeof(struct sockaddr));  
	if (result == -1)
		ft_error("Error : bind", &datas);
	result = listen(datas.fd_socket, SOMAXCONN); 
	if (result == -1)
		ft_error("Error : listen", &datas);
	std::cout << YELLOW << datas.name_server << " go to port : " WHITE \
			<< ntohs(datas.server.sin_port) << NONE << std::endl;


	std::cout << RED "========== Verification des valeurs du serveur : " YELLOW << i << std::endl;
	std::cout << CYANE "datas.file_404 : " YELLOW << datas.file_404 << NONE << std::endl;
	std::cout << CYANE "datas.file_500 : " YELLOW << datas.file_500 << NONE << std::endl;
	std::cout << CYANE "datas.server.sin_port : " YELLOW << datas.server.sin_port << NONE << std::endl;
	std::cout << CYANE "datas.server.sin_addr.s_addr : " YELLOW << datas.server.sin_addr.s_addr << NONE << std::endl;
	std::cout << CYANE "datas.name_server : " YELLOW << datas.name_server << NONE << std::endl;
	std::cout << CYANE "datas.size : " YELLOW << datas.size << NONE << std::endl;
	std::cout << CYANE "datas.root : " YELLOW << datas.root << NONE << std::endl;
	for (int j = 0; j < datas.location.size(); j++)
		{
			std::cout << "//// Location " << j + 1 << " ////" << std::endl;
			std::cout << "Requête client : " << datas.location[j].req_client <<std::endl;
			std::cout << "Root : " << datas.location[j].root <<std::endl;
			std::cout << "Chemin vers Index : " << datas.location[j].path_index <<std::endl;
			std::cout << "Directory listing : " << datas.location[j].dir_listing <<std::endl;
			std::cout << "Méthode : " <<datas.location[j].methods <<std::endl;
			std::cout << "Redirections :" <<std::endl;

			// std::map<std::string, std::string>::iterator it = parsing[i].location[j].redir.begin();
			// for (; it != datas.location[j].redir.end(); it++ )
			// 		std::cout << it->first << ' ' << it->second << std::endl;
			// std::cout << "Cgi :" <<std::endl;
			// std::map<std::string, std::string>::iterator it2 = parsing[i].location[j].cgi.begin();
			// for (; it2 != datas.location[j].cgi.end(); it2++ )
			// 	std::cout << it2->first << ' ' << it2->second << std::endl;
			// std::cout << std::endl;
		}


	std::cout << CYANE "========================================\n" << std::endl;
	std::cout << std::endl;


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
		std::cout << GREEN "Close fd_socket : " 
			<< firefox.clients.at(i).fd_socket << YELLOW " - SERVER : " << firefox.clients.at(i).name_server << NONE << std::endl;
	}
	std::cout << "\b\b  " << std::endl;
	std::cout << RED << "Exit to CTRL-C" << NONE << std::endl;
	std::cout << GREEN << "You left webserve_42, bye :)" << NONE << std::endl;
	exit (0);
}

void	ft_read_struct(std::vector<s_parsing> parsing)
{
	for (int i = 0; i < parsing.size(); i++)
	{
		std::cout << "////////////////////// Server " << i + 1 << " //////////////////////" << std::endl;
		std::cout << "IP : " << parsing[i].my_ip <<std::endl;
		std::cout << "Port : " << parsing[i].my_port <<std::endl;
		std::cout << "Name : " << parsing[i].name_server <<std::endl;
		std::cout << "Size : " << parsing[i].size <<std::endl;
		std::cout << "Errors:" <<std::endl;
		std::map<std::string, std::string>::iterator it = parsing[i].error.begin();
		for (; it != parsing[i].error.end(); it++ )
			std::cout << it->first << ' ' << it->second << std::endl;
		std::cout <<std::endl;

		for (int j = 0; j < parsing[i].location.size(); j++)
		{
			std::cout << "//// Location " << j + 1 << " ////" << std::endl;
			std::cout << "Requête client : " << parsing[i].location[j].req_client <<std::endl;
			std::cout << "Root : " << parsing[i].location[j].root <<std::endl;
			std::cout << "Chemin vers Index : " << parsing[i].location[j].path_index <<std::endl;
			std::cout << "Directory listing : " << parsing[i].location[j].dir_listing <<std::endl;
			std::cout << "Méthode : " << parsing[i].location[j].methods <<std::endl;
			std::cout << "Redirections :" <<std::endl;

			std::map<std::string, std::string>::iterator it = parsing[i].location[j].redir.begin();
			for (; it != parsing[i].location[j].redir.end(); it++ )
					std::cout << it->first << ' ' << it->second << std::endl;
			std::cout << "Cgi :" <<std::endl;
			std::map<std::string, std::string>::iterator it2 = parsing[i].location[j].cgi.begin();
			for (; it2 != parsing[i].location[j].cgi.end(); it2++ )
				std::cout << it2->first << ' ' << it2->second << std::endl;
			std::cout << std::endl;
		}

		std::cout <<std::endl;
	}
}

int	ft_parsing(std::vector<s_parsing> &parsing, char *conf)
{
	std::string 				file;
	int							nb_serv;
	std::vector<int>			tab_len;
	std::vector<std::string>    servers; // .conf cut par server
	std::string					config;

	config = "./conf/";
	config.append(conf);
	file = ft_read_file2(config);

	//file = ft_read_file2("./src/file.conf");
	nb_serv = number_server(file, tab_len);
	cut_server(file, tab_len, servers);
	if (create_struct(parsing, nb_serv))
		return (1);
	find_ip(servers, parsing, nb_serv);
	find_port(servers, parsing, nb_serv);
	find_name(servers, parsing, nb_serv);
	find_size(servers, parsing, nb_serv);
	find_error(servers, parsing, nb_serv);
	number_server(servers, parsing);
	find_req_client(parsing);
	find_root(parsing);
	find_index(parsing);
	find_dir_listing(parsing);
	find_methods(parsing);
	find_redir(parsing);
	find_cgi(parsing);

//	ft_read_struct(parsing);

	return (nb_serv);
}


// static void	ft_copy_file(std::string source, std::string copy)
// {
// 	int			pid = 0;
// 	t_client*	datas;

// 	pid = fork();
// 	if (pid < 0)
// 		ft_error("Error fork", datas);
// 	if (pid == 0)
// 	{
// 		execlp("cp", "cp", source.c_str(), copy.c_str(), NULL);
// 	}
// 	else
// 	{
// 		std::cout << "********  end fork *************" << std::endl;
// 		waitpid(pid, NULL, 0);
// 	}
// }

static void	ft_copy_file(std::string source, std::string copy)
{
	std::string	file;
	char		c;

	file = "";
	std::ifstream my_flux(source);
	if (!my_flux)
	{
		std::cout << RED "file no found !!!" NONE << std::endl;
		exit (1);
	}
	while (my_flux.get(c))
		file += c;
	my_flux.close();
	std::ofstream	ofs(copy, std::ios::out | std::ios::app);
	if (!ofs)
	{
		std::cout << "Error file out" << std::endl;
		exit(0);
	}
	ofs << file << std::endl;
	ofs.close();
}


//----------------------  test directory
std::string	auto_index(const std::string dir_name, const std::string target)
{
	DIR				*dir;
	struct dirent	*fic;
	std::string		value;

	dir = opendir(dir_name.c_str());
	if (!dir)
		return "";
	value.assign("<html>\n<head>\n<meta charset=\"utf-8\">\n"
			"<title>Directory Listing</title>\n</head>\n<body>\n<h1>"
			+ dir_name + "</h1>\n<ul>");
	while ((fic = readdir(dir)) != NULL)
	{
		value.append("<li><a href=\"");
		value.append(target);
		if (value[value.size() - 1] != '/')
			value.append("/");
		value.append(fic->d_name);
		if(fic->d_type == DT_DIR)
			value.append("/");
		value.append("\"> ");
		value.append(fic->d_name);
		if(fic->d_type == DT_DIR)
			value.append("/");
		value.append("</a></li>\n");
	}
	value.append("</ul></body></html>");
	closedir(dir);
	return value;
}


int main(int argc, char **argv, char **env)
{
	argc;
	argv;
	(void)env;

	if (argc != 2)
		ft_error("Error nb argc !!\n", NULL);


	//test directory
	//std::string	directory;
	//directory = auto_index("../", "------test--------");



	std::string	dot[4] = {".  ", ".. ", "...", "...."};
	int			n_anim = 0;
	int			result;
	int			len;
	int			bytes_sent;
	struct timeval		timeout;
	fd_set 				readfds;
	fd_set				writefds;    /// ---- à test

	std::vector<s_parsing>      parsing;

	// init page test delete
	ft_copy_file("./HTML/site_3_form/Wilhelm.html", "./HTML/site_3_form/test_delete.html");

	firefox.nb_server = ft_parsing(parsing, argv[1]);
	ft_read_struct(parsing);

	test_Sec_Fetch_Dest = "audio-audioworklet-document-embed-empty-font-frame-iframe-image-";
	test_Sec_Fetch_Dest += "manifest-object-paintworklet-report-script-serviceworker-";
	test_Sec_Fetch_Dest += "sharedworker-style-track-video-worker-xslt";
	ft_init_content_type();

	// Verify values
	// for (std::map<std::string, std::string>::iterator itt = var_content_type.begin(); itt != var_content_type.end(); itt++)
	// 	std::cout << GREEN << itt->first << " : " YELLOW << itt->second << NONE << std::endl;
	//std::cout << GREEN "\ntest_Sec_Fetch_Dest : " YELLOW << test_Sec_Fetch_Dest << "\n" NONE << std::endl;

	// std::map<std::string, std::string>::iterator	itt;
	// itt = var_content_type.find("3gp");
	// std::cout << "3gp = " << itt->second << std::endl;	
	//-------------------


	//std::cout << "test director" << auto_index("../", "------test--------") << std::endl;
	
	for (int i = 0; i < firefox.nb_server; i++)
		firefox.clients.push_back(ft_init_firefox(i, parsing));
	std::cout << GREEN "\nNb de clients dans firefox : " \
			<< firefox.clients.size() << NONE << std::endl;
	
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
		FD_ZERO(&writefds);
		for (int i = 0; i < firefox.nb_server; i++)
			FD_SET(firefox.clients[i].fd_socket, &readfds);  // multi clients
			
		std::cout << "\rWaiting on a connection " << dot[n_anim++] << std::flush;
		if (n_anim == 4)
			n_anim = 0;
		result = select(firefox.nb_server + 3, &readfds, &writefds, NULL, &timeout);

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
				//	std::cout << CYANE << firefox.clients[i].buffer << NONE << std::endl;
					if (!firefox.clients[i].buffer.empty())
					{
						firefox.clients[i].create_send = ft_created_reponse(&firefox.clients[i]);							
						bytes_sent = send(new_fd, firefox.clients[i].create_send.c_str(), firefox.clients[i].create_send.size(), 0); 
						result = shutdown (new_fd, 2);
						if (result == -1)
							ft_error("Error : shutdown", &firefox.clients[i]);
					}
					
					firefox.clients[i].status = "200 " + firefox.clients[i].name_server + "_OK :)";
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
