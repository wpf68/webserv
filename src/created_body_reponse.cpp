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
    	datas->status = "200 " + datas->name_server + " form";
		ft_test_request_exist(datas, datas->path_request);
		return (ft_parsing_form("", datas));
	}

	datas->path_request = get_reponse(datas->buffer, "GET ", " ");
	std::cout << RED "PATH Request Client : " YELLOW << datas->path_request \
			<< NONE "\n" << std::endl;
	
	datas->client_path = datas->root;
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

	//---  test pour page 500
	if (datas->client_path.find("/500.html") != std::string::npos)
	{
		datas->list_request_received = "";
		datas->status = "500 " + datas->name_server + "_error_server :(";
		datas->client_path = datas->file_500;
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
	std::cout << "--- datas->client_path : " << datas->client_path << std::endl;
	// -- test chemin valide
	std::string	test_path_valide = datas->client_path;
//	std::ifstream my_flux(datas->client_path);
	if (test_path_valide.rfind("/") + 1 == test_path_valide.size())
		test_path_valide = test_path_valide.substr(0, test_path_valide.size() - 1);
	std::cout << "test open ? : " << test_path_valide << std::endl;	
	std::ifstream my_flux(test_path_valide);

	if (!my_flux)
	{
		std::cout << CYANE "buffer bis ------- \n"<< datas->buffer << NONE << std::endl;   /// test
		if (datas->client_path.find("Directory.html") != std::string::npos)
		{
			file = auto_index("./", "./");
	//		file = auto_index("./", "------_test_Directory_--------");
			return (file);
		}
		std::cout << "*** pos de / : " << datas->client_path.rfind("/")  << "size = " << datas->client_path.size() << std::endl;  // test
		if (datas->client_path.rfind("/") + 1 == datas->client_path.size() && datas->client_path.size() != 1)
		{
			std::cout << CYANE "======= MATCH ===========" << std::endl; //------------------
	//		std::string	path = get_reponse(datas->client_path, "_test_Directory_--------/", " ");
	//		std::string	path = get_reponse(datas->client_path, "HTML/site_1/", " ");
			

			// traitement de la requete
			std::string	trequete = get_reponse(datas->path_request, "/", "/");
			std::cout << "trequete = " << trequete << std::endl;
			if (datas->path_request.find(trequete, trequete.size()) != std::string::npos)
				trequete = get_reponse(datas->path_request, trequete, "\0");
			else
				trequete = datas->path_request;
			std::cout << "trequete = " << trequete << std::endl;
			std::cout << "***** data client request = " << datas->path_request << "  trequete = " << trequete << std::endl;
			std::cout << "***** data client = " << datas->client_path << "  trequete = " << trequete << std::endl;
		//	datas->client_path = "/" + path;

			datas->path_request = trequete;
			trequete = "./" + trequete + "/";
			
			std::cout << "***** data client = " << datas->client_path << "  trquete = " << trequete << std::endl;
			file = auto_index(trequete, datas->path_request);
		//	datas->path_request = "";
			if (file.size() != 0)
				return (file);
		}
		datas->status = "404 " + datas->name_server + " error_page :(";
		datas->client_path = datas->file_404;
		datas->list_request_received = "";
		std::ifstream my_flux2(datas->client_path);
		if (!my_flux2)
		{
			datas->status = "500  " + datas->name_server + "_error_server :(";
			datas->client_path = datas->file_500;
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
	while (my_flux.get(c))
		file += c;
	my_flux.close();
	// std::cout << "file =\n" << file << std::endl;
	// exit (1); //-------------

	return (file);
}