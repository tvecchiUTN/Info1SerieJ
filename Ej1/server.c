#include <stdlib.h>
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

    int puerto = 8008;
    for(int i = 0; i < argc; i++)
    {
        if(i == 1)
        {
            puerto = atoi(argv[1]);
        }
    }

    int sk = OpenServer(PUERTO);
    if (sk < 0)
    {
        fprintf(stderr, "Error iniciando servidor\n");
        return 1;
    }
    while (1)
    {
        char fecha[32];
        char hora[16];

        getCurrentTime(fecha, hora);
    }

    // write(fd, )
}