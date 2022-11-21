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

	datas->client_path = datas->root + datas->location + "/repertory_part1.html";
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
 //   std::cout << file << std::endl;  ///---------------
	return (file);
}

std::string ft_formulaire_get_post(std::string &datas, t_client *datas_client)
{
	std::string file;
	std::string temp;

	file = "";
	if (datas.find("POST") != std::string::npos)
	{
		std::cout << "Formulaire POST" << std::endl;
	  //  temp = "\n\n";
		temp = get_reponse(datas, "\r\n\r\n", "\0");
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

