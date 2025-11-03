#include <stdlib.h>
#include "../network.h"
#include "funcsServer.h"
#include "utils.h"

int checkPuerto(int argc, char **argv, int *puerto)
{
    if(argc != 2)
    {
        return ERR_MISSING_ARGS;
    }

    *puerto = atoi(argv[1]);

    if((*puerto <= 1024 ) || (*puerto >= 49152))
    {
        *puerto = 0;
        return ERR_PUERTO; //Error por numero de puerto, tiene que estar entre 1024 y 49152
        //Puerto recomendado: 8008
    }

    return OK;
}

int fixStr(char* s)
{
    int i;
    for(i = 0; s[i]; i++)
    {
        if(s[i] < ' ')
        {
            s[i] = '\0';
            return OK;
        }
    }
    return ERR;
}

int casesError(int err, int skFd)
{
    switch (err)
    {
        case OK:
            return OK;
        break;

        case ERR_LOST_CONECTION:
            printf("Lost conection with client\n");
            return ERR;
        break;

        case ERR_TIME_LIMIT:
            send(skFd, MSGTIMEOUT, strlen(MSGTIMEOUT), 0);
            return ERR;
        break;

        case ERR_OTHER:
            printf("Hubo un error, revisar problemas\n");
        break;
    }

    return ERR;
}

int invertir(const char* src, char* dest)
{
    int j = strlen(src) - 1;
    if(j >= 1024)
    {
        return ERR;
    }
    int i;
    for(i = 0; src[i]; i++)
    {
        dest[i] = src[j];
        j--;
    }
    dest[i] = '\0';
    return OK;
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
