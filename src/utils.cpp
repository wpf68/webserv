/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pwolff <pwolff@student.42mulhouse.fr>>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/10 10:48:44 by pwolff            #+#    #+#             */
/*   Updated: 2022/12/10 10:48:44 by pwolff           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.h"

int exist_path(std::string path, t_client *datas)
{
    (void)datas;

    std::ifstream test(path.c_str());
    if (!test)
        return (0);
    test.close();

    return(1);
}