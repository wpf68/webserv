
#include <iostream>
#include <cstdlib>

int main(void)
{
	std::string requete_client = "./script_cgi.sh";  // parsing[0].location[0].cgi["py"]
													 // ou
													 // parsing[i].location[j].cgi["py"] (si boucle)
	
	std::string chemin_a_renvoyer = "./cgi.html";	

	system("./script_cgi.sh");


	return (0);
}

