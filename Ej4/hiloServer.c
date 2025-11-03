#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include "../network.h"
#include "utils.h"
#include "funcsServer.h"

void *hiloDialogo(void *args)
{
    int fd = *(int *)args;

    int flagCreate = 1;

    while (1)
    {
        //Seccion de creacion de hilo
        int littleSk = WaitConnect(fd);
        if (flagCreate)
        {
            pthread_mutex_lock(&myMutex);
            crtNew = 1;
            pthread_mutex_unlock(&myMutex);
            flagCreate = 0;
        }

        //Seccion de analisis de mensaje
        char msgRx[32] = {0};
        checkRecv(msgRx, 32, littleSk);
        fixStr(msgRx);

        //Seccion array de funciones
        int (*vecFuncs[])(int) = {funcInvertir, NULL};
        int sz = 0;
        for(int sz = 0; vecFuncs[sz]; sz++);

        //Seccion de lectura de comandos
        int cmdFd = open("comandos.txt", O_RDONLY);
        if (cmdFd < 0)
        {
            printf("NO se pudo abrir el archivo\n");
            // Supuestamente si no se pudo abrir puedo dejar unos comandos ya implementados
            // Pero tomara tiempo y es opcional
        }

        //Seccion de ejecucion de funciones
        int i = 0;
        while (1)
        {
            char cmdStr[16] = {0};

            if(read(cmdFd, cmdStr, 16) < 0)
            {
                break;
            }

            fixStr(cmdStr);

            if(!strcmp(msgRx, cmdFd))
            {
                if(i < sz)
                {
                    vecFuncs[i](littleSk);
                }
            }
            i++;
        }

        CloseConnect(littleSk);
    }

    pthread_exit(NULL);
}

int checkRecv(char *strRx, int sz, int fd)
{
    int len = -1;

    int crono = 0;
    while ((len < 0) && crono <= (TIMELIMIT * 2))
    {
        len = recv(fd, strRx, sz, MSG_DONTWAIT);
        if (len >= 0)
        {
            break;
        }

        crono++;

        usleep(500000);
    }

    if (len > 0)
    {
        fixStr(strRx);

        if (!strncmp(strRx, "Chau", 4))
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
