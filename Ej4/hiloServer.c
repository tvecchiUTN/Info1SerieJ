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
        // Seccion de creacion de hilo
        int littleSk = WaitConnect(fd);
        if (flagCreate)
        {
            pthread_mutex_lock(&myMutex);
            crtNew = 1;
            pthread_mutex_unlock(&myMutex);
            flagCreate = 0;
        }

        int finalizar = 1;

        while (finalizar)
        {
            // Seccion de analisis de mensaje
            char msgRx[32] = {0};
            send(littleSk, "Ingrese el comando: ", 21, 0);
            int err;
            err = checkRecv(msgRx, 32, littleSk);
            if(!casesError(err, littleSk))
            {
                finalizar = 0;
                continue;
            }
            printf("Msg recibed -->%s", msgRx);

            if(!strcmp(msgRx, "#out"))
            {
                send(littleSk, MSGADIOS, strlen(MSGADIOS), 0);
                break;
            }

            // Seccion array de funciones
            int (*vecFuncs[])(int) = {funcInvertir, NULL};
            int sz = 0;
            for (int sz = 0; vecFuncs[sz]; sz++);

            // Seccion de lectura de comandos
            int cmdFd = open("comandos.txt", O_RDONLY);
            if (cmdFd < 0)
            {
                printf("NO se pudo abrir el archivo\n");
                // Supuestamente si no se pudo abrir puedo dejar unos comandos ya implementados
                // Pero tomara tiempo y es opcional
            }

            // Seccion de ejecucion de funciones
            int i = 0;
            while (i >= 0)
            {
                char cmdStr[16] = {0};

                //Lectura de comandos
                if (read(cmdFd, cmdStr, 16) < 0)
                {
                    send(littleSk, "Comando no encontrado", 22, 0);
                    break;
                }
                fixStr(cmdStr);

                //Analisis de comandos
                if (!strcmp(msgRx, cmdStr))
                {
                    if (i < sz)
                    {
                        int ret = vecFuncs[i](littleSk);
                        if (!ret)
                        {
                            break;
                        }
                    }
                }
                i++;
            }
        }

        CloseConnect(littleSk);
    }

    pthread_exit(NULL);
}