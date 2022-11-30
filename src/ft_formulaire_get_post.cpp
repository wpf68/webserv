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
		{
			std::cout << i << " : '" << VecStr[i] << "'" << std::endl;
			ft_split(datas->repertory, VecStr[i], '=');
		}
		std::cout << std::endl;
	}
	j = 0;

	datas->client_path = datas->root + "/repertory_part1.html";
	std::cout << datas->client_path << " *********" << std::endl; ////
	file = ft_read_file(datas);

	
 //   std::cout << file << std::endl;  ///---------------
	for (std::vector<std::string>::iterator itt = datas->repertory.begin(); itt != datas->repertory.end(); itt ++)
	{
		if (j % 2)
		{
			std::cout << *itt << " - ";
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
	std::cout << std::endl;
	temp.clear();
 	std::cout << file << std::endl;  ///---------------
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
		// if (datas.find("Content-Type:") != std::string::npos)
		// {
		// 	ft_upload(datas, datas_client);
		// 	return(file);
		// }
		temp = get_reponse(datas, "\r\n\r\n", "\0");
		if (temp.find("DELETE_file_for_delete") != std::string::npos)
		{
			if (datas_client->location[0].methods.find("DELETE") == std::string::npos)  //----------------- test une seule location
			{
				datas_client->status = "403 demand prohibited";
				datas_client->client_path = "HTML/403_DELETE_prohibited.html";
				return (ft_read_file(datas_client));
			}
			first_line = get_reponse(datas_client->buffer, "", "\n");
			if (temp.find("NO") != std::string::npos)
			{

		//		std::cout << "----------- datas : \n" << datas << std::endl;

				temp.clear();			
			//	datas_client->path_request = "/index.html";
			//	datas_client->status = "307 demand success redirection ";
				datas_client->status = "204 demand success";

				temp = get_reponse(datas, "", "\r\n\r\n");
				temp.erase(0, first_line.size());
				temp = "GET /" + get_reponse(first_line, " /", " ") + " HTTP/1.1" + temp + "\n";
				datas_client->buffer = temp;
			//	datas_client->client_path = "HTML/site_3_form/index.html";
			//	file = ft_read_file(datas_client);
		//		std::cout << RED "--- ---  BYE ----- \n" NONE << std::endl;

				return (file);
			}
			temp = get_reponse(datas, "", "\r\n\r\n");
			temp.erase(0, first_line.size());
			temp = "DELETE /test_delete.html HTTP/1.1" + temp + "\n";
			datas_client->buffer = temp;
			datas_client->client_path = "HTML/site_3_form/successful.html";
			file = ft_read_file(datas_client);
			ft_delete("HTML/site_3_form/test_delete.html", datas_client);
			return (file);
		}
		if (datas_client->location[0].methods.find("POST") == std::string::npos)  //----------------- test une seule location
		{
			datas_client->status = "403 demand prohibited";
			datas_client->client_path = "HTML/403_POST_prohibited.html";
			return (ft_read_file(datas_client));
		}
		std::cout << "Formulaire POST" << std::endl;
		
	}
	else if (datas.find("GET") != std::string::npos)
	{
		std::cout << "Formulaire GET" << std::endl;
		temp = get_reponse(datas, "?", " ");
	}
	else
	{
		std::cout << RED "********  Prb Formulaire POST GET" NONE << std::endl;
	}


		std::cout << GREEN "formulaire : " NONE << temp << std::endl;
		file = ft_parsing_form(temp, datas_client);
		temp.clear();
		datas_client->path_request = "/repertory.html";
		datas_client->status = "205 demand success";
		return (file);
}

