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

std::string ft_formulaire_get_post(std::string &datas)
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
 //   datas->client_path = "formulaire";

    return (file);
}