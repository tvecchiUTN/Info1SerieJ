#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include "utils.h"
#include "../network.h"

/*
typedef struct
{
    int fd;
}param_t;
*/

volatile int crtNew = 1;

void* hiloDialogo(void* args)
{
    int fd = *(int*)args;

    while(1)
    {
        int littleSk = WaitConnect(fd);

        printf("Conectado\n");
        crtNew = 1;
        
        send(littleSk, "Prueba 1 2 3", 13, 0);

        sleep(10);

        CloseConnect(littleSk);
    }
}

int main(int argc, char **argv)
{
    int puerto;
    if(checkPuerto(argc, argv, &puerto) < 0)
    {
        fprintf(stderr, "Error de argumentos\n");
        return ERR;
    }

    pthread_t *hilos = malloc(10 * sizeof(pthread_t));
    int szHil = 0;
    if(!hilos)
    {
        fprintf(stderr, "Error vector de hilos\n");
        return ERR;
    }

    int mainSK = OpenServer(puerto);

    while(1)
    {
        if(crtNew)
        {
            pthread_create(hilos+szHil, NULL, hiloDialogo, &mainSK);
            szHil++;
            crtNew = 0;
        }
        sleep(1);
    }

}