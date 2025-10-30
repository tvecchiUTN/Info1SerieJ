#include <stdio.h>
#include <unistd.h>
#include "utils.h"
#include "../network.h"

#define ESPERA 10

int main(int argc, char **argv)
{
    int puerto;

    if(checkPuerto(argc, argv, &puerto) == ERR_MISSING_ARGS)
    {
        fprintf(stderr, "Arguments ERROR\n");
        return ERR;
    }


    int mainSK = OpenServer(puerto);
    if(mainSK < 0)
    {
        fprintf(stderr, "Error iniciando socket\n");
        return ERR;
    }

    while(1)
    {
        int littleSK = WaitConnect(mainSK);
        if(littleSK < 0)
        {
            fprintf(stderr, "Error conectando a ususario\n");
            continue;
        }
        printf("Conexion exitosa\n");

        int len = -1;

        int contador = 0;

        char rxStr[1024] = {0}; 
        while((len < 0) && (contador < ESPERA))
        {
            len = recv(littleSK, rxStr, 1024, MSG_DONTWAIT);
            if(len >= 0)
            {
                break;
            }

            sleep(1);
            contador++;
        }

        char txStr[1024];

        if(len < 0)
        {
            fprintf(stderr, "Usuario tardo mucho\n");
            strcpy(txStr, "Has tardado mucho, conexion cerrada\n");

            send(littleSK, txStr, strlen(txStr), 0);

            CloseConnect(littleSK);
            continue;
        }else if(!len)
        {
            fprintf(stderr, "Conexion perdida con cliente\n");
            CloseConnect(littleSK);
            continue;
        }

        rxStr[len] = '\0';

        invertir(rxStr, txStr);

        send(littleSK, txStr, strlen(txStr), 0);

        CloseConnect(littleSK);
    }

    CloseServer(mainSK);
}