#include <stdlib.h>
#include <stdio.h>
#include "network.h"

int main (void)
{

	int fd, len;
	char st[100];


	printf(">Esperando conexiones...\n");

	fd = OpenServer(3035);
	if(fd<0)
	{
	    printf("Error al iniciar el servidor\n");
	    exit(0);
	}

	printf(">Se conectó un cliente...\n>Esperando mensaje...\n");

	// Leo largo del mensaje a recibir
	read(fd,&len,sizeof(int));

	printf(">Se recibió largo de paquete: %d caracteres\n", len);

	// Leo mensaje
	read(fd,st,len);

	printf(">Mensaje recibido:\n%s\n>Cerrando conexión...\n", st);	

	CloseServer(fd);

	printf(">Desconectado!\n");

}
