#include <stdio.h>
#include <unistd.h>
#include "../network.h"
#include "utils.h"
#include "funcsServer.h"

#define TIMELIMIT 120

#define MSGTIMEOUT "Tiempo excedido, por favor hagalo mas rapido la proxima que no hay mucho tiempo\n"

#define MSGADIOS "Palabra terminadora escrita, vuelva pronto\n"

int checkRecv(char *strRx, int fd);

void *hiloDialogo(void *args)
{
    int fd = *(int *)args;

    int flagCreate = 1;

    while (1)
    {
        int littleSk = WaitConnect(fd);

        if (flagCreate)
        {
            pthread_mutex_lock(&myMutex);
            crtNew = 1;
            pthread_mutex_unlock(&myMutex);
            flagCreate = 0;
        }

        int clientConect = 1;

        while (clientConect)
        {
            char rxStr[1024] = {0};
            char txStr[1024] = {0};

            int err;
            err = checkRecv(rxStr, littleSk);

            switch (err)
            {
                case OK:
                    invertir(rxStr, txStr);
                    send(littleSk, txStr, strlen(txStr), 0);
                    break;
                
                case TERMINADOR:
                    send(littleSk, MSGADIOS, strlen(MSGADIOS), 0);
                    CloseConnect(littleSk);
                    clientConect = 0;
                    break;

                case ERR_LOST_CONECTION:
                    printf("Lost conection with client\n");
                    CloseConnect(littleSk);
                    clientConect = 0;
                    break;

                case ERR_TIME_LIMIT:
                    send(littleSk, MSGTIMEOUT, strlen(MSGTIMEOUT), 0);
                    CloseConnect(littleSk);
                    clientConect = 0;
                    break;

                case ERR_OTHER:
                    printf("Hubo un error, revisar problemas\n");
                    clientConect = 0;
                    break;
            }
        }

        CloseConnect(littleSk);
    }

    pthread_exit(NULL);
}

int checkRecv(char *strRx, int fd)
{
    int len = -1;

    int crono = 0;
    while ((len < 0) && crono <= (TIMELIMIT*2))
    {
        len = recv(fd, strRx, 1023, MSG_DONTWAIT);
        if (len >= 0)
        {
            break;
        }

        crono++;

        usleep(500000);
    }

    if (len > 0)
    {
        strRx[len] = '\0';

        for(int i = 0; strRx[i]; i++)
        {
            printf("DEBUG STR --> %d\n", strRx[i]);
        }
        printf("\n");

        if(!strncmp(strRx, STRTERMINADOR, 4))
        {
            return TERMINADOR;
        }

        return OK;
    }
    else if (!len)
    {
        return ERR_LOST_CONECTION;
    }
    else if (len < 0)
    {
        return ERR_TIME_LIMIT;
    }

    return ERR_OTHER;
}
