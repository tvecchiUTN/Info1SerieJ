#include "utils.h"
#include "funcsServer.h"
#include "../network.h"

int funcInvertir(int littleSk)
{
    char rxStr[1024] = {0};
    char txStr[1024] = {0};

    int err;
    err = checkRecv(rxStr, littleSk);

    switch (err)
    {
        case OK:
            invertir(rxStr, txStr);
            send(littleSk, txStr, strlen(txStr), 0);
        break;

        case TERMINADOR:
            send(littleSk, MSGADIOS, strlen(MSGADIOS), 0);
            CloseConnect(littleSk);
        break;

        case ERR_LOST_CONECTION:
            printf("Lost conection with client\n");
            CloseConnect(littleSk);
        break;

        case ERR_TIME_LIMIT:
            send(littleSk, MSGTIMEOUT, strlen(MSGTIMEOUT), 0);
            CloseConnect(littleSk);
        break;

        case ERR_OTHER:
            printf("Hubo un error, revisar problemas\n");
        break;
    }
}