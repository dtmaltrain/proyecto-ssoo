#include <unistd.h>
#include <stdio.h>
#include "conection.h"
#include "comunication.h"

int server_socket;
int ready = -1;

char *get_input()
{
  char *response = malloc(255);
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

      printf("¿Qué desea hacer?\n1)Enviar mensaje al servidor\n2)Enviar mensaje al otro cliente\n");
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

      printf("¿Qué desea hacer?\n1)Enviar mensaje al servidor\n2)Enviar mensaje al otro cliente\n");
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

      printf("Por favor elija la clase de su personaje\n1)Cazador\n2)Medico\n3)Hacker\n");
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
      char* cualquiera = get_input();
      char* response = "Comencemos";
      free(cualquiera);

      client_send_message(server_socket, 4, response);
      ready = 1;
    }

    if (msg_code == 5)
    {
      char *message = client_receive_payload(server_socket);
      printf("El servidor dice: %s\n", message);
      free(message);
    }

    if (msg_code == 6)
    {
      char *message = client_receive_payload(server_socket);
      printf("El servidor dice: %s\n", message);
      free(message);

      printf("¿Qué monstruo desea elegir?\n1)Great JagRuz\n2)Ruzalos\n3)Ruiz\n4)Monstruo aleatorio\n");
      // char option = getchar() - '0';
      // getchar(); //Para capturar el "enter" que queda en el buffer de entrada stdin
      // char p_monster[1];
      // p_monster[0] = option;
      char* p_monster = get_input();
      client_send_message(server_socket, 5, p_monster);
    }

    if (msg_code == 7)
    {
      char *message = client_receive_payload(server_socket);
      printf("El servidor dice: %s\n", message);
      free(message);
      printf("¿Qué deseas hacer?\n1)Usar Estocada\n2)Usar Corte Cruzado\n3)Usar Distraer\n4)Rendirse\n");
      char* decition = get_input();
      client_send_message(server_socket, 6, decition);
    }

    if (msg_code == 8)
    {
      char *message = client_receive_payload(server_socket);
      printf("El servidor dice: %s\n", message);
      free(message);
      printf("¿Qué deseas hacer?\n1)Usar Curar\n2)Usar Destello Regenerador\n3)Usar Descarga Vital\n4)Rendirse\n");
      char* decition = get_input();
      client_send_message(server_socket, 6, decition);
    }

    if (msg_code == 9)
    {
      char *message = client_receive_payload(server_socket);
      printf("El servidor dice: %s\n", message);
      free(message);
      printf("¿Qué deseas hacer?\n1)Usar Inyección SQL\n2)Usar Ataque DDOS\n3)Usar Fuerza Bruta\n4)Rendirse\n");
      char* decition = get_input();
      client_send_message(server_socket, 6, decition);
    }

    if (msg_code == 10)
    {
      char *message = client_receive_payload(server_socket);   
      printf("¿Elige al jugador?\n%s", message);
      free(message);
      char* decition = get_input();
      client_send_message(server_socket, 6, decition);
    }

  //   printf("ESTO ES MAIN CLIENT\n");
  }

  // Se cierra el socket
  close(server_socket);
  free(IP);

  return 0;
}
