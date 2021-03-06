#include "conection.h"

//LINKS REFERENCIAS:
//https://www.man7.org/linux/man-pages/man2/socket.2.html
//https://man7.org/linux/man-pages/man7/socket.7.html
//https://www.man7.org/linux/man-pages/man3/setsockopt.3p.html
//https://man7.org/linux/man-pages/man2/setsockopt.2.html
//https://linux.die.net/man/3/htons
//https://linux.die.net/man/3/inet_aton
//https://www.howtogeek.com/225487/what-is-the-difference-between-127.0.0.1-and-0.0.0.0/
//https://www.man7.org/linux/man-pages/man2/bind.2.html
//https://www.man7.org/linux/man-pages/man2/accept.2.html

int n_connected = 0;
int server_socket = -1;

PlayersInfo *prepare_sockets_and_get_clients(char *IP, int port)
{
  // Se define la estructura para almacenar info del socket del servidor al momento de su creación
  struct sockaddr_in server_addr;

  // Se solicita un socket al SO, que se usará para escuchar conexiones entrantes
  server_socket = socket(AF_INET, SOCK_STREAM, 0);

  // Se configura el socket a gusto (recomiendo fuertemente el REUSEPORT!)
  int opt = 1;
  // fd_set readfds; 
  int ret = setsockopt(server_socket, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));

  // Se guardan el puerto e IP en la estructura antes definida
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  inet_aton(IP, &server_addr.sin_addr);
  server_addr.sin_port = htons(port);

  // Se le asigna al socket del servidor un puerto y una IP donde escuchar
  int ret2 = bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));

  // Se coloca el socket en modo listening
  int ret3 = listen(server_socket, 1);

  // Se definen las estructuras para almacenar info sobre los sockets de los clientes
  struct sockaddr_in client1_addr;
  socklen_t addr_size = sizeof(client1_addr);

  // Se inicializa una estructura propia para guardar los n°s de sockets de los clientes.
  PlayersInfo *sockets_clients = malloc(sizeof(PlayersInfo));

  // Se aceptan a los primeros 2 clientes que lleguen. "accept" retorna el n° de otro socket asignado para la comunicación
  sockets_clients->socket_c1 = accept(server_socket, (struct sockaddr *)&client1_addr, &addr_size);
  sockets_clients->socket_c2 = -1;
  sockets_clients->socket_c3 = -1; 
  sockets_clients->socket_c4 = -1; 
  n_connected += 1;

  // sockets_clients->socket_c2 = accept(server_socket, (struct sockaddr *)&client2_addr, &addr_size);
  // printf("parti igual nomas \n");
  return sockets_clients;
}

PlayersInfo *add_new_clients(PlayersInfo *sockets_clients){
  struct sockaddr_in new_client_addr;
  socklen_t addr_size = sizeof(new_client_addr);
  if (sockets_clients->socket_c2 == -1){
      sockets_clients->socket_c2 = accept(server_socket, (struct sockaddr *)&new_client_addr, &addr_size);
      n_connected += 1;
  } else if (sockets_clients->socket_c3 == -1){
      sockets_clients->socket_c3 = accept(server_socket, (struct sockaddr *)&new_client_addr, &addr_size);
      n_connected += 1;
  } else if (sockets_clients->socket_c4 == -1){
      sockets_clients->socket_c4 = accept(server_socket, (struct sockaddr *)&new_client_addr, &addr_size);
      n_connected += 1;
  } else {
      printf("SE ALCANZÓ EL MÁXIMO DE 4 CONECTADOS\n");
  }
  return sockets_clients;
}

