#include <stdio.h>
#include <unistd.h>
#include "../network.h"
#include "utils.h"
#include "funcsServer.h"

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

        char msgRx[32] = {0};

        checkRecv(msgRx, littleSk);

        //Si yo pongo #inv, ejecuto la funcion que invierte
        
        int (*vecFuncs[])(int) = {funcInvertir, NULL};
        
        

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

        if(!strncmp(strRx, "Chau", 4))
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
