/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pwolff <pwolff@student.42mulhouse.fr>>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/03 09:40:49 by pwolff            #+#    #+#             */
/*   Updated: 2022/11/22 21:05:52 by wilhelmfermey    ###   ########.fr       */
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
#include <map>
#include <algorithm>
#include <sys/stat.h>
#include <sys/wait.h>


// pour multi client :
#include <sys/time.h>
#include <sys/select.h>
#include <vector>
#include <sstream>
#include <sys/types.h>
#include <dirent.h>
#include <filesystem>


#define MY_PORT 8003
#define MY_PORT2 8002  //  test multi clients
#define MY_IP "127.0.0.1"
#define NB_CONNECT 10
#define SIZE_RECV  104424

//////////// STRUCTURE POUR CHAQUE LOCATION(parsing) //////////
struct s_location
{
	std::string							req_client;
    std::string							root;
    std::string							path_index;
	std::string							dir_stock;
	std::string							dir_listing;
    std::string							methods;
	std::map<std::string, std::string> 	redir;
	std::map<std::string, std::string> 	cgi;

};

//////////// STRUCTURE PRINCIPALE(parsing) /////////////
struct s_parsing
{
    std::string							my_port;
    std::string							my_ip;
    std::string							name_server;
	int									size;
	int									nbr_serv;
	std::map<std::string, std::string> 	error;
	std::vector<s_location>      		location;

	std::vector<std::string>      		locations; // aide pour parsing (ne pas utiliser).
};

typedef struct s_client
{
	// client
	std::string client_get_post;
	std::string client_path;
	std::string sec_fetch_dest;
	std::string buffer;
	std::string path_request;

	// serveur
	std::string 				status;
	std::string					content_type;
	std::string 				file_500;
	std::string 				file_500_bis;
	std::string 				name_server;
	int							size;
	std::string 				list_request_received;
	std::string					root;
	std::string					index;
	std::string					root_path;
	std::vector<std::string>	repertory;
	int							nb_cookie;
	std::vector<s_location>		location;
	int							fd_socket;
	std::string					create_send;

	// parsing
	std::string 				file_404;
	struct sockaddr_in 			server;
	std::string					dir_stock;

	char						**env;

}   t_client;

typedef struct s_server
{
	std::vector<t_client>	clients;
	int 					nb_server;
}   t_server;

void		ft_adresse_IP(struct sockaddr_in &their_addr);
void  		ft_error(std::string msg, t_client *datas);

std::string get_reponse(std::string &buffer, std::string request, std::string endl);
std::string ft_created_reponse(t_client *datas);
std::string ft_formulaire_get_post(std::string &temp, t_client *datas);
std::string ft_read_file(t_client *datas);
std::string ft_parsing_form(std::string temp, t_client *datas);
int 		ft_test_request_exist(t_client *datas, std::string &path_request);
std::string ft_created_body_reponse(t_client *datas);
void		ft_init_content_type(void);
void		ft_delete(std::string path, t_client *datas);
std::string	auto_index(const std::string dir_name, const std::string target);
void  		ft_error(std::string msg, t_client *datas);
std::string	ft_redir(std::string redir, t_client *datas);
std::string	ft_code_HTTP(std::string code, std::string message, t_client *datas);
void 		ft_init_code_type(void);
bool		ft_upload_to_server(std::string temp, t_client *datas_client);
std::string	ft_CGI_py(std::string test_path_valide, t_client *datas);
int 		exist_path(std::string path, t_client *datas);
std::string CGI_py(std::string test_path_valide, t_client *datas);
std::string CGI_c(std::string test_path_valide, t_client *datas);


//////////////////// PARSING_1 ///////////////////
std::string	ft_read_file2(std::string file_path);
int 		number_server(std::string &file, std::vector<int> &tab_len);
void    	cut_server(std::string &file, std::vector<int> &tab_len,     std::vector<std::string> &servers);

//////////////////// PARSING_2 ///////////////////
int		create_struct(std::vector<s_parsing> &parsing, int nb_serv);
void    find_ip(std::vector<std::string> servers, std::vector<s_parsing> &parsing, int nb_serv);
void    find_port(std::vector<std::string> servers, std::vector<s_parsing> &parsing, int nb_serv);
void    find_name(std::vector<std::string> servers, std::vector<s_parsing> &parsing, int nb_serv);
void    find_size(std::vector<std::string> servers, std::vector<s_parsing> &parsing, int nb_serv);
void    find_error(std::vector<std::string> servers, std::vector<s_parsing> &parsing, int nb_serv);

//////////////////// PARSING_3 ///////////////////
void 	number_server(std::vector<std::string> servers, std::vector<s_parsing> &parsing);
void    find_req_client(std::vector<s_parsing> &parsing);
void    find_root(std::vector<s_parsing> &parsing);
void    find_index(std::vector<s_parsing> &parsing);
void    find_dir_listing(std::vector<s_parsing> &parsing);
void	find_dir_stock(std::vector<s_parsing> &parsing);
void    find_methods(std::vector<s_parsing> &parsing);
void    find_redir(std::vector<s_parsing> &parsing);
void    find_cgi(std::vector<s_parsing> &parsing);

#endif
