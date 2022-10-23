/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pwolff <pwolff@student.42mulhouse.fr>>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/23 11:28:18 by pwolff            #+#    #+#             */
/*   Updated: 2022/10/23 11:28:18 by pwolff           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

    #include <sys/types.h> 
    #include <sys/socket.h> 
    #include <stdio.h> 
    #include <stdlib.h> 
    #include <unistd.h> 
    #include <string.h> 
    #include <netinet/ip.h>

    #include <string>

    #define MYPORT 8003


  //  #define DEST_IP   "216.58.213.78"
  //  #define DEST_PORT 8003

    #include <arpa/inet.h>

void    ft_error(int error)
{
    printf("Error : %d\n", error);
    exit (1);

}


int     main()
{
    int     fd;
    struct  sockaddr_in my_addr;

    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        ft_error(1);

    printf("fd = %d\n", fd);

    my_addr.sin_family = AF_INET;     /* host byte order */
    my_addr.sin_port = htons(MYPORT); /* short, network byte order */
  //  my_addr.sin_addr.s_addr = inet_addr("132.241.5.10");
    my_addr.sin_addr.s_addr = htonl(INADDR_ANY);  /* use my IP adresse */
    bzero(&(my_addr.sin_zero), 8);    /* zéro pour le reste de la struct */

    /* ne pas oublier les test d'erreur pour bind(): */
    if ((bind(fd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr))) == -1)
        ft_error(2);


    // struct sockaddr_in dest_addr;   /* Contiendra l'adresse de destination */

    // dest_addr.sin_family = AF_INET;        /* host byte order */
    // dest_addr.sin_port = htons(DEST_PORT); /* short, network byte order */
    // dest_addr.sin_addr.s_addr = inet_addr(DEST_IP);
    // bzero(&(dest_addr.sin_zero), 8);       /* zéro pour le reste de la struct */

    // /* ne pas oublier les tests d'erreur pour connect()! */
    // if ((connect(fd, (struct sockaddr *)&dest_addr, sizeof(struct sockaddr))) == -1)
    //     ft_error(3);

    int new_fd;
    struct sockaddr their_addr; /* Informations d'adresse du client */
    socklen_t sin_size;

    listen(fd, 10);

    sin_size = sizeof(struct sockaddr_in);
    new_fd = accept(fd, &their_addr, &sin_size);

    printf("new_fd = %d\n", new_fd);

   
  //  std::string msg ("Beej était là!");
    int len, bytes_sent;
    
    len = strlen("Beej était là!");
    bytes_sent = send(new_fd, "Beej était là!", len, 0);








    shutdown (new_fd, 2);
    shutdown (fd, 2);

    return (0);
}
    




/*         *********    donne IP adresse d'un Host name   **********    */
/*         *********    google.com   ==>   216.58.213.78  **********    */
/*
    #include <stdio.h> 
    #include <stdlib.h> 
    #include <errno.h> 
    #include <netdb.h> 
    #include <sys/types.h>
    #include <netinet/in.h> 

    #include <sys/socket.h>
    #include <arpa/inet.h>

    int main(int argc, char *argv[])
    {
        struct hostent *h;


        if (argc != 2) {  // Vérification d'erreurs de la ligne de commande 
            fprintf(stderr,"usage: getip adresse\n");
            exit(1);
        }

        if ((h=gethostbyname(argv[1])) == NULL) {  // récupérer infos de l'hôte 
            herror("gethostbyname");
            exit(1);
        }

        printf("Host name  : %s\n", h->h_name);
        printf("IP adresse : %s\n",inet_ntoa(*((struct in_addr *)h->h_addr)));

        char *ip;

        ip = (inet_ntoa(*((struct in_addr *)h->h_addr)));
        printf("adresse : %s\n", ip);

        struct sockaddr_in  ina;
        ina.sin_addr.s_addr = inet_addr(ip);
        printf("IP long : %d\n", ina.sin_addr.s_addr);
        printf("IP long to ASCII : %s\n", inet_ntoa(ina.sin_addr));

        return 0;
    }
*/

/*

int main(int argc, char **argv)
{
    (void) argc;
    (void) argv;



    return (0);
}*/