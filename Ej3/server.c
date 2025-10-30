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

pthread_mutex_t myMutex;

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

        send(littleSk, "Prueba 1 2 3", 13, 0);

        sleep(60);

        CloseConnect(littleSk);
    }

    pthread_exit(NULL);
}

int main(int argc, char **argv)
{
    int puerto;
    if (checkPuerto(argc, argv, &puerto) < 0)
    {
        fprintf(stderr, "Error de argumentos\n");
        return ERR;
    }

    int capMax = 16;
    pthread_t *hilos = malloc(capMax * sizeof(pthread_t));
    int szHil = 0;
    int flagMax = 0;

    if (!hilos)
    {
        fprintf(stderr, "Error vector de hilos\n");
        return ERR;
    }

    pthread_mutex_init(&myMutex, NULL);

    int mainSK = OpenServer(puerto);

    while (1)
    {
        if (szHil >= capMax) // Seccion de reajuste de hilos
        {
            capMax *= 2;
            pthread_t *auxHilos = realloc(hilos, capMax * sizeof(pthread_t));
            if (!auxHilos)
            {
                printf("No se pudo reservar mas memoria\n");
                flagMax = 1;
            }
            else
            {
                hilos = auxHilos;
            }
        }

        if (crtNew) //Si puedo crar un nuevo hilo
        {
            if (!flagMax) //EN caso que no pueda guardar mas
            {
                pthread_create(hilos + szHil, NULL, hiloDialogo, &mainSK);
                pthread_detach(hilos + szHil);
                printf("Se creo un hilo para escuchar\n");
                szHil++;

                pthread_mutex_lock(&myMutex);
                crtNew = 0;
                pthread_mutex_unlock(&myMutex);
            }
        }

        sleep(1); //Reducir la cantidad de procesos
    }
}