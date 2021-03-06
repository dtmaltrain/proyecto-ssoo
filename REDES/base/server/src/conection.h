#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "comunication.h"
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO

int n_connected;
int server_socket;

//http://manpages.ubuntu.com/manpages/trusty/man7/sys_socket.h.7posix.html
//https://pubs.opengroup.org/onlinepubs/009695399/basedefs/sys/types.h.html
//https://pubs.opengroup.org/onlinepubs/009695399/basedefs/netinet/in.h.html
//https://pubs.opengroup.org/onlinepubs/7908799/xns/arpainet.h.html
//https://pubs.opengroup.org/onlinepubs/7908799/xns/netdb.h.html

typedef struct players_info
{
  int socket_c1;
  int socket_c2;
  int socket_c3;
  int socket_c4;
} PlayersInfo;

PlayersInfo *prepare_sockets_and_get_clients(char *IP, int port);
PlayersInfo *add_new_clients(PlayersInfo *sockets_clients);