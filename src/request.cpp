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

std::string get_path(std::string buffer)
{
	std::string		path;
	std::size_t		len_begin;
	std::size_t		len_end;
	
	len_begin = buffer.find("GET");
	len_end = buffer.find(" ", len_begin + 4);
	path.assign(buffer, len_begin + 4, len_end - (len_begin + 4));
	std::cout << GREEN << "begin : " << len_begin << " end : " << len_end << NONE << std::endl;

	return (path);
}