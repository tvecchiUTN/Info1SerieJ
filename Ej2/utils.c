#include <stdlib.h>
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