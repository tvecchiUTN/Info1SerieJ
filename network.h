#ifndef __NETWORK_H 
#define __NETWORK_H

#include <sys/socket.h> // Necesario para las estructuras y funciones de sockets (socket, bind, listen, accept, connect).
#include <netinet/in.h> // Necesario para las estructuras sockaddr_in y sockaddr_in6, y funciones como htons.
#include <arpa/inet.h>  // Necesario para funciones de manipulación de direcciones IP como inet_ntoa, inet_ntop.

/**
 * @brief Abre un socket de servidor y lo prepara para escuchar conexiones entrantes.
 * @param portnr El número de puerto en el que el servidor escuchará.
 * @return El descriptor de archivo (file descriptor) del socket del servidor si tiene éxito, -1 en caso de error.
 */
int OpenServer(int portnr);

/**
 * @brief Cierra el socket principal del servidor (el que escucha).
 * @param socketfd El descriptor de archivo del socket del servidor a cerrar.
 * @return 0 si tiene éxito, -1 en caso de error.
 */
int CloseServer(int socketfd);

/**
 * @brief Espera (bloquea) hasta que un cliente se conecte al servidor.
 * @param msSock El descriptor de archivo del socket del servidor que está escuchando.
 * @return El descriptor de archivo del nuevo socket creado para la conexión con el cliente si tiene éxito, -1 en caso de error.
 */
int WaitConnect (int msSock);

/**
 * @brief Cierra la conexión con un cliente específico.
 * @param sockfd El descriptor de archivo del socket de la conexión con el cliente a cerrar.
 * @return 0 si tiene éxito, -1 en caso de error.
 */
int CloseConnect(int sockfd);


// --- Funciones del Cliente ---

/**
 * @brief Establece una conexión con un servidor remoto.
 * @param Server La dirección IP o nombre de host del servidor.
 * @param Port El número de puerto del servidor.
 * @return El descriptor de archivo del socket conectado al servidor si tiene éxito, 0 o un valor negativo en caso de error.
 */
int ConnectToServer(char * Server, int Port);

/**
 * @brief Cierra la conexión con el servidor.
 * @param socketfd El descriptor de archivo del socket conectado al servidor.
 * @return 0 si tiene éxito, -1 en caso de error.
 */
int DisconnectFromServer(int socketfd);

#endif