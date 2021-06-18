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


int getRandom(int lower, int upper)
{
    int num = (rand() %(upper - lower + 1)) + lower;
    return num;
}
// printRandoms(lower, upper);

int getPlayer(){
  int x = -1;
  if (monster -> distracted == 1)
  {
    for (int i = 0; i < n_connected; i++)
    {
      if(players[i] -> distracted == 1){
        x = i;
      }
    }
  }
  else
  {
    int x = getRandom(0, n_connected);
  }
  return x;
}

void send_message_to_all(char* message){
  for (int i = 0; i < n_connected; i++)
    {
      if (sockets[i] != -1)
        {
          server_send_message(sockets[i], 5, message);
        }
    }
}

char* get_users(){
  char* out = malloc(255);
  strcpy(out, "");
  for (int i = 0; i < n_connected; i++) {
      char mid[63];
      sprintf(mid, "%i) %s, hp: %i\n", i + 1, players[i] -> name, players[i] -> hp);
      strcat(out, mid);
  }
  return out;
}

////////////////////////////////////// HABILIDADES ///////////////////////////////////////////////////////////////////////////////

void estocada(Character *atk, Character *def){
    char action[255];
    hit(def, 1000);
    if (def -> bleeds < 3)
    {
    def -> bleeds += 1;
    }
    sprintf(action, "%s usa Estocada sobre %s\n", atk->name, def->name);
    send_message_to_all(action);
}

void corte_cr(Character *atk, Character *def){
    char action[255];
    hit(monster, 3000);
    sprintf(action, "%s usa Corte Cruzado \n %s tiene %i vida\n", atk->name, def-> name, def -> hp);
    send_message_to_all(action);
}

void distraer(Character* atk, Character* def){
  char action[255];
  sprintf(action, "%s usa Distraer", atk -> name);
  send_message_to_all(action);
  // def -> distracted = 1;
  // players[i] -> 
}

void curar(Character* atk, int idx){
  Character* def;
  if (atk -> monster_or_player == 0) // 0 es que es player
  {
    
    char* choice = get_users();
    printf("%s\n", choice);
    server_send_message(sockets[idx], 10, choice);
    int option = -1;
    while(option != 6){
      option = server_receive_id(sockets[idx]);
    }
    char *player_option = server_receive_payload(sockets[idx]);
    int n = player_option[0] - '0';
    printf("n:%i\n", n);
    // printf("%s", choice); //MANDAR A socket atk
    def = players[n - 1];
    free(choice);
  }else{
    def = atk;
  }
  heal(def, 2000);
  
  char action[255];
  sprintf(action, "%s\n usa Curar sobre %s\n", atk->name, def-> name);
  send_message_to_all(action);
}

void destello(Character* atk, Character* def){

  Character* healed;
  int dmg = getRandom(750, 2001);
  hit(def, dmg);
  // printf("daño: %i\n", dmg);
  if (atk == 0)
  {
    int ply = getRandom(0, n_connected);
    healed = players[ply];
  }else{
    healed = atk;
  }
  int extra = ceil((dmg + 0.0)/2);
  heal(healed, extra);
  char action[255];
  sprintf(action, "%s\n usa Destello Regenerador, hace %i de daño, y cura %i a %s\n", atk -> name, dmg, extra, healed -> name);
  send_message_to_all(action);
}
void descarga(Character* atk, Character* def){

  char action[255];
  int dmg = 2*(3000 - atk -> hp);
  hit(def, dmg);
  sprintf(action, "%s\n usa Descarga Vital, y hace %i de daño", atk -> name, dmg);
  send_message_to_all(action);
}
void inyeccion(Character* atk){
  Character* def;
  if (atk -> monster_or_player == 0)
  {
    char* choice = get_users();
    printf("%s", choice); //MANDAR A socket atk
    //def = players[i] i lo elige el weon
    free(choice);
  }else{
    def = atk;
  }
  // elegir jugador
  def -> bonus = 2;
  char action[255];
  sprintf(action, "%s\n usa Inyección SQL sobre %s:\n", atk -> name, def -> name);
  send_message_to_all(action);
}
void ataque_ddos(Character* atk, Character* def){
  char action[255];
  hit(def, 1500);
  sprintf(action, "%s\n usa Ataque DDOS sobre %s\n", atk -> name, def -> name);
  send_message_to_all(action);
}
void fuerza_bruta(Character* atk, Character* def){
  char action[255];
  sprintf(action, "%s\n usa Fuerza Bruta\n", atk -> name);
  send_message_to_all(action);
  atk -> brute += 1;
  if (atk -> brute == 3)
  {
    hit(def, 10000);
    atk -> brute = 0;
    char action[255];
    sprintf(action, "%s\n usa Fuerza Bruta por 3ra vez y hace 10000 de dmg", atk -> name);
    send_message_to_all(action);
  }
}

// ------------------------- HABILIDADES GREAT JAGRUZ

void ruzgar(){
  int x = getPlayer();
  Character* def = players[x]; 
  hit(def, 1000);
  char action[255];
  sprintf(action, "Great JagRuz usa Ruzgar y hace 1000 de dmg a %s\n", def -> name);
  send_message_to_all(action);
}

void coletazo(){
  for (int i = 0; i < n_connected; i++){
    hit(players[i], 500);
  }
  char* action = "Great JagRuz usa Coletazo y hace 500 de dmg a todos los jugadores\n";
  send_message_to_all(action);
}

// ------------------------- HABILIDADES RUZALOS

void salto(){
  int x = getPlayer();
  Character* def = players[x]; 
  hit(def, 1500);
  monster -> jump = 1;
  char action[255];
  sprintf(action, "Ruzalos usa Salto y hace 1500 de dmg a %s\n", def -> name);
  send_message_to_all(action);
}

void espina_venenosa(){
  int x = getPlayer();
  Character* def = players[x];
  //FALTA ENVENENAR Y VER SI ESTÁ YA ENVENENADO
  char action[255];
  sprintf(action, "Ruzalos usa Espina Venenosa y envenena a %s\n", def -> name);  
  send_message_to_all(action);
}

// ------------------------- HABILIDADES RUIZ

void caso_de_copia(){
  int x = getPlayer();
  Character* def = players[x];
  char action[255];
  sprintf(action, "Ruiz usa Caso de Copia, aplica {} a %s\n", def -> name);  
  send_message_to_all(action);
}

void reprobatron(){
  int x = getPlayer();
  Character* def = players[x];
  // FALTA IMPLEMENTAR ESTADO REPROBADO 
  char action[255];
  sprintf(action, "Ruiz usa Reprobatron-9000 y reprueba a %s\n", def -> name);  
  send_message_to_all(action);
}

void sudo(){
  // FALTA IMPLEMENTAR CAMBIO EN CONTADOR DE TURNOS
  int dmg = 100 * turn_count;
  for (int i = 0; i < n_connected; i++){
    hit(players[i], dmg);
  }  
  char action[255];
  sprintf(action, "Ruiz usa sudo rm -rf y hace {%i} de dmg a todos los jugadores\n", dmg);
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
  printf("El player dice: %s\n", client_message);
  int lenc = strlen(client_message);
  int n_class = client_message[lenc - 1] - '0';
  client_message[lenc - 1] = '\0';
  player_ready(players[index], client_message, n_class);
  printf("PLAYER: Nombre: %s, Clase: %i\n", players[index]->name, players[index]->id_class);
  char *response = "Nuevo jugador en el lobby:\n  Nombre: %s, Clase: %i\n  Estado Lobby: %i/4";
  n_ready += 1;
  char msje[strlen(response)];
  sprintf(msje, "Nuevo jugador en el lobby:\n  Nombre: %s, Clase: %i\n  Estado Lobby: %i/4", players[index]->name, players[index]->id_class, n_connected);
  server_send_message(players_info->socket_c1, 5, msje);
  return NULL;
}
void *choose_pj(void *n_socket)
{
  printf("HOLA Thread de jugadores eligiendo clase\n");
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
    // printf("Se Conectó un weon \n");
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
  while (1){
    server_send_message(players_info->socket_c1, 4, welcome);
    while (msg_code != 4)
    {
      msg_code = server_receive_id(players_info->socket_c1);
      char *client_message = server_receive_payload(players_info->socket_c1);
      printf("El lider dice: %s\n", client_message);
    }
    if (n_connected == n_ready){
      break;
    }
    char *warning = "Un jugador todavía no elige su nombre y clase, por favor paciencia...\n";
    server_send_message(players_info->socket_c1, 5, warning);
    msg_code = -1;
  }

  char *response = "COMENCEMOS EL JUEGO";
  server_send_message(players_info->socket_c1, 5, response);
  start = 1;
  printf("%i\n", start);
  printf("CHAO Thread que espera a que el Líder confirme el inicio\n");
  return NULL;
}

int main(int argc, char *argv[])
{
  srand(time(0));
  for (int i = 0; i < 4; i++)
  {
    players[i] = char_init();
  }
  Character* monster= char_init();

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
  printf("LIDER: Nombre: %s, Clase: %i\n", players[0]->name, players[0]->id_class);
  n_ready += 1;
  pthread_t thread2_id;
  // Le enviamos al primer cliente un mensaje de bienvenida
  pthread_create(&thread2_id, NULL, wait_start, NULL);
  pthread_join(thread2_id, NULL);
  pthread_cancel(thread_id);
  
  //  ---------------------------------------------------------------- ACA PARTE EL JUEGO 
  
  int monster_msg_code = -1;
  char *select_monster = "Por favor elija un monstruo";
  server_send_message(players_info->socket_c1, 6, select_monster);

  while(monster_msg_code != 5){
    monster_msg_code = server_receive_id(players_info->socket_c1);
  }
  char* client_monster_message = server_receive_payload(players_info->socket_c1);
  int monster_id = client_monster_message[0] - '0';
  printf("ID MONSTRUO: %i", monster_id);
  monster_ready(monster, monster_id);
  printf("El lider dice: %s\n", client_monster_message);
  
  char eleccion_monstruo[255];
  sprintf(eleccion_monstruo, "Elegiste el monstruo %s\n \n Empieza el juego!", monster -> name);
  if (n_connected == 1){
    sockets[0] = players_info->socket_c1;
  }
  for (int i = 0; i < n_connected; i++)
  {
    if (sockets[i] != -1)
      {
    server_send_message(sockets[i], 5, eleccion_monstruo);
      }
  }
  
  printf("Monstruo AAAAAAAAAAAAAAA %s, vida %i\n", monster -> name, monster -> hp);
  while (turn_count < 15)
  {
    for (int i = 0; i < n_connected; i++)
    {
      int option = -1;
      char turnn[255];
      sprintf(turnn, "\nTurno de %s\n", players[i]->name);
      for (int u = 0; u < n_connected; u++)
      {
        server_send_message(sockets[u], 5, turnn);
      }
      char turn[255];
      sprintf(turn, "Es tu turno");
      printf("mi id es: %i\n", players[i]->id_class);
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
    
    while(option != 6){
      option = server_receive_id(sockets[i]);
    }
    char *player_option = server_receive_payload(sockets[i]);
    int n = player_option[0] - '0';
    // p_use_ability(players[i], player_option_int);
    char action[255];
    if (n == 4){
      // morirse
      }
    if (players[i] -> id_class == 1)
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
          else
          {
            for (int j = 0; j < n_connected; i++){
              players[j] -> distracted = 0;
            }
            distraer(players[i], monster);
          }
          
      }
    else if (players[i] -> id_class == 2)
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
          else
          {
            descarga(players[i], monster);
          }
      }
    else
      {
          //HACKER
          if (n == 1)
          {
            inyeccion(players[i]);
          }
          else if (n == 2)
          {
            ataque_ddos(players[i], monster);
          }
          else
          {
            fuerza_bruta(players[i], monster);
          }
      }
  // FIN TURNO PLAYER
  if (players[i] -> bonus > 0)
  {
    players[i] -> bonus --;
  }

  if (players[i] -> toxin > 0)
  {
    players[i] -> toxin --;
  }
  
  if (players[i] -> bleeds > 0)
  {
    players[i] -> bleeds --;
  }
  // FIN DE TODOS LOS TURNOS DE PLAYERS
  }
  int number = getRandom(1, 11);
  printf("number: %i\n", number);
  if (monster_id == 1){
    if (number <= 5)
    {
      ruzgar();
    }
    else
    {
      coletazo();
    }
  }
  else if (monster_id == 2)
  {
    if (monster -> jump == 1){
      espina_venenosa();
    }
    else{
    if (number <= 4)
    {
      salto();
    }
    else
    {
      espina_venenosa();
    }
    }
  }
  else
  {
    if (number <= 4)
    {
      caso_de_copia();
    }
    else if (number > 4 && number <=6){
      reprobatron();
    }
    else
    {
      sudo();
    }
    
  }
    // FIN TURNO MONSTER
    turn_count ++;
    if (monster -> jump > 0)
    {
      monster -> jump = 0;
    }
  }

  return 0;
}
