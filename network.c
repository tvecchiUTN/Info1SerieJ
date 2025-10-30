#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <arpa/inet.h>
#include "network.h"

#define BACKLOG 10
#define DGB_NET 0

#define xperror(n) { perror(n); return -1;}

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}


int OpenServer(int portnr)
{
    int sockfd;    // listen on sock_fd
    struct sockaddr_in my_addr;     // my address information

//	int dummy=1;	//usado en setsockopt
    int porttype = SOCK_STREAM;

    char errorst[100];

				// AF_ o PF_INET (address or Protocol Family ...
    if ( (sockfd = socket(PF_INET, porttype, 0) ) == -1)
        xperror("Error try ing to set up socket...\n");

//    if (setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&dummy,sizeof(int)) == -1)
//        perror("Error trying to set REUSEADDR to new socket...\n");

    my_addr.sin_family = AF_INET;           // host byte order
    my_addr.sin_port = htons(portnr);       // short, network byte order
    my_addr.sin_addr.s_addr = INADDR_ANY;   // automatically fill with my IP
//    my_addr.sin_addr.s_addr = inet_addr("192.168.0.18");   
 
    memset(&(my_addr.sin_zero), 0, 8);      // zero the rest of the struct

    if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr))== -1)
    {       sprintf(errorst,"Error trying to bind new socket to address %s:%d",inet_ntoa(my_addr.sin_addr), portnr);
            xperror(errorst);
    }

#if DGB_NET  == 1
    printf ("DBG_NET(%04d) Server vinculado (bind) con port: %d\n",__LINE__,htons(portnr));
	printf ("\t %x %d / %x %d\n\n",htons(portnr),htons(portnr),portnr,portnr);

#endif    
    
    if (listen(sockfd, BACKLOG) == -1)
    {   sprintf(errorst, "Error trying to start listening from new socket (bound to port nr %d)", portnr) ;
        xperror(errorst);
    }

#if DGB_NET==1
printf ("DBG_NET(%04d) Server escuchando (listen) en port: %d\n",__LINE__,htons(portnr));

#endif    
    
    return  sockfd;
}
    
    
    
int WaitConnect (int msSock)
{
int new_fd;
//	struct sockaddr_storage their_addr;	// connector's information
	struct sockaddr_in their_addr;	// connector's information
    socklen_t sin_size = sizeof(struct sockaddr);

    // Queda a la espera de nuevas conexiones
    new_fd = accept(msSock, (struct sockaddr *)&their_addr, &sin_size);
    
#if DGB_NET==1
	char sAux[100];

	printf ("DBG_NET(%04d) Server - ",__LINE__);
	printf ("IP Origen: %s\n",inet_ntop(AF_INET, &their_addr.sin_addr, sAux, INET_ADDRSTRLEN));
	printf ("%s\n",sAux);
#endif    
	
	
	return new_fd;

}


int CloseConnect(int sockfd)
{
    return (close(sockfd));
}


int CloseServer(int sockfd)
{
    return (close(sockfd));
}


int ConnectToServer(char * Server, int Port)
{
    int sockfd;  
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char sPort[10];


    sprintf(sPort,"%d",Port);

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;


    if ((rv = getaddrinfo(Server, sPort, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 0;
    }

    // loop through all the results and connect to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next)
    {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
	{
            perror("client: socket");
            continue;
        }


        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1)
		{
            close(sockfd);
            perror("client: connect");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "client: failed to connect\n");
        return 0;
    }
    
#if DGB_NET  == 1
    char sAux[100];
//    printf ("DBG_NET(%04d) Conectandome a: \n",__LINE__);

//	inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr), s, sizeof s);

    printf ("DBG_NET(%04d) Conectandome a: %s\n",__LINE__,inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr), sAux, sizeof sAux));
#endif
	
	
    freeaddrinfo(servinfo); 
    
    return sockfd;
   
}

int DisconnectFromServer(int socketfd)
{
    return (close(socketfd));
}
