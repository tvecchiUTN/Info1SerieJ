#include "utils.h"
#include "funcsServer.h"
#include "../network.h"

int funcInvertir(int littleSk)
{

    char msgRx[1024];
    char msgTx[1024];

    send(littleSk, "Ingrese texto a invertir: ", 27, 0);
    int err = checkRecv(msgRx, 1024, littleSk);

    if(!casesError(err, littleSk))
    {
        return ERR;
    }

    invertir(msgRx, msgTx);

    send(littleSk, msgTx, strlen(msgTx), 0);

    return OK;
}