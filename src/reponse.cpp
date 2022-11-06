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

static std::string ft_read_file(t_parsing *datas)
{
	char		c;
	std::string	file;

	file = "";
	std::ifstream my_flux(datas->client_path);
	if (!my_flux)
	{
		//ft_error("Error : open file", datas);
		datas->status = "404 webser42_error_page :(";
		// datas->client_path = "HTML/site_1/404.html";
		// std::ifstream my_flux("HTML/site_1/404.html");
		// if (!my_flux)
		// {
		//     datas->status = "500";
		//     return (datas->file_500);
		// }
		return (file);
	}
	while (my_flux.get(c))
		file += c;
	my_flux.close();
	return (file);
}

static void ft_test_request_exist(t_parsing *datas, std::string &path_request)
{
	std::size_t	position_request;
	std::string type;

	
	std::cout << WHITE "***********  TEST REQUEST EXIST *****************" NONE << std::endl;
	std::cout << "New request : " << path_request << std::endl;
	std::cout << "List request already : " << datas->list_request_received << std::endl;



	type = ".";
	type = get_reponse_image_end_line(datas->client_path, type);
	std::cout << RED "type : " YELLOW << type << NONE "\n" << std::endl;
	if (type == "html")
	{
		if (datas->list_request_received == path_request)  // request already recieve
		{
			datas->status = "304 webserv_42_already sent :)";
		}
		else
		{
			datas->list_request_received = path_request;
		}
	}


	std::cout << "Status : " << datas->status << std::endl;
	std::cout << "New list request : " << datas->list_request_received << std::endl;
	std::cout << WHITE "*************************************************" NONE << std::endl;
}

static std::string ft_created_body_reponse(t_parsing *datas)
{
	std::string	file;    
	std::string	path_request;

	path_request = "GET";
	path_request = get_reponse_space(datas->buffer, path_request);
	
	

	std::cout << RED "PATH Request Client : " YELLOW << path_request << NONE "\n" << std::endl;
	datas->client_path = "HTML/site_1";
	if (path_request == "/")
		datas->client_path += "/index.html";
	else
		datas->client_path += path_request;
	std::cout << RED "PATH Reponse Server : " YELLOW << datas->client_path << NONE "\n" << std::endl;

	// test request deja demandé ou non
	ft_test_request_exist(datas, datas->client_path);


	file = ft_read_file(datas);
	return (file);
}

static void ft_type_image(t_parsing *datas)
{
	std::string type_image;

	type_image = ".";
	std::cout << "\nSearch extension image de : " << datas->client_path << std::endl;
	type_image = get_reponse_image_end_line(datas->client_path, type_image);
	std::cout << RED "type_image : " YELLOW << type_image << NONE "\n" << std::endl;
	if (type_image == "jpg" || type_image == "jpeg")
		datas->content_type = "Content-Type: image/jpeg\r\n";
	else if (type_image == "png")
		datas->content_type = "Content-Type: image/png\r\n";
	else if (type_image == "pdf")
		datas->content_type = "Content-Type: application/pdf\r\n";
	else if (type_image == "mp4")
		datas->content_type = "Content-Type: video/mp4\r\n";
	else
		datas->content_type = "Content-Type: \r\n";

}

static void ft_test_content_type(t_parsing *datas)
{
	std::string reponse;

	if (datas->sec_fetch_dest == "document")
		datas->content_type = "Content-Type: text/html; charset=UTF-8\r\n";
	else if (datas->sec_fetch_dest == "image" || datas->sec_fetch_dest == "iframe" || datas->sec_fetch_dest == "video")
		ft_type_image(datas);
	else
		datas->content_type = "Content-Type: \r\n";
}

static void  ft_get_content_type (t_parsing *datas)
{
  //  std::string content_type;

	datas->content_type = "Sec-Fetch-Dest:";
	datas->sec_fetch_dest = get_reponse_end_line(datas->buffer, datas->content_type);
   // content_type += "\r\n";
	std::cout << RED "Sec-Fetch-Dest Client : " YELLOW << datas->sec_fetch_dest << NONE "\n" << std::endl;
	ft_test_content_type(datas);
}

std::string ft_created_reponse(t_parsing *datas)
{
	std::string	create_send;
	std::string	body_reponse;
	std::string content_type;

	body_reponse = ft_created_body_reponse(datas);

	//"HTTP/1.1 200 OK\rContent-Length:24\rServer: webserv/1.0.0\r\n\nSalut Maxime et Wilhelm-"
	create_send = "HTTP/1.1 " + datas->status + "\r\n";
	create_send += "Content-Length: " + std::to_string(body_reponse.size()) + "\r\n";
//	create_send += "Content-Length:" + std::to_string(24) + "\r\n";
	create_send += "Content-Location: /\r\n";
	ft_get_content_type(datas);
	create_send += datas->content_type;
//	create_send += "Content-Type: text/html\r\n";
	create_send += "Date: Mon, 31 Oct 2022 17:15:12 GMT\r\n";
	create_send += "Server: webserv/42\r\n\n";
//	create_send += "Salut Maxime et Wilhelm-";

	std::cout << create_send << std::endl;
	create_send += body_reponse + "\r\n";
	


	return (create_send);
}

