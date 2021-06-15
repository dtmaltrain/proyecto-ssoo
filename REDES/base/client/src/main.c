#include <unistd.h>
#include <stdio.h>
#include "conection.h"
#include "comunication.h"

int server_socket;
int ready = -1;

char *get_input()
{
  char *response = malloc(20);
  int pos = 0;
  while (1)
  {
    char c = getchar();
    if (c == '\n')
      break;
    response[pos] = c;
    pos++;
  }
  response[pos] = '\0';
  return response;
}

void *new_in_lobby(void *dummy)
{

  while (ready == -1)
  {
    int msg_code = client_receive_id(server_socket);
    if (msg_code == 5)
    {
      char *message = client_receive_payload(server_socket);
      printf("El servidor dice: %s\n", message);
    }
  }
  return NULL;
}

int main(int argc, char *argv[])
{
  //Se obtiene la ip y el puerto donde está escuchando el servidor (la ip y puerto de este cliente da igual)
  char *IP = "0.0.0.0";
  int PORT = 8080;

  // Se prepara el socket
  server_socket = prepare_socket(IP, PORT);

  // Se inicializa un loop para recibir todo tipo de paquetes y tomar una acción al respecto
  while (1)
  {
    int msg_code = client_receive_id(server_socket);

    if (msg_code == 1)
    { //Recibimos un mensaje del servidor
      char *message = client_receive_payload(server_socket);
      printf("El servidor dice: %s\n", message);
      free(message);

      printf("¿Qué desea hacer?\n   1)Enviar mensaje al servidor\n   2)Enviar mensaje al otro cliente\n");
      int option = getchar() - '0';
      getchar(); //Para capturar el "enter" que queda en el buffer de entrada stdin

      printf("Ingrese su mensaje: ");
      char *response = get_input();

      client_send_message(server_socket, option, response);
    }

    if (msg_code == 2)
    { //Recibimos un mensaje que proviene del otro cliente
      char *message = client_receive_payload(server_socket);
      printf("El otro cliente dice: %s\n", message);
      free(message);

      printf("¿Qué desea hacer?\n   1)Enviar mensaje al servidor\n   2)Enviar mensaje al otro cliente\n");
      int option = getchar() - '0';
      getchar(); //Para capturar el "enter" que queda en el buffer de entrada stdin

      printf("Ingrese su mensaje: ");
      char *response = get_input();

      client_send_message(server_socket, option, response);
    }

    if (msg_code == 3)
    {
      char *message = client_receive_payload(server_socket);
      printf("El servidor dice: %s\n", message);
      free(message);

      pthread_t cthread_id;
      pthread_create(&cthread_id, NULL, new_in_lobby, NULL);

      printf("Por favor elija la clase de su personaje\n   1)Cazador\n   2)Medico\n   3)Hacker\n");
      char option = getchar();
      getchar(); //Para capturar el "enter" que queda en el buffer de entrada stdin

      printf("Ingrese su nombre: ");
      char *response = get_input();
      char p_class[1];
      p_class[0] = option;
      char str[strlen(response) + 1];
      strcpy(str, response);
      strcat(str, p_class);
      client_send_message(server_socket, 3, str);
    }
    if (msg_code == 4)
    {
      char *message = client_receive_payload(server_socket);
      printf("El servidor dice: %s\n", message);
      free(message);

      printf("Si desea comenzar la partida, por favor ingrese cualquier mensaje\n");
      getchar(); //Para capturar el "enter" que queda en el buffer de entrada stdin
      char *response = "Comencemos";

      client_send_message(server_socket, 4, response);
      ready = 1;
    }

    if (msg_code == 5)
    {
      char *message = client_receive_payload(server_socket);
      printf("El servidor dice: %s\n", message);
      free(message);
    }

    printf("ESTO ES MAIN CLIENT\n");
  }

  // Se cierra el socket
  close(server_socket);
  free(IP);

  return 0;
}
