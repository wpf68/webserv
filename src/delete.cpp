/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delete.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pwolff <pwolff@student.42mulhouse.fr>>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/24 10:43:40 by pwolff            #+#    #+#             */
/*   Updated: 2022/12/02 15:21:35 by pwolff           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.h"

void	ft_delete(std::string path, t_client *datas)
{
	int	test;
	std::ifstream file(path);

//	std::cout << "Delete : " << path << std::endl;
	if (file.good())
	{
		test = remove(path.c_str());
		if (!test)
			datas->status = "200 OK file deleted";
		else
			datas->status = "202 Accepted";
	}
	else
		datas->status = "204 No Content file no found";
	datas->list_request_received.erase();
}