// gracias x la ayudantia

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "classes.h"
#include "comunication.h"
#include "conection.h"
#include <time.h>
#include <math.h>

PlayersInfo *players_info;
Character *players[4];
int sockets[4] = {-1, -1, -1, -1};
int start = -1;
int trol = 0;
int slct_screen = -1;
int n_ready = 0;
int turn_count = 0;
int win = 0;

int getRandom(int lower, int upper)
{
  int num = (rand() % (upper - lower + 1)) + lower;
  return num;
}
// printRandoms(lower, upper);

int getPlayer(Character *atk)
{
  int x = -1;
  if (atk->distracted == 1)
  {
    for (int i = 0; i < n_connected; i++)
    {
      if (players[i]->distracted == 1)
      {
        x = i;
      }
    }
  }
  else
  {
    x = getRandom(0, n_connected - 1);
  }
  // printf("\n\n la equis x %d \n\n", x);
  return x;
}

void send_message_to_all(char *message)
{
  for (int i = 0; i < n_connected; i++)
  {
    server_send_message(sockets[i], 5, message);
  }
}

char *get_users()
{
  char *out = malloc(255);
  strcpy(out, "");
  for (int i = 0; i < n_connected; i++)
  {
    if (players[i]->hp > 0)
    {
      char mid[63];
      sprintf(mid, "%i) %s, HP: %i\n", i + 1, players[i]->name, players[i]->hp);
      strcat(out, mid);
    }
  }
  return out;
}

void gamestate(Character *monster)
{
  char *plyrs = get_users();
  send_message_to_all(plyrs);
  free(plyrs);
  char m_state[255];
  printf("%s, HP: %i\n", monster->name, monster->hp);
  sprintf(m_state, "%s, HP: %i\n", monster->name, monster->hp);
  send_message_to_all(m_state);
}

////////////////////////////////////// HABILIDADES ///////////////////////////////////////////////////////////////////////////////

void estocada(Character *atk, Character *def)
{
  char action[255];
  hit(atk, def, 1000);
  // hit(atk, def, 1000000);
  if (def->bleeds < 3)
  {
    def->bleeds += 1;
  }
  sprintf(action, "%s usa Estocada sobre %s\n", atk->name, def->name);
  send_message_to_all(action);
}

void corte_cr(Character *atk, Character *def)
{
  char action[255];
  hit(atk, def, 3000);
  sprintf(action, "%s usa Corte Cruzado \n %s tiene %i vida\n", atk->name, def->name, def->hp);
  send_message_to_all(action);
}

void distraer(Character *atk, Character *def)
{
  char action[255];
  sprintf(action, "%s usa Distraer", atk->name);
  def->distracted = 1;
  for (int i = 0; i < n_connected; i++)
  {
    players[i]->distracted = 0;
  }
  atk->distracted = 1;
  send_message_to_all(action);
}

void curar(Character *atk, int idx)
{
  Character *def;
  if (atk->monster_or_player == 0) // 0 es que es player
  {

    char *choice = get_users();
    printf("%s\n", choice);
    server_send_message(sockets[idx], 10, choice);
    int option = -1;
    while (option != 6)
    {
      option = server_receive_id(sockets[idx]);
    }
    char *player_option = server_receive_payload(sockets[idx]);
    int n = player_option[0] - '0';
    printf("n:%i\n", n);
    def = players[n - 1];
    free(choice);
  }
  else
  {
    def = atk;
  }
  if (atk->reprobate > 0)
  {
    heal(def, 1000);
  }
  else
  {
    heal(def, 2000);
  }

  char action[255];
  sprintf(action, "%s\n usa Curar sobre %s\n", atk->name, def->name);
  send_message_to_all(action);
}

void destello(Character *atk, Character *def)
{

  Character *healed;
  int dmg = getRandom(750, 2000);
  hit(atk, def, dmg);
  // printf("daño: %i\n", dmg);
  if (atk == 0)
  {
    int ply = getRandom(0, n_connected - 1);
    healed = players[ply];
  }
  else
  {
    healed = atk;
  }
  int extra = ceil((dmg + 0.0) / 2);
  if (atk->reprobate > 0)
  {
    dmg = extra;
    extra = ceil((extra + 0.0) / 2);
  }

  heal(healed, extra);
  char action[255];
  sprintf(action, "%s\n usa Destello Regenerador, hace %i de daño, y cura %i a %s\n", atk->name, dmg, extra, healed->name);
  send_message_to_all(action);
}
void descarga(Character *atk, Character *def)
{

  char action[255];
  int dmg = 2 * (atk->hp_max - atk->hp);
  hit(atk, def, dmg);
  sprintf(action, "%s\n usa Descarga Vital, y hace %i de daño", atk->name, dmg);
  send_message_to_all(action);
}
void inyeccion(Character *atk, int idx)
{
  Character *def;
  if (atk->monster_or_player == 0)
  {
    char *choice = get_users();
    printf("%s\n", choice);
    server_send_message(sockets[idx], 10, choice);
    int option = -1;
    while (option != 6)
    {
      option = server_receive_id(sockets[idx]);
    }
    char *player_option = server_receive_payload(sockets[idx]);
    int n = player_option[0] - '0';
    printf("n:%i\n", n);
    def = players[n - 1];
    free(choice);
  }
  else
  {
    def = atk;
  }
  def->bonus = 2;
  char action[255];
  sprintf(action, "%s\n usa Inyección SQL sobre %s:\n", atk->name, def->name);
  send_message_to_all(action);
}
void ataque_ddos(Character *atk, Character *def)
{
  char action[255];
  hit(atk, def, 1500);
  sprintf(action, "%s\n usa Ataque DDOS sobre %s\n", atk->name, def->name);
  send_message_to_all(action);
}
void fuerza_bruta(Character *atk, Character *def)
{
  char action[255];
  sprintf(action, "%s\n usa Fuerza Bruta\n", atk->name);
  send_message_to_all(action);
  atk->brute += 1;
  if (atk->brute == 3)
  {
    hit(atk, def, 10000);
    atk->brute = 0;
    char action[255];
    sprintf(action, "%s\n usa Fuerza Bruta por 3ra vez y hace 10000 de dmg", atk->name);
    send_message_to_all(action);
  }
}

// ------------------------- HABILIDADES GREAT JAGRUZ

void ruzgar(Character *atk)
{
  int x = getPlayer(atk);
  Character *def = players[x];
  hit(atk, def, 1000);
  char action[255];
  sprintf(action, "Great JagRuz usa Ruzgar y hace 1000 de dmg a %s\n", def->name);
  send_message_to_all(action);
}

void coletazo(Character *atk)
{
  for (int i = 0; i < n_connected; i++)
  {
    hit(atk, players[i], 500);
  }
  char *action = "Great JagRuz usa Coletazo y hace 500 de dmg a todos los jugadores\n";
  send_message_to_all(action);
}

// ------------------------- HABILIDADES RUZALOS

void salto(Character *atk)
{
  int x = getPlayer(atk);
  Character *def = players[x];
  hit(atk, def, 1500);
  atk->jump = 2;
  char action[255];
  sprintf(action, "Ruzalos usa Salto y hace 1500 de dmg a %s\n", def->name);
  send_message_to_all(action);
}

void espina_venenosa(Character *atk)
{
  int x = getPlayer(atk);
  Character *def = players[x];
  char action[255];
  if (def->toxin > 0)
  {
    hit(atk, def, 500);
    sprintf(action, "Ruzalos usa Espina Venenosa y hace 500 de dmg a %s\n", def->name);
  }
  else
  {
    def->toxin = 3;
    sprintf(action, "Ruzalos usa Espina Venenosa y envenena a %s\n", def->name);
  }

  send_message_to_all(action);
}

// ------------------------- HABILIDADES RUIZ

void caso_de_copia(Character *atk)
{
  int hab = getRandom(1, 9);
  int x = getPlayer(atk);
  Character *def = players[x];

  if (hab == 1)
  {
    char action[255];
    sprintf(action, "Ruiz usa Caso de Copia y copia Estocada\n");
    send_message_to_all(action);
    estocada(atk, def);
  }
  else if (hab == 2)
  {
    char action[255];
    sprintf(action, "Ruiz usa Caso de Copia y copia Corte Cruzado\n");
    send_message_to_all(action);
    corte_cr(atk, def);
  }
  else if (hab == 3)
  {
    char action[255];
    sprintf(action, "Ruiz usa Caso de Copia y copia Distraer\n");
    send_message_to_all(action);
    distraer(atk, def);
  }
  else if (hab == 4)
  {
    char action[255];
    sprintf(action, "Ruiz usa Caso de Copia y copia Curar\n");
    send_message_to_all(action);
    curar(atk, -1);
  }
  else if (hab == 5)
  {
    char action[255];
    sprintf(action, "Ruiz usa Caso de Copia y copia Destello Regenerador\n");
    send_message_to_all(action);
    destello(atk, def);
  }
  else if (hab == 6)
  {
    char action[255];
    sprintf(action, "Ruiz usa Caso de Copia y copia Descarga Vital\n");
    send_message_to_all(action);
    descarga(atk, def);
  }
  else if (hab == 7)
  {
    char action[255];
    sprintf(action, "Ruiz usa Caso de Copia y copia Inyeccion SQL\n");
    send_message_to_all(action);
    inyeccion(atk, -1);
  }
  else if (hab == 8)
  {
    char action[255];
    sprintf(action, "Ruiz usa Caso de Copia y copia Ataque DDOS\n");
    send_message_to_all(action);
    ataque_ddos(atk, def);
  }
  else
  {
    char action[255];
    sprintf(action, "Ruiz usa Caso de Copia y copia Fuerza Bruta\n");
    send_message_to_all(action);
    fuerza_bruta(atk, def);
  }
}

void reprobatron(Character *atk)
{
  int x = getPlayer(atk);
  Character *def = players[x];

  char action[255];
  sprintf(action, "Ruiz usa Reprobatron-9000 y reprueba a %s\n", def->name);
  def->reprobate = 2;
  send_message_to_all(action);
}

void sudo(Character *atk)
{

  int dmg = 100 * turn_count;
  for (int i = 0; i < n_connected; i++)
  {
    hit(atk, players[i], dmg);
  }
  char action[255];
  sprintf(action, "Ruiz usa sudo rm -rf y hace %i de dmg a todos los jugadores\n", dmg);
  send_message_to_all(action);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////7

void *pj_wait_choice(void *n_socket)
{
  int index = slct_screen;
  char *welcome = "Bienvenido!!";
  server_send_message(sockets[index], 3, welcome);
  int msg_code = server_receive_id(sockets[index]);
  char *client_message = server_receive_payload(sockets[index]);
  //printf("El player dice: %s\n", client_message);
  int lenc = strlen(client_message);
  int n_class = client_message[lenc - 1] - '0';
  client_message[lenc - 1] = '\0';
  player_ready(players[index], client_message, n_class);
  //printf("PLAYER: Nombre: %s, Clase: %i\n", players[index]->name, players[index]->id_class);
  char *response = "Nuevo jugador en el lobby:\n  Nombre: %s, Clase: %i\n  Estado Lobby: %i/4";
  n_ready += 1;
  char msje[strlen(response)];
  sprintf(msje, "Nuevo jugador en el lobby:\n  Nombre: %s, Clase: %i\n  Estado Lobby: %i/4", players[index]->name, players[index]->id_class, n_connected);
  server_send_message(players_info->socket_c1, 5, msje);
  return NULL;
}

void *choose_pj(void *n_socket)
{
  while (1)
  {
    char *welcome;
    for (int i = 1; i < n_connected; i++)
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
  return NULL;
}

void *wait_players_thread(void *caca)
{
  while (n_connected < 4)
  {
    PlayersInfo *new_players_info;
    players_info = add_new_clients(players_info);
    sockets[0] = players_info->socket_c1;
    sockets[1] = players_info->socket_c2;
    sockets[2] = players_info->socket_c3;
    sockets[3] = players_info->socket_c4;
    // printf("Se Conectó un weon \n");
  }
  //printf("CHAO Thread 1\n");
  return NULL;
  //pthread_exit(players_info);
}

void *wait_start(void *dummy)
{
  int msg_code = -1;
  char *welcome = "Esperando en el lobby...";

  while (1)
  {
    server_send_message(players_info->socket_c1, 4, welcome);
    while (msg_code != 4)
    {
      msg_code = server_receive_id(players_info->socket_c1);
      char *client_message = server_receive_payload(players_info->socket_c1);
    }
    if (n_connected == n_ready)
    {
      break;
    }
    char *warning = "Un jugador todavía no elige su nombre y clase, por favor paciencia...\n";
    server_send_message(players_info->socket_c1, 5, warning);
    msg_code = -1;
  }
  char *response = "Comencemos el juego";
  server_send_message(players_info->socket_c1, 5, response);
  start = 1;
  return NULL;
}

int choose_monster(Character *monster)
{
  int monster_msg_code = -1;
  char *select_monster = "Por favor elija un monstruo";
  server_send_message(players_info->socket_c1, 6, select_monster);

  while (monster_msg_code != 5)
  {
    monster_msg_code = server_receive_id(players_info->socket_c1);
  }
  char *client_monster_message = server_receive_payload(players_info->socket_c1);
  int monster_id = client_monster_message[0] - '0';
  if (monster_id == 4)
  {
    monster_id = getRandom(0, 3);
  }
  //printf("ID MONSTRUO: %i", monster_id);
  monster_ready(monster, monster_id);
  //printf("El lider dice: %s\n", client_monster_message);

  char eleccion_monstruo[255];
  sprintf(eleccion_monstruo, "Elegiste el monstruo %s\n Empieza el juego!\n", monster->name);

  for (int i = 0; i < n_connected; i++)
  {
    if (sockets[i] != -1)
    {
      server_send_message(sockets[i], 5, eleccion_monstruo);
    }
  }
  printf("Monstruo %s, vida %i\n", monster->name, monster->hp);
  return monster_id;
}

int main(int argc, char *argv[])
{
  srand(time(0));
  for (int i = 0; i < 4; i++)
  {
    players[i] = char_init();
  }
  Character *monster = char_init();

  //printf("Hello P2!\n");
  // Se define una IP y un puerto
  char *IP;
  int PORT;
  char *flag_1 = argv[1];
  char *flag_2 = argv[3];
  if (strcmp(flag_1, "-i") == 0)
  {
    IP = argv[2];
    PORT = atoi(argv[4]);
  }
  else
  {
    IP = argv[4];
    PORT = atoi(argv[2]);
  }

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
  //printf("El lider dice: %s\n", client_message);
  int lenc = strlen(client_message);
  int n_class = client_message[lenc - 1] - '0';
  client_message[lenc - 1] = '\0';
  player_ready(players[0], client_message, n_class);
  printf("LIDER: Nombre: %s, Clase: %i\n", players[0]->name, players[0]->id_class);
  n_ready += 1;
  pthread_t thread2_id;
  // Le enviamos al primer cliente un mensaje de bienvenida
  pthread_create(&thread2_id, NULL, wait_start, NULL);
  pthread_join(thread2_id, NULL);
  pthread_cancel(thread_id);

  //  ---------------------------------------------------------------- ACA PARTE EL JUEGO
  if (n_connected == 1)
  {
    sockets[0] = players_info->socket_c1;
  }
  //int segunda_vez = 0;
  while (1)
  {
    // if (segunda_vez == 1)
    // {
    //   int desconectados[4] = {-1, -1, -1, -1};
    //   for (int i = 0; i < n_connected; i++)
    //   {
    //     char keep[255];
    //     sprintf(keep, "%s ¿Deseas seguir jugando?", players[i]->name);
    //     server_send_message(sockets[i], 11, keep);
    //     int option_playing = -1;
    //     while (option_playing != 6)
    //     {
    //       option_playing = server_receive_id(sockets[i]);
    //     }
    //     char *keep_playing = server_receive_payload(sockets[i]);
    //     int keeping = keep_playing[0] - '0';
    //     if (keeping == 1)
    //     {
    //       printf("SIGUE JUGANDO\n");
    //     }
    //   }
    // }
    // else
    // {
    //   segunda_vez = 1;
    // }
    int monster_id = choose_monster(monster);
    while (turn_count < 99999)
    {
      int someone_alive = n_connected;
      for (int i = 0; i < n_connected; i++)
      {
        if (players[i]->alive == 1)
        {
          gamestate(monster);
          int option = -1;
          char turnn[255];
          sprintf(turnn, "\nTurno de %s\n", players[i]->name);
          for (int u = 0; u < n_connected; u++)
          {
            server_send_message(sockets[u], 5, turnn);
          }
          char turn[255];
          sprintf(turn, "Es tu turno");
          if (players[i]->id_class == 1)
          {
            server_send_message(sockets[i], 7, turn);
          }
          else if (players[i]->id_class == 2)
          {
            server_send_message(sockets[i], 8, turn);
          }
          else
          {
            server_send_message(sockets[i], 9, turn);
          }

          while (option != 6)
          {
            option = server_receive_id(sockets[i]);
          }
          char *player_option = server_receive_payload(sockets[i]);
          int n = player_option[0] - '0';
          // p_use_ability(players[i], player_option_int);
          char action[255];
          if (players[i]->id_class == 1)
          {
            // CAZADOR
            if (n == 1)
            {
              estocada(players[i], monster);
            }
            else if (n == 2)
            {
              corte_cr(players[i], monster);
            }
            else if (n == 3)
            {
              distraer(players[i], monster);
            }
            else
            {
              players[i]->hp = 0;
              players[i]->alive = 0;
              someone_alive -= 1;
              char dead[255];
              sprintf(dead, "%s se ha rendido\n", players[i]->name);
              send_message_to_all(dead);
              continue;
            }
          }
          else if (players[i]->id_class == 2)
          {
            // MEDICO
            if (n == 1)
            {
              curar(players[i], i);
            }
            else if (n == 2)
            {
              destello(players[i], monster);
            }
            else if (n == 3)
            {
              descarga(players[i], monster);
            }
            else
            {
              players[i]->hp = 0;
              players[i]->alive = 0;
              someone_alive -= 1;
              char dead[255];
              sprintf(dead, "%s se ha rendido\n", players[i]->name);
              send_message_to_all(dead);
              continue;
            }
          }
          else
          {
            //HACKER
            if (n == 1)
            {
              inyeccion(players[i], i);
            }
            else if (n == 2)
            {
              ataque_ddos(players[i], monster);
            }
            else if (n == 3)
            {
              fuerza_bruta(players[i], monster);
            }
            else
            {
              players[i]->hp = 0;
              players[i]->alive = 0;
              someone_alive -= 1;
              char dead[255];
              sprintf(dead, "%s se ha rendido\n", players[i]->name);
              send_message_to_all(dead);
              continue;
            }
          }
          // FIN TURNO PLAYER
          if (players[i]->bonus > 0)
          {
            players[i]->bonus--;
          }

          if (players[i]->toxin > 0)
          {
            players[i]->hp -= 400;
            char toxic[255];
            sprintf(toxic, "%s pierde 400 de vida debido a la toxina\n", players[i]->name);
            send_message_to_all(toxic);
            if (players[i]->hp < 0)
            {
              players[i]->hp = 0;
            }
            players[i]->toxin--;
          }

          if (players[i]->bleeds > 0)
          {
            players[i]->hp -= 500;
            char bleed[255];
            sprintf(bleed, "%s pierde 500 de vida debido al sangrado\n", monster->name);
            send_message_to_all(bleed);
            if (players[i]->hp < 0)
            {
              players[i]->hp = 0;
            }
            players[i]->bleeds--;
          }
          if (monster->hp == 0)
          {
            win = 1;
            break;
          }
          if (players[i]->hp == 0 && players[i]->alive == 1)
          {
            players[i]->alive = 0;
            char dead[255];
            sprintf(dead, "%s ha muerto o se ha rendido\n", players[i]->name);
            send_message_to_all(dead);
          }
          // FIN DE TODOS LOS TURNOS DE PLAYERS
        }
        else if (players[i]->alive == 0)
        {
          someone_alive -= 1;
        }
      }
      if (someone_alive == 0)
      {
        char defeat[255];
        sprintf(defeat, "%s ha derrotado a tu equipo\n", monster->name);
        send_message_to_all(defeat);
        break;
      }

      if (win == 1)
      {
        win = 0;
        char victory[255];
        sprintf(victory, "VICTORIA, derrotaste a %s\n", monster->name);
        send_message_to_all(victory);
        break;
      }
      gamestate(monster);
      int number = getRandom(1, 10);
      if (monster_id == 1)
      {
        if (number <= 5)
        {
          ruzgar(monster);
        }
        else
        {
          coletazo(monster);
        }
      }
      else if (monster_id == 2)
      {
        printf("jump: %i\n", monster->jump);
        if (monster->jump == 1)
        {
          espina_venenosa(monster);
        }
        else
        {
          if (number <= 4)
          {
            salto(monster);
          }
          else
          {
            espina_venenosa(monster);
          }
        }
      }
      else
      {
        if (number <= 4)
        {
          caso_de_copia(monster);
        }
        else if (number > 4 && number <= 6)
        {
          reprobatron(monster);
        }
        else
        {
          sudo(monster);
          turn_count = 0;
        }
      }
      for (int i = 0; i < n_connected; i++)
      {
        if (players[i]->alive == 1 && players[i]->hp == 0)
        {
          players[i]->alive = 0;
          char dead[255];
          sprintf(dead, "%s ha muerto\n", players[i]->name);
          send_message_to_all(dead);
        }
      }
      // FIN TURNO MONSTER
      turn_count++;
      if (monster->jump > 0)
      {
        monster->jump--;
      }
      if (monster->distracted > 0)
      {
        monster->distracted = 0;
      }
      if (monster->bleeds > 0)
      {
        monster->hp -= 500;
        char bleed[255];
        sprintf(bleed, "%s pierde 500 de vida debido al sangrado\n", monster->name);
        send_message_to_all(bleed);
        if (monster->hp < 0)
        {
          monster->hp = 0;
        }
        monster->bleeds--;
      }
      for (int i = 0; i < n_connected; i++)
      {
        if (players[i]->reprobate > 0)
        {
          players[i]->reprobate--;
        }
      }
      if (monster->hp == 0 && monster->alive == 1)
      {
        monster->alive = 0;
        win = 1;
      }
      if (win == 1)
      {
        win = 0;
        char victory[255];
        sprintf(victory, "VICTORIA, derrotaste a %s\n", monster->name);
        send_message_to_all(victory);
        break;
      }
    }
  }
  return 0;
}
