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

#include "webserv.h"

std::string	ft_CGI_py(std::string test_path_valide, t_client *datas)
{
// 	int			pid = 0;

//     char        args[100][200] = {"/bin/exec", "python3", "cgitest", ">", "cgi___test2", 0};
//   //  char        args[100][200] = {"/bin/cp", "cgitest", "cgi___test2", 0};
//     char        *arg[100];

//     file = "";
//     arg[0] = args[0];
//     arg[1] = args[1];
//     arg[2] = args[2];
//     arg[3] = args[3];
//     arg[4] = args[4];
//     arg[5] = args[5];

//     for (int i = 0; arg[i]; i++)
//         std::cout << "arg " << i << " : " << arg[i] << std::endl;

// 	pid = fork();
// 	if (pid < 0)
// 		ft_error("Error fork", datas);
// 	if (pid == 0)
// 	{
//         //execlp("cp", "cp", test_path_valide.c_str(), "cgi____test", NULL);
// 		if (execlp("exec", "exec", "python3", test_path_valide.c_str(), ">", "cgi_py.html", NULL) == -1)
//             std::cout << RED "Error execve" NONE << std::endl;

//         // if (execve(arg[0], arg, datas->env) == -1)
//         //     std::cout << RED "Error execve" NONE << std::endl;
// 	}
// 	else
// 	{
// 		std::cout << "********  end fork *************" << std::endl;
// 		waitpid(pid, NULL, 0);
// 	}  


    std::string file;
    char		c;

    // création du scipt.sh
    file = "#!/bin/bash\n\n";
    file += "exec python3 " + test_path_valide + " > temp.html\n\n";

    std::ofstream	ofs("script_cgi_py.sh", std::ios::out | std::ios::trunc);
	if (!ofs)
		std::cout << RED "Error file out" NONE << std::endl;
	ofs << file << std::endl;
	ofs.close();


    system("./script_cgi_py.sh");
    file = "";
    std::ifstream my_flux("temp.html");
	if (!my_flux)
		std::cout << RED "file no found !!!" NONE << std::endl;
	while (my_flux.get(c))
		file += c;
	my_flux.close();

    remove("temp.html");
    remove("script_cgi_py.sh");

    return (file);
}