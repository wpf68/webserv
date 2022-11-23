#include "webserv.h"

std::string ft_read_file2(std::string file_path)
{
	char		c;
	std::string	file = "";

	std::ifstream content(file_path);
	if (!content)
		std::cout << "error_page" << std::endl;
	while (content.get(c))
		file += c;
	content.close();
	return (file);
}

int	number_server(std::string &file, std::vector<int> &tab_len)
{
	int 						occur = 0;
	std::string::size_type		pos = 0;
	std::string					target = "server {";

	while ((pos = file.find(target, pos )) != std::string::npos)
	{
		occur++;
		tab_len.push_back(pos);
		pos += target.length();
	}
		tab_len.push_back(file.size());
	return (occur);
}

void	cut_server(std::string &file, std::vector<int> &tab_len, std::vector<std::string> &servers)
{
	int	i = 0;

	for (int i = 0; i < tab_len.size() - 1; i++)
		servers.push_back(file.substr(tab_len[i], tab_len[i +1] - tab_len[i]));
}
