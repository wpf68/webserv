/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pwolff <pwolff@student.42mulhouse.fr>>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/03 09:46:00 by pwolff            #+#    #+#             */
/*   Updated: 2022/11/03 09:46:00 by pwolff           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.h"

std::string get_reponse(std::string &buffer, std::string request, std::string endl)
{
	std::string		reponse;
	std::size_t		len_begin;
	std::size_t		len_end;
	
	len_begin = buffer.find(request);
	if (endl == "\0")
		len_end = buffer.size();
	else
		len_end = buffer.find(endl, len_begin + request.size());
	reponse.assign(buffer, len_begin + request.size(), len_end - len_begin - request.size());
	std::cout << GREEN << "begin get_reponse: " << len_begin << " end : " \
	<< len_end << " fin : " << len_end - len_begin - request.size() << NONE << std::endl;

	return (reponse);
}