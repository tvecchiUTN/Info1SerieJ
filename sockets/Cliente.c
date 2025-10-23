#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "network.h"

int main (void)
{

	int fd;
	char IPDestino[25];
	char st[100];
	int len;

	printf(">Ingrese la dirección IP del servidor: ");
	fgets(IPDestino, 25, stdin);


	fd = ConnectToServer(IPDestino,3035);

	if (fd == 0)
	{
		perror("Error al conectar con el servidor");
		exit(0);
	}
	
	printf(">Conectado con el Servidor...\n");


	printf(">Ingrese una cadena de texto para enviar al servidor\n> ");
	fgets(st,100,stdin);
	len = strlen(st);
	len ++;	// Agrego para enviar el '/0'

	// Envío largo del mensaje
	write(fd,&len,sizeof(int));
	
	// Envío mensaje
	write(fd,st,len);

	printf(">Mensaje enviado...\n");

	sleep(2);

	printf(">Desconectando...\n");

	DisconnectFromServer(fd);
	
	printf(">Desconectado!\n");


}
