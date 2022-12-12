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
extern std::map<std::string, std::string>   var_content_code;

int ft_test_request_exist(t_client *datas, std::string &path_request)
{
	std::size_t	position_request;
	std::string type;
	int			status;

	status = 0;	
	type = get_reponse(datas->client_path, ".", "\0");
	if (type == "html")
	{
		if (datas->list_request_received == path_request)
		{
			// datas->status = "204 " + var_content_code["204"] + " " + datas->name_server;
			status = 1;
			datas->status = "304 " + var_content_code["304"] + " " + datas->name_server;
			datas->list_request_received = "";
		}
		else
			datas->list_request_received = path_request;
	}
	return (status);
}

static void ft_type_content_type(t_client *datas)
{
	std::string type_image;

	type_image = get_reponse(datas->client_path, ".", "\0");
	std::string&	secound = var_content_type[type_image];
	if (secound == "")
		secound = "text/html; charset=UTF-8";
	datas->content_type = "Content-Type: " + secound + "\r\n";
}

static void ft_test_content_type(t_client *datas)
{
	std::string temp;

	temp = datas->sec_fetch_dest + "-";
	if (test_Sec_Fetch_Dest.find(temp) != std::string::npos)
		ft_type_content_type(datas);
	else
		datas->content_type = "Content-Type: \r\n";
}

static void  ft_get_content_type (t_client *datas)
{
	datas->content_type = "Sec-Fetch-Dest: ";
	datas->sec_fetch_dest = get_reponse(datas->buffer, datas->content_type, "\r");
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

static std::string	ft_Last_Modified(void)
{
	time_t rawtime;
  	struct tm * timeinfo;
  	char buffer [80];

	time (&rawtime);
	timeinfo = gmtime (&rawtime);
	strftime (buffer,80,"Last-Modified: %a, %d %b %Y 00:42:42 %Z",timeinfo);
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
	body_reponse = ft_created_body_reponse(datas);
	if (datas->buffer.find("GET /redirection.html HTTP/1.1") != std::string::npos)
		datas->status = "301 " + var_content_code["301"] + " " + datas->name_server;
	create_send = "HTTP/1.1 " + datas->status + "\r\n";
	create_send += "Content-Length: " + std::to_string(body_reponse.size()) + "\r\n";
	if (datas->status.find("301 ") != std::string::npos && datas->path_request.find("www.") != std::string::npos)
		create_send += "Location: http://" + datas->path_request +"\r\n";
	create_send += "Content-Location: " + datas->path_request +"\r\n";
	if (datas->status.find("500 ") != std::string::npos)
		create_send += "Retry-After: Fri, 25 Dec 2042 00:42:42 GMT \r\n";
	
	create_send += "Cache-Control: max-age=10\r\n";
	create_send += "Content-Language: en-US,fr-FR \r\n";
	ft_get_cookie(datas);
	create_send += "Set-Cookie: Welcome to " + std::to_string(datas->nb_cookie) + " visite(s)\r\n";
	ft_get_content_type(datas);
	create_send += datas->content_type;
	create_send += ft_Last_Modified() + "\r\n";
	create_send += ft_date() + "\r\n";
	create_send += "Server: " + datas->name_server + "\r\n\n";
	std::cout << CYANE << datas->buffer << NONE << std::endl;
	std::cout << YELLOW << create_send << NONE << std::endl;
	create_send += body_reponse + "\r\n";
	return (create_send);
}

