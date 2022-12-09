/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   upload_to_server.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pwolff <pwolff@student.42mulhouse.fr>>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/08 11:18:10 by pwolff            #+#    #+#             */
/*   Updated: 2022/12/08 11:18:10 by pwolff           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.h"

extern std::map<std::string, std::string>   var_content_code;

bool ft_upload_to_server(std::string temp, t_client *datas_client)
{
	std::string begin_end_flag;
	std::string filename;
	std::string file_copy;
//	size_t		nb;
	
	filename = get_reponse(temp, "filename=\"", "\"");
	filename = "HTML/site_1/" + datas_client->dir_stock + "/" + filename;
	begin_end_flag = get_reponse(temp, "", "\r\n");
	file_copy = get_reponse(temp, "\n\r\n", "\n" + begin_end_flag);
	//nb = file_copy.find("--------");

	//file_copy.erase(nb);
	
	// std::cout << YELLOW "filename : --" << filename << "--" << std::endl;
	// std::cout << YELLOW "begin-end : **" << begin_end_flag << "**" << std::endl;
	// std::cout << YELLOW "body : \n" << file_copy<< "\n-- end --" << std::endl;

	std::ofstream	ofs(filename, std::ios::out | std::ios::app);
	if (!ofs)
	{
		std::cout << "Error file out" << std::endl;
		return (false);
		//exit(0);
	}
	ofs << file_copy << std::endl;
	ofs.close();

	return (true);
}