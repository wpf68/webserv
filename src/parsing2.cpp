#include "webserv.h"

int	create_struct(std::vector<s_parsing> &parsing, int nb_serv)
{
	if (nb_serv == 0)
	{
		std::cout << "No server found" << std::endl;
		return (1);
	}
	for (int i = 0; i < nb_serv; i++)
			parsing.push_back(s_parsing());
	return (0);
}

void	find_ip(std::vector<std::string> servers, std::vector<s_parsing> &parsing, int nb_serv)
{
//	std::cout << servers[0] << std::endl;
	for (int i = 0; i < nb_serv; i++)
	{
		int				start = 0;
		int				end = 0;
		std::string		target = "listen";
		std::string		res;

		start = servers[i].find("listen", start); //trouve target
		if (start == -1)
			exit(0);
		start = start + target.length(); // avance la taille de target
		while (servers[i][start] == ' ' or servers[i][start] == '	')
			start++; // avance les espaces et tabs
		end = start;
		while (servers[i][end] != ':' )
			end++; // avance juqu'a la fin de res
		res = servers[i].substr(start, end - start); // prend res
		parsing[i].my_ip = res; // met res dans la struct
	}
}

void	find_port(std::vector<std::string> servers, std::vector<s_parsing> &parsing, int nb_serv)
{
	for (int i = 0; i < nb_serv; i++)
	{
		int				start = 0;
		int				end = 0;
		std::string		target = "listen";
		std::string		res;

		start = servers[i].find("listen", start); //trouve target
		if (start == -1)
			exit(0);
		start = start + target.length(); // avance la taille de target
		while (servers[i][start] != ':')
			start++; // avance
		start++;
		end = start;
		while (servers[i][end] != ';' && servers[i][end] != ' ')
			end++; // avance juqu'a la fin de res
		res = servers[i].substr(start, end - start); // prend res
		parsing[i].my_port = res; // met res dans la struct
	}
}

void	find_name(std::vector<std::string> servers, std::vector<s_parsing> &parsing, int nb_serv)
{
	for (int i = 0; i < nb_serv; i++)
	{
		int				start = 0;
		int				end = 0;
		std::string		target = "server_name";
		std::string		res;

		start = servers[i].find(target, start); //trouve target
		if (start == -1)
			exit(0);
		start = start + target.length(); // avance la taille de target
		while (servers[i][start] == ' ' or servers[i][start] == '	')
			start++; // avance
		end = start;
		while (servers[i][end] != ';' && servers[i][end] != ' ')
			end++; // avance juqu'a la fin de res
		res = servers[i].substr(start, end - start); // prend res
		parsing[i].name_server = res; // met res dans la struct
//		std::cout << parsing[i].name_server << std::endl;
	}
}

void	find_size(std::vector<std::string> servers, std::vector<s_parsing> &parsing, int nb_serv)
{
	for (int i = 0; i < nb_serv; i++)
	{
		int				start = 0;
		int				end = 0;
		std::string		target = "client_max_body_size";
		std::string		res;

		start = servers[i].find(target, start); //trouve target
		if (start == -1)
			exit(0);
		start = start + target.length(); // avance la taille de target
		while (servers[i][start] == ' ' or servers[i][start] == '	')
			start++; // avance
		end = start;
		while (servers[i][end] != ';' && servers[i][end] != ' ')
			end++; // avance juqu'a la fin de res
		res = servers[i].substr(start, end - start); // prend res
		parsing[i].size = stoi(res); // met res dans la struct
	}
}

void	find_error(std::vector<std::string> servers, std::vector<s_parsing> &parsing, int nb_serv)
{

	for (int i = 0; i < nb_serv; i++)
	{
		int				start = 0;
		int				end = 0;
		std::string		target = "error_page";
		std::string		key;
		std::string		res;

		while ((start = servers[i].find(target, start)) != std::string::npos)
		{
			start = start + target.length(); // avance la taille de target
			while (servers[i][start] == ' ' or servers[i][start] == '	')
				start++; // avance
			end = start;
			while (servers[i][end] != '	' && servers[i][end] != ' ')
				end++; // avance juqu'a la fin de key
			key = servers[i].substr(start, end - start); // prend key
			start = end;
			while (servers[i][start] == ' ' or servers[i][start] == '	')
				start++;
			end = start;
			while (servers[i][end] != ' ' && servers[i][end] != ';')
				end++;
			res = servers[i].substr(start, end - start);
			parsing[i].error[key] = res;
		}
	}
}
