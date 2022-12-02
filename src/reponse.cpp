/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reponse.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pwolff <pwolff@student.42mulhouse.fr>>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 13:03:52 by pwolff            #+#    #+#             */
/*   Updated: 2022/11/04 13:03:52 by pwolff           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.h"

extern std::string							test_Sec_Fetch_Dest;
extern std::map<std::string, std::string>	var_content_type;

int ft_test_request_exist(t_client *datas, std::string &path_request)
{
	std::size_t	position_request;
	std::string type;
	int			status;

	status = 0;	
	// std::cout << WHITE "*******  TEST REQUEST EXIST *******" NONE << std::endl;
	// std::cout << "New request : " << path_request << std::endl;
	// std::cout << "List request already : " << datas->list_request_received \
			<< std::endl;
	type = get_reponse(datas->client_path, ".", "\0");
//	std::cout << RED "type : " YELLOW << type << NONE "\n" << std::endl;
	if (type == "html")
	{
		if (datas->list_request_received == path_request)
		{
			datas->status = "204 " + datas->name_server + " already sent :)";
			status = 1;
		}
		else
			datas->list_request_received = path_request;
	}
	// std::cout << "Status : " << datas->status << std::endl;
	// std::cout << "New list request : " << datas->list_request_received << std::endl;
	// std::cout << WHITE "************************************" NONE << std::endl;
	return (status);
}

static void ft_type_content_type(t_client *datas)
{
	std::string type_image;

//	type_image = ".";
//	datas->client_path;
//	std::cout << "\nSearch extension image to : " << datas->client_path << std::endl;
//	type_image = get_reponse_image_end_line(datas->client_path, type_image);
	type_image = get_reponse(datas->client_path, ".", "\0");



//----------------  trouver la pair  ---------------------------

//	std::cout << RED "type_image : " YELLOW << type_image << NONE "\n" << std::endl;
	std::string&	secound = var_content_type[type_image];
	if (secound == "")
		secound = "text/html; charset=UTF-8";
	datas->content_type = "Content-Type: " + secound + "\r\n";
}

static void ft_test_content_type(t_client *datas)
{
	std::string temp;

	// test sec_fetch_dest Protocole HTTP
	temp = datas->sec_fetch_dest + "-";
	if (test_Sec_Fetch_Dest.find(temp) != std::string::npos)
	{
	//	std::cout << GREEN "Request " << datas->sec_fetch_dest << " accepted" NONE << std::endl;
		ft_type_content_type(datas);
	}
	else
	{
	//	std::cout << RED "Request " << datas->sec_fetch_dest << " no Protocol !!!" NONE << std::endl;
		datas->content_type = "Content-Type: \r\n";
	}
		
}

static void  ft_get_content_type (t_client *datas)
{
	datas->content_type = "Sec-Fetch-Dest: ";
//	datas->sec_fetch_dest = get_reponse_end_line(datas->buffer, datas->content_type);
	datas->sec_fetch_dest = get_reponse(datas->buffer, datas->content_type, "\r");

//	std::cout << RED "Sec-Fetch-Dest Client : " YELLOW << datas->sec_fetch_dest \
			<< NONE "\n" << std::endl;
	ft_test_content_type(datas);
}

static std::string	ft_date(void)
{
	time_t rawtime;
  	struct tm * timeinfo;
  	char buffer [80];

	time (&rawtime);
	timeinfo = gmtime (&rawtime);
	strftime (buffer,80,"Date: %a, %d %b %Y %H:%M:%S %Z",timeinfo);

	return (std::string(buffer));
}

static void	ft_get_cookie(t_client *datas)
{
	std::string	val;

	if (datas->buffer.find("Cookie") == std::string::npos)
	{
		datas->nb_cookie = 1;
		return;
	}
	val = get_reponse(datas->buffer, "Cookie: Welcome to ", " ");
	std::istringstream iss(val);
	iss >> datas->nb_cookie;
	datas->nb_cookie += 1;
}

std::string ft_created_reponse(t_client *datas)
{
	std::string	create_send;
	std::string	body_reponse;
	std::string content_type;

	body_reponse = "";

//	if (datas->buffer.find("Sec-Fetch-Mode: navigate") != std::string::npos 
//			&& datas->buffer.find("Sec-Fetch-Dest: document") != std::string::npos)
	body_reponse = ft_created_body_reponse(datas);
	if (datas->buffer.find("GET /redirection.html HTTP/1.1") != std::string::npos)
		datas->status = "301 redirection site";
		
	create_send = "HTTP/1.1 " + datas->status + "\r\n";
	create_send += "Content-Length: " + std::to_string(body_reponse.size()) + "\r\n";
	create_send += "Content-Location: " + datas->path_request +"\r\n";
	// cookies
	ft_get_cookie(datas);
	create_send += "Set-Cookie: Welcome to " + std::to_string(datas->nb_cookie) + " visite(s)\r\n";  // x à chercher dans l'ancienne valeur
	ft_get_content_type(datas);
	create_send += datas->content_type;
	create_send += ft_date() + "\r\n";
	create_send += "Server: " + datas->name_server + "\r\n\n";
	std::cout << CYANE << datas->buffer << NONE << std::endl;
	std::cout << YELLOW << create_send << NONE << std::endl;
	create_send += body_reponse + "\r\n";
	return (create_send);
}

