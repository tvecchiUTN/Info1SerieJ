#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int OpenServer(int portnr);
int CloseServer(int socketfd);


int WaitConnect (int msSock);
int CloseConnect(int sockfd);

int ConnectToServer(char * Server, int Port);
int DisconnectFromServer(int socketfd);

