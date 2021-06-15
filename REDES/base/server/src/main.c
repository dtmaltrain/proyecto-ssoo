// gracias x la ayudantia

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "player.h"
#include "comunication.h"
#include "conection.h"

PlayersInfo *players_info;
Player *players[4];
int sockets[4] = {-1, -1, -1, -1};
int start = -1;
int trol = 0;
int slct_screen = -1;
int n_ready = 0;

char *revert(char *message)
{
  //Se invierte el mensaje

  int len = strlen(message) + 1;
  char *response = malloc(len);

  for (int i = 0; i < len - 1; i++)
  {
    response[i] = message[len - 2 - i];
  }
  response[len - 1] = '\0';
  return response;
}
void *pj_wait_choice(void *n_socket)
{
  int index = slct_screen;
  char *welcome = "Bienvenido!!";
  server_send_message(sockets[index], 3, welcome);
  int msg_code = server_receive_id(sockets[index]);
  char *client_message = server_receive_payload(sockets[index]);
  printf("El player dice: %s\n", client_message);
  int lenc = strlen(client_message);
  int n_class = client_message[lenc - 1] - '0';
  client_message[lenc - 1] = '\0';
  player_ready(players[index], client_message, n_class);
  printf("PLAYER: Nombre: %s, Clase: %i\n", players[index]->name, players[index]->p_class);
  char *response = "Nuevo jugador en el lobby:\n  Nombre: %s, Clase: %i\n  Estado Lobby: %i/4";
  n_ready += 1;
  char msje[strlen(response)];
  sprintf(msje, "Nuevo jugador en el lobby:\n  Nombre: %s, Clase: %i\n  Estado Lobby: %i/4", players[index]->name, players[index]->p_class, n_connected);
  server_send_message(players_info->socket_c1, 5, msje);
  return NULL;
}
void *choose_pj(void *n_socket)
{
  printf("HOLA Thread de jugadores eligiendo clase\n");
  while (1)
  {
    char *welcome;
    for (int i = 1; i < 4; i++)
    {
      if (sockets[i] != -1)
      {
        char *invalido = "NULL";
        if (strcmp(players[i]->name, invalido) == 0)
        {
          pthread_t pthread_id;
          players[i]->name = "IN CHOICE";
          slct_screen = i;
          pthread_create(&pthread_id, NULL, pj_wait_choice, NULL);
          sleep(1);
          //players[i]->name = client_message;
        }
      }
    }
  }

  printf("CHAO Thread de jugadores eligiendo clase\n");
  return NULL;
}

void *wait_players_thread(void *caca)
{
  printf("HOLA Thread que espera nuevos clients\n");
  while (n_connected < 4)
  {
    PlayersInfo *new_players_info;
    players_info = add_new_clients(players_info);
    sockets[0] = players_info->socket_c1;
    sockets[1] = players_info->socket_c2;
    sockets[2] = players_info->socket_c3;
    sockets[3] = players_info->socket_c4;
    printf("Se Conectó DON WEON \n");
  }
  printf("CHAO Thread 1\n");
  return NULL;
  //pthread_exit(players_info);
}

void *wait_start(void *dummy)
{
  printf("HOLA Thread que espera a que el Líder confirme el inicio\n");
  int msg_code = -1;
  char *welcome = "Esperando en el lobby...";
  server_send_message(players_info->socket_c1, 4, welcome);
  while (msg_code != 4)
  {
    msg_code = server_receive_id(players_info->socket_c1);
    char *client_message = server_receive_payload(players_info->socket_c1);
    printf("El lider dice: %s\n", client_message);
    char *response = "COMENCEMOS EL JUEGO";
    server_send_message(players_info->socket_c1, 1, response);
  }
  start = 1;
  printf("%i\n", start);
  printf("CHAO Thread que espera a que el Líder confirme el inicio\n");
  return NULL;
}

int main(int argc, char *argv[])
{
  for (int i = 0; i < 4; i++)
  {
    players[i] = Player_init();
  }

  printf("Hello P2!\n");
  // Se define una IP y un puerto
  char *IP = "0.0.0.0";
  int PORT = 8080;

  // Se crea el servidor y se obtienen los sockets de ambos clientes.
  players_info = prepare_sockets_and_get_clients(IP, PORT);

  pthread_t thread_id;
  pthread_create(&thread_id, NULL, wait_players_thread, NULL);
  pthread_t thread_choice_leader_id;
  pthread_create(&thread_choice_leader_id, NULL, choose_pj, NULL);
  long pos = 1;
  char *welcome = "Bienvenido Líder del grupo!!";
  server_send_message(players_info->socket_c1, 3, welcome);
  int msg_code = server_receive_id(players_info->socket_c1);
  char *client_message = server_receive_payload(players_info->socket_c1);
  printf("El lider dice: %s\n", client_message);
  int lenc = strlen(client_message);
  int n_class = client_message[lenc - 1] - '0';
  client_message[lenc - 1] = '\0';
  player_ready(players[0], client_message, n_class);
  printf("LIDER: Nombre: %s, Clase: %i\n", players[0]->name, players[0]->p_class);
  n_ready += 1;
  pthread_t thread2_id;
  // Le enviamos al primer cliente un mensaje de bienvenida
  pthread_create(&thread2_id, NULL, wait_start, NULL);
  pthread_join(thread2_id, NULL);
  pthread_cancel(thread_id);

  // Guardaremos los sockets en un arreglo e iremos alternando a quién escuchar.
  int sockets_array[2] = {players_info->socket_c1, players_info->socket_c2};
  int my_attention = 0;
  while (1)
  {
    // printf("AAAA\n");
    // // Se obtiene el paquete del cliente 1
    // int msg_code = server_receive_id(sockets_array[my_attention]);

    // if (msg_code == 1) //El cliente me envió un mensaje a mi (servidor)
    // {
    //   char *client_message = server_receive_payload(sockets_array[my_attention]);
    //   printf("El cliente %d dice: %s\n", my_attention + 1, client_message);

    //   // Le enviaremos el mismo mensaje invertido jeje
    //   char *response = revert(client_message);

    //   // Le enviamos la respuesta
    //   server_send_message(sockets_array[my_attention], 1, response);
    // }
    // else if (msg_code == 2)
    // { //El cliente le envía un mensaje al otro cliente
    //   char *client_message = server_receive_payload(sockets_array[my_attention]);
    //   printf("Servidor traspasando desde %d a %d el mensaje: %s\n", my_attention + 1, ((my_attention + 1) % 2) + 1, client_message);

    //   // Mi atención cambia al otro socket
    //   my_attention = (my_attention + 1) % 2;

    //   server_send_message(sockets_array[my_attention], 2, client_message);
    // }
    printf("MAIN SERVIDOR\n");
    sleep(4);
  }

  return 0;
}
