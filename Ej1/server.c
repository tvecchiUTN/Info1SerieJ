#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "getTime.h"
#include "network.h"

#define PUERTO 8008

int main(int argc, char **argv)
{
    if(argc < 2)
    {
        printf("Missing arguments\n");
        exit(0);
    }

    int cd;
    int puerto = PUERTO;
    for(int i = 0; i < argc; i++)
    {
        if(i == 1)
        {
            puerto = atoi(argv[1]);
        }
    }

    int sk = OpenServer(puerto);
    if (sk < 0)
    {
        fprintf(stderr, "Error iniciando servidor\n");
        return 1;
    }

    printf("Socket creado\n");

    while (1)
    {

        cd = WaitConnect(sk);
        if(cd < 0)
        {
            perror("Wait Connect");
            break;
        }

        char sSend[64] = {0};

        printf("Se conecto un cliente\n");

        getCurrentTime(sSend);

        send(cd, sSend, strlen(sSend), 0);

        printf("Mensaje enviado.\n Cerrando\n");

        CloseConnect(cd);
    }

    CloseServer(sk);

}