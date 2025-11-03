#ifndef __FUNCSSERVER_H
#define __FUNCSSERVER_H

#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define TIMELIMIT 120

#define MSGTIMEOUT "Tiempo excedido, por favor hagalo mas rapido la proxima que no hay mucho tiempo\n"

#define MSGADIOS "Palabra terminadora escrita, vuelva pronto\n"

#define OK 1
#define TERMINADOR 0
#define ERR_LOST_CONECTION -1
#define ERR_TIME_LIMIT -2
#define ERR_OTHER -3

extern volatile int crtNew;
extern pthread_mutex_t myMutex;

void *hiloDialogo(void *args);

int funcInvertir(int littleSk);

#endif