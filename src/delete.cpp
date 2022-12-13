/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delete.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pwolff <pwolff@student.42mulhouse.fr>>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/24 10:43:40 by pwolff            #+#    #+#             */
/*   Updated: 2022/12/13 11:39:10 by pwolff           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.h"

extern std::map<std::string, std::string>   var_content_code;

void	ft_delete(std::string path, t_client *datas)
{
	int	test;
	std::ifstream file(path);

	if (file.good())
	{
		test = remove(path.c_str());
		if (test == 0)
		{
			datas->status = "200 " + var_content_code["200"] + " " + datas->name_server;
			// std::cout << "++++++  delete  +++++++++" << std::endl;
			// datas->path_request = "/successful.html";
			// datas->client_path = "HTML/successful.html";

		}
			
		else
			datas->status = "202 " + var_content_code["202"] + " " + datas->name_server;
	}
	else
		datas->status = "202 " + var_content_code["202"] + " " + datas->name_server;
	datas->list_request_received.erase();
}