#include "webserv.h"

void number_server(std::vector<std::string> servers, std::vector<s_parsing> &parsing)
{
	int 	nb_loc;

	for (int i = 0; i < servers.size(); i++) // pour chaque server
	{
		int 						nb_loc = 0;
		std::string::size_type		pos = 0;
		std::vector<int> 			tab_len;
		std::vector<std::string>	tab_loc;
		std::string					target = "location";

		while ((pos = servers[i].find(target, pos )) != std::string::npos) // compte nb local + pos
		{
			nb_loc++;
			tab_len.push_back(pos);
			pos += target.length();
		}
		tab_len.push_back(servers[i].size());

		for (int j = 0; j < tab_len.size() -1; j++) // découpe les loc pour mettre dans tableau de loc
			tab_loc.push_back(servers[i].substr(tab_len[j], tab_len[j +1] - tab_len[j]));

		parsing[i].locations = tab_loc;

		for (int j = 0; j < nb_loc; j++) // creation du bon nombre de struct location dans struct parsing.
			parsing[i].location.push_back(s_location());
	}
}

void	find_req_client(std::vector<s_parsing> &parsing)
{
	for (int i = 0; i < parsing.size(); i++)
	{
		for (int j = 0; j < parsing[i].location.size(); j++)
		{
			int				start = 0;
			int				end = 0;
			std::string		target = "location";
			std::string		res;

			start = parsing[i].locations[j].find(target, start); //trouve target
			start = start + target.length(); // avance la taille de target
			while (parsing[i].locations[j][start] == ' ' or parsing[i].locations[j][start] == '	')
				start++; // avance
			end = start;
			while (parsing[i].locations[j][end] != ';' && parsing[i].locations[j][end] != ' ')
				end++; // avance juqu'a la fin de res
			res = parsing[i].locations[j].substr(start, end - start); // prend res
			parsing[i].location[j].req_client = res;
		}
	}
}

void	find_root(std::vector<s_parsing> &parsing)
{
	for (int i = 0; i < parsing.size(); i++)
	{
		for (int j = 0; j < parsing[i].location.size(); j++)
		{
			int				start = 0;
			int				end = 0;
			std::string		target = "root";
			std::string		res;

			while ((start = parsing[i].locations[j].find(target, start)) != std::string::npos)
			{
				start = start + target.length(); // avance la taille de target
				while (parsing[i].locations[j][start] == '	' or parsing[i].locations[j][start] == ' ')
					start++; // avance
				end = start;
				while (parsing[i].locations[j][end] != ';' && parsing[i].locations[j][end] != ' ')
					end++; // avance juqu'a la fin de res
				res = parsing[i].locations[j].substr(start, end - start); // prend res
				parsing[i].location[j].root = res;
			}
		}
	}
}

void	find_index(std::vector<s_parsing> &parsing)
{
	for (int i = 0; i < parsing.size(); i++)
	{
		for (int j = 0; j < parsing[i].location.size(); j++)
		{
			int				start = 0;
			int				end = 0;
			std::string		target = "index";
			std::string		res;

			while ((start = parsing[i].locations[j].find(target, start)) != std::string::npos)
			{
				start = start + target.length(); // avance la taille de target
				while (parsing[i].locations[j][start] == '	' or parsing[i].locations[j][start] == ' ')
					start++; // avance
				end = start;
				while (parsing[i].locations[j][end] != ';' && parsing[i].locations[j][end] != ' ')
					end++; // avance juqu'a la fin de res
				res = parsing[i].locations[j].substr(start, end - start); // prend res
				start = end;
			//	parsing[i].location[j].path_index = parsing[i].location[j].root + '/' +res;
				parsing[i].location[j].path_index = res;
			}
		}
	}
}

void	find_dir_listing(std::vector<s_parsing> &parsing)
{
	for (int i = 0; i < parsing.size(); i++)
	{
		for (int j = 0; j < parsing[i].location.size(); j++)
		{
			int				start = 0;
			int				end = 0;
			std::string		target = "directory_listing";
			std::string		res;

			while ((start = parsing[i].locations[j].find(target, start)) != std::string::npos)
			{
				start = start + target.length(); // avance la taille de target
				while (parsing[i].locations[j][start] == '	' or parsing[i].locations[j][start] == ' ')
					start++; // avance
				end = start;
				while (parsing[i].locations[j][end] != ';' && parsing[i].locations[j][end] != ' ')
					end++; // avance juqu'a la fin de res
				res = parsing[i].locations[j].substr(start, end - start); // prend res
				parsing[i].location[j].dir_listing = res;
			}
		}
	}
}

void	find_dir_stock(std::vector<s_parsing> &parsing)
{
	for (int i = 0; i < parsing.size(); i++)
	{
		for (int j = 0; j < parsing[i].location.size(); j++)
		{
			int				start = 0;
			int				end = 0;
			std::string		target = "directory_stock";
			std::string		res;

			while ((start = parsing[i].locations[j].find(target, start)) != std::string::npos)
			{
				start = start + target.length(); // avance la taille de target
				while (parsing[i].locations[j][start] == '	' or parsing[i].locations[j][start] == ' ')
					start++; // avance
				end = start;
				while (parsing[i].locations[j][end] != ';' && parsing[i].locations[j][end] != ' ')
					end++; // avance juqu'a la fin de res
				res = parsing[i].locations[j].substr(start, end - start); // prend res
				parsing[i].location[j].dir_stock = res;
			}
		}
	}
}

void	find_methods(std::vector<s_parsing> &parsing)
{
	for (int i = 0; i < parsing.size(); i++)
	{
		for (int j = 0; j < parsing[i].location.size(); j++)
		{
			int				start = 0;
			int				end = 0;
			std::string		target = "	allow_methods";
			std::string		res;

			while ((start = parsing[i].locations[j].find(target, start)) != std::string::npos)
			{
				start = start + target.length(); // avance la taille de target
				while (parsing[i].locations[j][start] == '	' or parsing[i].locations[j][start] == ' ')
					start++; // avance
				end = start;
				while (parsing[i].locations[j][end] != ';' && parsing[i].locations[j][end] != ' ')
					end++; // avance juqu'a la fin de res
				res = parsing[i].locations[j].substr(start, end - start); // prend res
//				std::cout << res << std::endl;
				parsing[i].location[j].methods = res;
			}
		}
	}
}

void	find_redir(std::vector<s_parsing> &parsing)
{
	for (int i = 0; i < parsing.size(); i++)
	{
		for (int j = 0; j < parsing[i].location.size(); j++)
		{
			int				start = 0;
			int				end = 0;
			std::string		target = "return";
			std::string		key;
			std::string		res;

			while ((start = parsing[i].locations[j].find(target, start)) != std::string::npos)
			{
				start = start + target.length(); // avance la taille de target
				while (parsing[i].locations[j][start] == ' ' or parsing[i].locations[j][start] == '	')
					start++; // avance
				end = start;
				while (parsing[i].locations[j][end] != '	' && parsing[i].locations[j][end] != ' ')
					end++; // avance juqu'a la fin de key
				key = parsing[i].locations[j].substr(start, end - start); // prend key
				start = end;
				while (parsing[i].locations[j][start] == ' ' or parsing[i].locations[j][start] == '	')
					start++;
				end = start;
				while (parsing[i].locations[j][end] != ' ' && parsing[i].locations[j][end] != ';')
					end++;
				res = parsing[i].locations[j].substr(start, end - start);
				parsing[i].location[j].redir[key] = res;
			}
		}
	}
}

void	find_cgi(std::vector<s_parsing> &parsing)
{
	for (int i = 0; i < parsing.size(); i++)
	{
		for (int j = 0; j < parsing[i].location.size(); j++)
		{
			int				start = 0;
			int				end = 0;
			std::string		target = "cgi_info";
			std::string		key;
			std::string		res;

			while ((start = parsing[i].locations[j].find(target, start)) != std::string::npos)
			{
				start = start + target.length(); // avance la taille de target
				while (parsing[i].locations[j][start] == ' ' or parsing[i].locations[j][start] == '	')
					start++; // avance
				end = start;
				while (parsing[i].locations[j][end] != '	' && parsing[i].locations[j][end] != ' ')
					end++; // avance juqu'a la fin de key
				key = parsing[i].locations[j].substr(start, end - start); // prend key
				start = end;
				while (parsing[i].locations[j][start] == ' ' or parsing[i].locations[j][start] == '	')
					start++;
				end = start;
				while (parsing[i].locations[j][end] != ' ' && parsing[i].locations[j][end] != ';')
					end++;
				res = parsing[i].locations[j].substr(start, end - start);
				parsing[i].location[j].cgi[key] = res;
			}
		}
	}
}

int	ft_check_parsing(std::vector<s_parsing> parsing)
{
	for (int i = 0; i < parsing.size(); i++)
	{
		struct sockaddr_in sa;
	    int result = inet_pton(AF_INET, parsing[i].my_ip.c_str(), &(sa.sin_addr));
    	if (result == 0)
		{
			std::cout << "IP parsing error" << std::endl;
			return (1);
		}

		for (int j = 0; j < parsing[i].my_port.size(); j++)
		{
			if (isalpha(parsing[i].my_port[j]))
			{
				std::cout << "Port parsing error" << std::endl;
				return (1);
			}
		}

		for (int j = 0; j < parsing[i].my_port.size(); j++)
		{
			if (stoi(parsing[i].my_port) < 0 || stoi(parsing[i].my_port) > 65353)
			{
				std::cout << "Port parsing error" << std::endl;
				return (1);
			}
		}
	}

	return (0);
}
