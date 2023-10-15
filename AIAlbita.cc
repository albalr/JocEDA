#include "Player.hh"


/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME Albita


struct PLAYER_NAME : public Player {

/**
 * Factory: returns a new instance of this class.
 * Do not modify this function.
 */
static Player* factory () {
  return new PLAYER_NAME;
}

/**
 * Types and attributes for your player can be defined here.
 */

typedef vector<int> vec; 
typedef vector<Dir> vecDir;
typedef vector<vector<pair<int,Dir>> > grafDir;

vecDir directions = {Up,Down,Left,Right}; //directions of the alive units

//moves randomly
void move_random(int id_alive) {

  Dir d = directions[random(0,directions.size()-1)];
  Pos new_pos = unit(id_alive).pos + d;
  if (pos_ok(new_pos) and cell(new_pos.i,new_pos.j).type != Waste) move(id_alive,d);
}

//returns wether there is a zombie next to the player
bool adjacent_zombie(int alive_id) {
  
  for (int d = 0; d < directions.size(); ++d) {
    Pos new_pos = unit(alive_id).pos + directions[d];
    if (pos_ok(new_pos)) {
      int id_in_cell = cell(new_pos.i,new_pos.j).id;
      if (id_in_cell != -1 and unit(id_in_cell).type == Zombie) {move(alive_id,directions[d]); return true;}
    }
  }
  return false;
}

bool is_zombie(int id, Pos p_zombie) {
  if (pos_ok(p_zombie)) {
    int id_in_cell = cell(p_zombie.i,p_zombie.j).id;
    if (id_in_cell != -1 and unit(id_in_cell).type == Zombie) return true;
  }
  return false;
}

bool adjacent_enemy(int alive_id) {

  bool atack = false;

  for (int d = 0; d < directions.size(); ++d) {
    Pos new_pos = unit(alive_id).pos + directions[d];
    if (pos_ok(new_pos)) {

      int id_in_cell = cell(new_pos.i,new_pos.j).id;
      int id_enemy = cell(new_pos.i,new_pos.j).owner;

      if (id_in_cell != -1 and unit(id_in_cell).type == Alive and id_enemy != me()) {
        
        if (/*random(0,3) <= 1 or*/ strength(me()) > strength(id_enemy)) {
          move(alive_id,directions[d]);
          atack = true;
          break;
        }
      }
    }
  } 
  return atack;
}

bool is_enemy(int id_my, Pos p_enemy) {

  int id_in_cell = cell(p_enemy).id;
  int id_enemy = cell(p_enemy).owner;

  if (id_in_cell != -1 and unit(id_in_cell).type == Alive and id_enemy != id_my) return true;   
  
  return false;
}

// void scape(int id_alive) {

// }

Dir direction_to(const grafDir &dist, Pos pos) {
  stack<Dir> rute; 

  int f_i = pos.i;
  int c_i = pos.j;

  int distance = dist[f_i][c_i].first;
  int distance_ant;

  Pos new_pos;

  while (distance >= 1) {

    bool found = false;
    int d = 0;
    while (d < int(directions.size()) and not found) {
      new_pos = pos + directions[d];

      int f = new_pos.i;
      int c = new_pos.j;
      if (pos_ok(new_pos)) {

        distance_ant = dist[f][c].first;

        if (distance_ant == distance - 1) {rute.push(dist[pos.i][pos.j].second); found = true;}
      }
      ++d;
    }

    distance = distance_ant;
    pos = new_pos;

  }
  
  return rute.top();
  
}

//bfs functions to search for food
void there_is_enemy(int id_alive) {

  bool enemy = false;
  bool food = false;
  Pos pos_f;
  Pos pos_e;
  grafDir dist(60,vector<pair<int,Dir>>(60,{-1,Up}));

  Pos pos_ini = unit(id_alive).pos;

  int fila_ini = pos_ini.i; 
  int columna_ini = pos_ini.j;
  dist[fila_ini][columna_ini].first = 0;

  queue<Pos> q;
  q.push(pos_ini);

  Dir where;
  Dir where_f;

  while (not q.empty()) {
    Pos p = q.front();
    
    q.pop();
    int f = p.i; 
    int c = p.j;
    for (auto d : directions) {
      Pos new_pos = p + d;
      int n_f = new_pos.i;
      int n_c = new_pos.j;

      if (pos_ok(n_f,n_c) and cell(n_f,n_c).type != Waste and dist[n_f][n_c].first == -1) {
        q.push(new_pos);
        dist[n_f][n_c].first = dist[f][c].first + 1;
        dist[n_f][n_c].second = d;

        if (not enemy) if (is_enemy(id_alive,new_pos)) {enemy = true; pos_e = new_pos; where = direction_to(dist, new_pos); }
        if (not food) if (cell(n_f,n_c).food) {food = true; pos_f = new_pos; where_f = direction_to(dist, new_pos);}

      }
    }
  }
  cerr << " ENEMIC A " << pos_e << " HE DANAR A " << where << " ESTIC A " << unit(id_alive).pos << endl;
  cerr << " MENJAR A " << pos_f << " HE DANAR A " << where_f << " ESTIC A " << unit(id_alive).pos << endl;
  if (enemy and round() > 100) {
    //cerr << " ENEMIC A " << pos_e << " HE DANAR A " << where << " ESTIC A " << unit(id_alive).pos << endl;
    int id_in = cell(pos_e).owner;
    if (strength(id_in) < strength(me())) move(id_alive, where); 
    else {
      if (food) move(id_alive, where_f);
      cerr << "lenemic te mes punts NO ATACO" << endl;
    }
  }
  else move(id_alive, where_f);
  //else move_random(id_alive);
}

/**
 * Play method, invoked once per each round.
 */
virtual void play () {

  int my_player = me();
  vec alive = alive_units(my_player); //alive units vector
  
  for (int id = 0; id < int(alive.size()); ++id) {

    if (not adjacent_zombie(alive[id])) 
      //if (not adjacent_enemy(alive[id]))
        there_is_enemy(alive[id]); 
  }
}

};

/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
