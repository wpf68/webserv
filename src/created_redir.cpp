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

std::string    ft_redir(std::string redir, t_client *datas)
{
    std::string file;

    file = "<!DOCTYPE html> <html> <head> <meta http-equiv=\"Refresh\" content=\"0; url=//";
    file += redir + "\" /> </head> <body> <p>Redirection in 0 seconds</p> </body> </html>";

    return (file);
}