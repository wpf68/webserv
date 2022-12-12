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

extern std::map<std::string, std::string>   var_content_code;


std::string CGI_py(std::string test_path_valide, t_client *datas)
{
//	if (test_path_valide.find(".py") != std::string::npos)
	{	
        char	   	c;
	    std::string	file = "";
		int			pid = 0;
		std::string source_bin = "";
		int			test_bin;

		for (int j = 0; j < datas->location.size(); j++)
		{
			if (datas->location[j].req_client.find("/cgi") != std::string::npos)
				source_bin = datas->location[j].cgi["py"];
		}
		
		std::cout << GREEN "path : " << test_path_valide << " source bin : " << source_bin << std::endl;
		if (source_bin == "" || !(exist_path(source_bin, NULL)))
		{
			datas->status = "503 " + var_content_code["503"] + " " + datas->name_server;
			datas->client_path = "HTML/503.html";
			datas->list_request_received.erase();
			std::ifstream my_flux2(datas->client_path);
			if (!my_flux2)
			{
				datas->status = "500 " + var_content_code["500"] + " " + datas->name_server;
				datas->client_path = datas->file_500;
				datas->list_request_received.erase();
				std::ifstream test_file(datas->file_500);
				if (!test_file)
					datas->client_path = datas->file_500_bis;
				else
					test_file.close();
				return (datas->file_500);
			}
			while (my_flux2.get(c))
			file += c;
			my_flux2.close();
			return (file);
		}
		pid = fork();
		if (pid < 0)
			std::cout << RED "Error fork" NONE << std::endl;
		if (pid == 0)
		{
			execlp(source_bin.c_str(), source_bin.c_str(), test_path_valide.c_str(), NULL);
		}
		else
		{
			std::cout << "********  end fork *************" << std::endl;
			waitpid(pid, NULL, 0);
		}

		file = "";
		std::ifstream my_flux("temp.html");
		if (!my_flux)
        {
            std::cout << RED "file no found !!!" NONE << std::endl;
            return (datas->file_500_bis);
        }
			
		while (my_flux.get(c))
			file += c;
		my_flux.close();

		remove("temp.html");
		remove("script_cgi_py.sh");
		return (file);
	}
}

std::string CGI_c(std::string test_path_valide, t_client *datas)
{
    char		c;
	std::string	file = "";
    int			pid = 0;
    std::string source_bin = "";
    int			test_bin;

    for (int j = 0; j < datas->location.size(); j++)
    {
        if (datas->location[j].req_client.find("/cgi") != std::string::npos)
            source_bin = datas->location[j].cgi["c"];
    }

    std::cout << GREEN "path : " << test_path_valide << " source bin : " << source_bin << std::endl;
    if (source_bin == "" || !(exist_path(source_bin, NULL)))
    {
        datas->status = "503 " + var_content_code["503"] + " " + datas->name_server;
        datas->client_path = "HTML/503.html";
        datas->list_request_received.erase();
        std::ifstream my_flux2(datas->client_path);
        if (!my_flux2)
        {
            datas->status = "500 " + var_content_code["500"] + " " + datas->name_server;
            datas->client_path = datas->file_500;
            datas->list_request_received.erase();
            std::ifstream test_file(datas->file_500);
            if (!test_file)
                datas->client_path = datas->file_500_bis;
            else
                test_file.close();
            return (datas->file_500);
        }
        while (my_flux2.get(c))
        file += c;
        my_flux2.close();
        return (file);
    }
    pid = fork();
    if (pid < 0)
        std::cout << RED "Error fork" NONE << std::endl;
    if (pid == 0)
    {
        execlp(source_bin.c_str(), source_bin.c_str(), test_path_valide.c_str(), NULL);
    }
    else
    {
        std::cout << "********  end fork *************" << std::endl;
        waitpid(pid, NULL, 0);
    }

    file = "";
    std::ifstream my_flux("temp.html");
    if (!my_flux)
    {
        std::cout << RED "file no found !!!" NONE << std::endl;
        return (datas->file_500_bis);
    }
    while (my_flux.get(c))
        file += c;
    my_flux.close();

    remove("temp.html");
    remove("script_cgi_c.sh");
    remove("sh_c");

    return (file);
}























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