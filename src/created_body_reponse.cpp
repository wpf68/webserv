/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   created_body_reponse.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pwolff <pwolff@student.42mulhouse.fr>>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 13:14:22 by pwolff            #+#    #+#             */
/*   Updated: 2022/11/21 13:14:22 by pwolff           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.h"

std::string ft_created_body_reponse(t_client *datas)
{
	std::string	file;
	std::string	temp;

	temp = get_reponse(datas->buffer, "", "\n");
	std::cout << RED "---- first line : " << temp << std::endl;
	if (temp.find("?") != std::string::npos) 		
	{
		std::cout << RED "Message GET recu" NONE << std::endl;
		return (ft_formulaire_get_post(temp, datas));
	}
	else if (temp.find("POST") != std::string::npos)
	{
		std::cout << RED "Message POST recu" NONE << std::endl;
		return (ft_formulaire_get_post(datas->buffer, datas));
	}
	std::cout << RED "NO Message" NONE << std::endl;

	if (temp.find("__repertory__.html") != std::string::npos)
	{
		datas->path_request = "/repertory.html";
    	datas->status = "200 server form";
		ft_test_request_exist(datas, datas->path_request);
		return (ft_parsing_form("", datas));
	}

	datas->path_request = get_reponse(datas->buffer, "GET ", " ");
	std::cout << RED "PATH Request Client : " YELLOW << datas->path_request \
			<< NONE "\n" << std::endl;
	
	datas->client_path = datas->root + datas->location;
	if (datas->path_request == "/")
		datas->client_path += "/index.html";  // ----------------------------- Variable du parsing path by default
	else                                      // voir à tester si le path est autorisé par le .conf
		datas->client_path += datas->path_request;
	std::cout << RED "PATH Reponse Server : " YELLOW << datas->client_path \
			<< NONE "\n" << std::endl;
	if (ft_test_request_exist(datas, datas->client_path))
		return ("");
	file = ft_read_file(datas);
	return (file);
}





std::string ft_read_file(t_client *datas)
{
	char		c;
	std::string	file;

	file = "";

	// ---  test pour page 500
	if (datas->client_path == datas->root + datas->location + "/500.html")
	{
		datas->list_request_received = "";
		datas->status = "500 webser42_error_server :(";
			return (datas->file_500);

	}
	std::cout << "--- datas->client_path : " << datas->client_path << std::endl;
	std::ifstream my_flux(datas->client_path);
	if (!my_flux)
	{
		datas->status = "404 webser42_error_page :(";
		datas->client_path = datas->file_404;
		datas->list_request_received = "";
		std::ifstream my_flux2(datas->client_path);
		if (!my_flux2)
		{
			datas->status = "500 webser42_error_server :(";
			return (datas->file_500);
		}
		while (my_flux2.get(c))
		file += c;
		my_flux2.close();
		return (file);
	}
	while (my_flux.get(c))
		file += c;
	my_flux.close();
	return (file);
}