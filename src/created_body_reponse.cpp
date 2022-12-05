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

extern std::map<std::string, std::string>   var_content_code;

std::string ft_created_body_reponse(t_client *datas)
{
	std::string	file;
	std::string	temp;
	std::string path;

	file = "";
	for (int j = 0; j < datas->location.size(); j++)
	{
		std::map<std::string, std::string>::iterator it = datas->location[j].redir.begin();

		for (; it != datas->location[j].redir.end(); it++ )
		{
			if (it->second != "")
			{
				std::cout << RED "redirect : " << it->second << std::endl;
				datas->status = it->first + " redirect";
				datas->client_path = "redir.html";
				datas->path_request = it->second;
				return (ft_redir(it->second, datas));
			}
		}
	}
	if (datas->buffer.find("Content-Length:") != std::string::npos)
	{
		temp = get_reponse(datas->buffer, "Content-Length: ", "\n");
		std::stringstream	ss;
		int					size_body;

		ss << temp;
		ss >> size_body;
		std::cout << RED "size int - " << size_body << NONE << std::endl;
		if (size_body > datas->size && datas->buffer.find("DELETE_file_for_delete") == std::string::npos)
		{
			datas->status = "403 " + var_content_code["403"] + " " + datas->name_server;
			datas->client_path = "HTML/403_size.html";
			datas->list_request_received.erase();
			return (ft_read_file(datas));
		}
	}
	temp = get_reponse(datas->buffer, "", "\n");
	if (temp.find("?") != std::string::npos) 		
	{
		if (datas->location[0].methods.find("GET") == std::string::npos)
		{
			datas->status = "405 " + var_content_code["405"] + " " + datas->name_server;
			datas->client_path = "HTML/405_GET_prohibited.html";
			datas->list_request_received.erase();
			return (ft_read_file(datas));
		}
		return (ft_formulaire_get_post(temp, datas));
	}
	else if (temp.find("DELETE") != std::string::npos)
	{
		datas->path_request = get_reponse(datas->buffer, "DELETE ", " ");
		if (datas->path_request != "/test_delete.html")
		{
			datas->status = "405 " + var_content_code["405"] + " " + datas->name_server;
			datas->client_path = "HTML/405_DELETE_prohibited.html";
			datas->list_request_received.erase();
			return(ft_read_file(datas));
		}
		path = datas->root + datas->path_request;
		ft_delete(path, datas);
		return (file);
	}
	else if (temp.find("POST") != std::string::npos)
		return (ft_formulaire_get_post(datas->buffer, datas));
	if (temp.find("__repertory__.html") != std::string::npos)
	{
		datas->path_request = "/repertory.html";
    	datas->status = "200 " + var_content_code["200"] + " " + datas->name_server;
		ft_test_request_exist(datas, datas->path_request);
		return (ft_parsing_form("", datas));
	}
	if (datas->location[0].methods.find("GET") == std::string::npos)
	{
		datas->status = "405 " + var_content_code["405"] + " " + datas->name_server;
		datas->client_path = "HTML/405_GET_prohibited.html";
		datas->list_request_received.erase();
		return (ft_read_file(datas));
	}
	datas->path_request = get_reponse(datas->buffer, "GET ", " ");
	datas->client_path = datas->root;
	if (datas->path_request == "/")
		datas->client_path += datas->root_path;
	else 
		datas->client_path += datas->path_request;
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
	if (datas->client_path.find("/500.html") != std::string::npos)
	{
		datas->list_request_received = "";
		datas->status = "500 " + var_content_code["500"] + " " + datas->name_server;
		datas->client_path = datas->file_500;
		datas->list_request_received.erase();
		std::ifstream test_file(datas->file_500);
		if (!test_file)
			return (datas->file_500_bis);
		else
			test_file.close();
		std::ifstream my_flux2(datas->client_path);
		while (my_flux2.get(c))
		file += c;
		my_flux2.close();
		return (file);
	}
	std::string	test_path_valide = datas->client_path;
	if (test_path_valide.rfind("/") + 1 == test_path_valide.size())
		test_path_valide = test_path_valide.substr(0, test_path_valide.size() - 1);
	std::ifstream my_flux(test_path_valide);

	if (!my_flux)
	{
		if (datas->client_path.find("Directory.html") != std::string::npos)
		{
			file = auto_index("./", "./");
			return (file);
		}
		if (datas->client_path.rfind("/") + 1 == datas->client_path.size() && datas->client_path.size() != 1)
		{
			std::string	trequete = get_reponse(datas->path_request, "/", "/");
			if (datas->path_request.find(trequete, trequete.size()) != std::string::npos)
				trequete = get_reponse(datas->path_request, trequete, "\0");
			else
				trequete = datas->path_request;
			datas->path_request = trequete;
			trequete = "./" + trequete + "/";
			file = auto_index(trequete, datas->path_request);
			if (file.size() != 0)
				return (file);
		}

		datas->status = "404 " + var_content_code["404"] + " " + datas->name_server;
		datas->client_path = datas->file_404;
		datas->list_request_received.erase();
		std::ifstream my_flux2(datas->client_path);
		if (!my_flux2)
		{
			datas->status = "500 " + var_content_code["500"] + " " + datas->name_server;
			datas->client_path = datas->file_500;
			datas->list_request_received.erase();
			std::ifstream test_file(datas->file_500);
			if (!test_file)
				datas->client_path = datas->file_500_bis;
			else
				test_file.close();
			return (datas->file_500);
		}
		while (my_flux2.get(c))
		file += c;
		my_flux2.close();
		return (file);
	}
	for (int i = 0; i < datas->location.size(); i++)
	{
		if (datas->location[i].dir_listing == "on" && test_path_valide.find(datas->location[i].root) != std::string::npos)
		{
			file = auto_index(datas->root, datas->root);
			return (file);
		}
	}
	while (my_flux.get(c))
		file += c;
	my_flux.close();

	return (file);
}