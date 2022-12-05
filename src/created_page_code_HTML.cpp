/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   created_redir.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pwolff <pwolff@student.42mulhouse.fr>>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/04 18:29:31 by pwolff            #+#    #+#             */
/*   Updated: 2022/12/04 18:29:31 by pwolff           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.h"

std::string    ft_code_HTTP(std::string code, std::string message, t_client *datas)
{
    std::string file;

    file = "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><meta http-";
    file += "equiv=\"X-UA-Compatible\" content=\"IE=edge\"><meta name=\"viewport\" ";
    file += "content=\"width=device-width, initial-scale=1.0\"><title>" + code;
    file += "</title></head><body><a href=\"./index.html\">\"Go to Accueil\"</a>";
    file += "<h1 style=\"color: red;\">" + message;
    file += "</h1></body></html>";
    return (file);
}