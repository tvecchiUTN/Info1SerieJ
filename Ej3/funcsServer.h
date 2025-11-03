#ifndef __FUNCSSERVER_H
#define __FUNCSSERVER_H

#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define STRTERMINADOR "Chau\13\10"

#define OK 1
#define TERMINADOR 0
#define ERR_LOST_CONECTION -1
#define ERR_TIME_LIMIT -2
#define ERR_OTHER -3

extern volatile int crtNew;
extern pthread_mutex_t myMutex;

void *hiloDialogo(void *args);

#endif