#include <stdio.h>
#include <stdlib.h>
#include "network.h"


int main(int argc, char **argv)
{
    if(argc < 3)
    {
        printf("Missing arguments\n");
        return 1;
    }

    char* IPAdrres;
    int puerto = 8008;

    for(int i = 0; i < argc; i++)
    {
        if(i == 1)
        {
            IPAdrres = argv[1];
        }
        else if(i == 2)
        {
            puerto = atoi(argv[2]);
        }
    }

    int sk; 
    sk = ConnectToServer(IPAdrres, puerto);
    if(sk < 0)
    {
        printf("Error conectando con servidor\n");
        exit(0);
    }

    printf("Se ha conectado con el servidor\n");

    char rxTime[64];

    recv(sk, rxTime, 64, 0);

    printf("Hoy es %s\n", rxTime);

    DisconnectFromServer(sk);
}