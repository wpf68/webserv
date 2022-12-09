/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_formulaire_get_post.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pwolff <pwolff@student.42mulhouse.fr>>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 12:38:12 by pwolff            #+#    #+#             */
/*   Updated: 2022/11/17 12:38:12 by pwolff           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.h"

extern std::map<std::string, std::string>   var_content_code;

int ft_split(std::vector<std::string>& vecteur, std::string chaine, char separateur)
{
	std::string::size_type stTemp = chaine.find(separateur);
	
	while(stTemp != std::string::npos)
	{
		vecteur.push_back(chaine.substr(0, stTemp));
		chaine = chaine.substr(stTemp + 1);
		stTemp = chaine.find(separateur);
	}
	vecteur.push_back(chaine);
	return (vecteur.size());
}

std::string  ft_parsing_form(std::string temp, t_client *datas)
{
	std::string                 file;
	std::vector<std::string>    VecStr;
	int                         j;

	VecStr.clear();
	file = "";
	if (temp != "")
	{
		int nbTabl = ft_split(VecStr, temp, '&');
		for(int i = 0; i < nbTabl; ++i)
			ft_split(datas->repertory, VecStr[i], '=');
	}
	j = 0;
	datas->client_path = datas->root + "/repertory_part1.html";
	file = ft_read_file(datas);
	for (std::vector<std::string>::iterator itt = datas->repertory.begin(); itt != datas->repertory.end(); itt ++)
	{
		if (j % 2)
		{
			file += "<p style=\"color: rgb(0, 0, 0); text-align: center; background-color: darksalmon;\"><span>" + *itt + "</span>";
			if ((itt + 1) != datas->repertory.end())
			{
				itt += 2;
				if (itt != datas->repertory.end())
					file += "<span>" + *itt + "</span></p>";
			}
			else
				file += "<span>  </span></p>";
		}     
		j++;
	}
	file += " </body> </html>";
	temp.clear();

	return (file);
}

static void	ft_upload(std::string &datas, t_client *datas_client)
{
	std::string	corps;
	std::string	key;
	std::string	type;
	std::string	file;

	std::cout << CYANE "datas : \n" << datas_client->buffer << "\n" NONE << std::endl; 

	corps = get_reponse(datas, "\r\n\r\n", "\0");
	std::cout << CYANE "corps : \n" << corps << "\n" NONE << std::endl; 
	key = get_reponse(corps, "", "\n");
	std::cout << CYANE "key : \n" << key << "\n" NONE << std::endl; 
	type = get_reponse(corps, "Content-Type: ", "\n");
	type = get_reponse(type, "/", "\n");
	file = get_reponse(corps, "\r\n\r\n", key);
	std::cout << RED "Type : " << type << "\nfile :\n" << file << "\n" NONE << std::endl; 
}

std::string ft_formulaire_get_post(std::string &datas, t_client *datas_client)
{
	std::string file;
	std::string temp;
	std::string first_line;

	file = "";
	if (datas.find("POST") != std::string::npos)
	{
		temp = get_reponse(datas, "\r\n\r\n", "\0");
		if (temp.find("DELETE_file_for_delete") != std::string::npos)
		{
			if (datas_client->location[0].methods.find("DELETE") == std::string::npos)
			{
				datas_client->status = "405 " + var_content_code["405"] + " " + datas_client->name_server;
				datas_client->client_path = "HTML/405_DELETE_prohibited.html";
				datas_client->list_request_received.erase();
				return (ft_read_file(datas_client));
			}
			first_line = get_reponse(datas_client->buffer, "", "\n");
			if (temp.find("NO") != std::string::npos)
			{
				temp.clear();			
				datas_client->status = "204 " + var_content_code["204"] + " " + datas_client->name_server;
			//	datas_client->status = "200 " + var_content_code["200"] + " " + datas_client->name_server;

				temp = get_reponse(datas, "", "\r\n\r\n");
				temp.erase(0, first_line.size());
				temp = "GET /" + get_reponse(first_line, " /", " ") + " HTTP/1.1" + temp + "\n";
				datas_client->buffer = temp;
				return (file);
			}
			temp = get_reponse(datas, "", "\r\n\r\n");
			temp.erase(0, first_line.size());
			temp = "DELETE /test_delete.html HTTP/1.1" + temp + "\n";
			datas_client->buffer = temp;
			datas_client->client_path = "HTML/site_3_form/delete.html";
			file = ft_read_file(datas_client);
			ft_delete("HTML/site_3_form/test_delete.html", datas_client);
		//	datas_client->path_request = "/successful.html";
			return (file);
		}
		if (datas_client->location[0].methods.find("POST") == std::string::npos)
		{
			datas_client->status = "405 " + var_content_code["405"] + " " + datas_client->name_server;
			datas_client->client_path = "HTML/405_POST_prohibited.html";
			datas_client->list_request_received.erase();
			return (ft_read_file(datas_client));
		}
	}
	else if (datas.find("GET") != std::string::npos)
		temp = get_reponse(datas, "?", " ");
	else
		std::cout << RED "********  Prb Formulaire POST GET" NONE << std::endl;


	// std::cout << GREEN "temp : \n" << temp << std::endl;  //  ----------------------
	if (datas_client->buffer.find("boundary") != std::string::npos)
	{
		if (!ft_upload_to_server(temp, datas_client))
		{
			datas_client->status = "500 " + var_content_code["500"] + " " + datas_client->name_server;
			datas_client->client_path = "HTML/500.html";
			datas_client->list_request_received.erase();
			return (ft_read_file(datas_client));

		}
		file = "<!DOCTYPE html><html lang=\"en\"><head>";
		file += "<meta charset=\"UTF-8\"><meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">";
		file += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><title>Upload</title></head>";
		file += "<body><a href=\"./index.html\">\"Go to the home page\" </a><h1 style=\"color: red;\">Upload to server in ";
		file += datas_client->dir_stock + " successful</h1>";
		file += "</body></html>";
		return(file);
	}
	if (temp.find("file_to_path_server=") != std::string::npos)
	{
		// datas_client->status = "200 " + var_content_code["200"] + " " + datas_client->name_server;
		// datas_client->client_path = "HTML/200.html";
		// datas_client->list_request_received.erase();
		std::string	path = datas_client->root + "/" + datas_client->dir_stock + "/" + get_reponse(temp, "file_to_path_server=", "\0");
		std::cout << RED "temp : \n" << temp << "-- path : --" << path << "--" << std::endl;  //  ----------------------
		std::ifstream my_flux(path);
		if (!my_flux)
		{
			// std::cout << RED "file no found !!!" NONE << std::endl;
			// datas_client->status = "404 " + var_content_code["404"] + " " + datas_client->name_server;
			// datas_client->client_path = "HTML/404.html";
			// datas_client->list_request_received.erase();
			// return (ft_read_file(datas_client));
		file = "<!DOCTYPE html><html lang=\"en\"><head>";
		file += "<meta charset=\"UTF-8\"><meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">";
		file += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><title>Download</title></head>";
		file += "<body><a href=\"./index.html\">\"Go to the home page\" </a><h1 style=\"color: red;\">Download to server no accepted</h1>";
		file += "<h1 style=\"color: red;\">File no valid</h1>";
		file += "</body></html>";
		return (file);

		}
		my_flux.close();
		file = "<!DOCTYPE html><html lang=\"en\"><head>";
		file += "<meta charset=\"UTF-8\"><meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">";
		file += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><title>Download</title></head>";
		file += "<body><a href=\"./index.html\">\"Go to the home page\" </a><h1 style=\"color: red;\">Download to server accepted</h1>";
		file += "<p><a type=\"application/octet-stream\" href=\"" + datas_client->dir_stock + "/";
	//	file += "<p><a href=\"File_Upload/";
		file += get_reponse(temp, "file_to_path_server=", "\0");
		file += "\"download> Confirm to Download ";
		file += get_reponse(temp, "file_to_path_server=", "\0");
		file += "</a></p>";
		file += "</body></html>";
		return (file);
	}

	file = ft_parsing_form(temp, datas_client);
	temp.clear();
	datas_client->path_request = "/repertory.html";
	datas_client->status = "205 " + var_content_code["205"] + " " + datas_client->name_server;
	return (file);
}

