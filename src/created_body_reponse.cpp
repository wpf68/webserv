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
				//std::cout << RED "redirect : " << it->second << std::endl;
				datas->status = it->first + " " + var_content_code[it->first] + " " + datas->name_server;
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
	//	std::cout << RED "size int - " << size_body << NONE << std::endl;
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
		if (datas->status.find("200 ") != std::string::npos)
		{
			// std::cout << "++++++  delete  +++++++++" << std::endl;
			// datas->path_request = "/successful.html";
			// datas->client_path = "HTML/successful.html";
			// return (ft_read_file(datas));

		}		
		return (file);
	}
	else if (temp.find("POST") != std::string::npos)
		return (ft_formulaire_get_post(datas->buffer, datas));
	if (temp.find("__repertory__.html") != std::string::npos)
	{
		datas->path_request = "/repertory.html";
    	datas->status = "200 " + var_content_code["200"] + " " + datas->name_server;
		//ft_test_request_exist(datas, datas->path_request);
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
	{
		//	datas->client_path += datas->root_path;
		if (datas->index != "")
			datas->client_path = datas->root + datas->index; 
		else
			datas->client_path = datas->root + "/index.html";	
	}
	
	else 
		datas->client_path += datas->path_request;
	// if (ft_test_request_exist(datas, datas->client_path))
	// 	return ("");
//	std::cout << RED "*********client_path = " << datas->client_path << "--" NONE << std::endl;
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
		// ------------  test ---------
		// if (datas->client_path.find("Directory.html") != std::string::npos)
		// {
		// 	std::ifstream my_flux2(datas->path_request.erase(0,1));
		// 	datas->status = "200 " + var_content_code["200"] + " " + datas->name_server;
		// 	while (my_flux2.get(c))
		// 		file += c;
		// 	my_flux2.close();
		// 	return (file);
		// }
		for (int i = 0; i < datas->location.size(); i++)
		{
			if (datas->location[i].dir_listing == "on" && test_path_valide.find(datas->location[i].root) != std::string::npos)
			{
				std::ifstream my_flux2(datas->path_request.erase(0,1));
				datas->status = "200 " + var_content_code["200"] + " " + datas->name_server;
				while (my_flux2.get(c))
					file += c;
				my_flux2.close();
				// std::cout << "*******  on  **********" << std::endl;
				// std::cout << datas->path_request << "\n" << file << std::endl;
				return (file);
			}
		}
		// ---------------------------------------

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
			file = auto_index(datas->root + datas->index, datas->root + datas->index);
			return (file);
		}
	}
	// --------------- CGI  ------------------------------
	// if (test_path_valide.find(".py") != std::string::npos)
	// 	return (ft_CGI_py(test_path_valide, datas));
		
	if (test_path_valide.find(".py") != std::string::npos)
		return (CGI_py(test_path_valide, datas));

	// {	
	// 	int			pid = 0;
	// 	std::string source_bin = "";
	// 	int			test_bin;

	// 	for (int j = 0; j < datas->location.size(); j++)
	// 	{
	// 		if (datas->location[j].req_client.find("/cgi") != std::string::npos)
	// 			source_bin = datas->location[j].cgi["py"];
	// 	}
		
	// 	std::cout << GREEN "path : " << test_path_valide << " source bin : " << source_bin << std::endl;
	// 	if (source_bin == "" || !(exist_path(source_bin, NULL)))
	// 	{
	// 		datas->status = "503 " + var_content_code["503"] + " " + datas->name_server;
	// 		datas->client_path = "HTML/503.html";
	// 		datas->list_request_received.erase();
	// 		std::ifstream my_flux2(datas->client_path);
	// 		if (!my_flux2)
	// 		{
	// 			datas->status = "500 " + var_content_code["500"] + " " + datas->name_server;
	// 			datas->client_path = datas->file_500;
	// 			datas->list_request_received.erase();
	// 			std::ifstream test_file(datas->file_500);
	// 			if (!test_file)
	// 				datas->client_path = datas->file_500_bis;
	// 			else
	// 				test_file.close();
	// 			return (datas->file_500);
	// 		}
	// 		while (my_flux2.get(c))
	// 		file += c;
	// 		my_flux2.close();
	// 		return (file);
	// 	}
	// 	pid = fork();
	// 	if (pid < 0)
	// 		std::cout << RED "Error fork" NONE << std::endl;
	// 	if (pid == 0)
	// 	{
	// 		execlp(source_bin.c_str(), source_bin.c_str(), test_path_valide.c_str(), NULL);
	// 	}
	// 	else
	// 	{
	// 		std::cout << "********  end fork *************" << std::endl;
	// 		waitpid(pid, NULL, 0);
	// 	}

	// 	file = "";
	// 	std::ifstream my_flux("temp.html");
	// 	if (!my_flux)
	// 		std::cout << RED "file no found !!!" NONE << std::endl;
	// 	while (my_flux.get(c))
	// 		file += c;
	// 	my_flux.close();

	// 	remove("temp.html");
	// 	remove("script_cgi_py.sh");
	// 	return (file);
	// }

	if (test_path_valide.find(".c") != std::string::npos && test_path_valide.find(".c") == test_path_valide.size() - 2)
		return(CGI_c(test_path_valide, datas));
	// {	
	// 	int			pid = 0;
	// 	std::string source_bin = "";
	// 	int			test_bin;

	// 	for (int j = 0; j < datas->location.size(); j++)
	// 	{
	// 		if (datas->location[j].req_client.find("/cgi") != std::string::npos)
	// 			source_bin = datas->location[j].cgi["c"];
	// 	}
		
	// 	std::cout << GREEN "path : " << test_path_valide << " source bin : " << source_bin << std::endl;
	// 	if (source_bin == "" || !(exist_path(source_bin, NULL)))
	// 	{
	// 		datas->status = "503 " + var_content_code["503"] + " " + datas->name_server;
	// 		datas->client_path = "HTML/503.html";
	// 		datas->list_request_received.erase();
	// 		std::ifstream my_flux2(datas->client_path);
	// 		if (!my_flux2)
	// 		{
	// 			datas->status = "500 " + var_content_code["500"] + " " + datas->name_server;
	// 			datas->client_path = datas->file_500;
	// 			datas->list_request_received.erase();
	// 			std::ifstream test_file(datas->file_500);
	// 			if (!test_file)
	// 				datas->client_path = datas->file_500_bis;
	// 			else
	// 				test_file.close();
	// 			return (datas->file_500);
	// 		}
	// 		while (my_flux2.get(c))
	// 		file += c;
	// 		my_flux2.close();
	// 		return (file);
	// 	}
	// 	pid = fork();
	// 	if (pid < 0)
	// 		std::cout << RED "Error fork" NONE << std::endl;
	// 	if (pid == 0)
	// 	{
	// 		execlp(source_bin.c_str(), source_bin.c_str(), test_path_valide.c_str(), NULL);
	// 	}
	// 	else
	// 	{
	// 		std::cout << "********  end fork *************" << std::endl;
	// 		waitpid(pid, NULL, 0);
	// 	}

	// 	file = "";
	// 	std::ifstream my_flux("temp.html");
	// 	if (!my_flux)
	// 		std::cout << RED "file no found !!!" NONE << std::endl;
	// 	while (my_flux.get(c))
	// 		file += c;
	// 	my_flux.close();

	// 	remove("temp.html");
	// 	remove("script_cgi_c.sh");
	// 	remove("sh_c");
		
	// 	return (file);
	// }
	// ---------------------------------------------
	while (my_flux.get(c))
		file += c;
	my_flux.close();

	return (file);
}