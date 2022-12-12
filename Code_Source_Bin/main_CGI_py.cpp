/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pwolff <pwolff@student.42mulhouse.fr>>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/09 12:37:41 by pwolff            #+#    #+#             */
/*   Updated: 2022/12/09 12:37:41 by pwolff           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#  define RED		"\033[1;31m"
#  define NONE		"\033[0;37m"

#include <cstring>
#include <iostream>
#include <fstream>
#include <stdlib.h>

int main(int argc, char **argv)
{
    std::string file;
    std::string test_path_valide = argv[1];

    (void)argc;
    // création du scipt.sh
    file = "#!/bin/bash\n\n";
    file += "exec python3 " + test_path_valide + " > temp.html\n\n";

    std::ofstream	ofs("script_cgi_py.sh", std::ios::out | std::ios::trunc);
	if (!ofs)
    {
        std::cout << RED "Error file out" NONE << std::endl;
        return (1);
    }
		
	ofs << file << std::endl;
	ofs.close();

    system("chmod 777 script_cgi_py.sh");
    system("./script_cgi_py.sh");

    // file = "";
    // std::ifstream my_flux("temp.html");
	// if (!my_flux)
	// 	std::cout << RED "file no found !!!" NONE << std::endl;
	// while (my_flux.get(c))
	// 	file += c;
	// my_flux.close();

    // remove("temp.html");
    // remove("script_cgi_py.sh");

    return (0);
}