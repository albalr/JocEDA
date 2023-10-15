#include "Player.hh"

/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME Vita


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
vecDir diagonals = {DR,RU,UL,LD}; //directions of the zombies

//moves randomly
void move_random(int id_alive) {

  Dir d = directions[random(0,directions.size()-1)];
  Pos new_pos = unit(id_alive).pos + d;
  if (pos_ok(new_pos) and cell(new_pos.i,new_pos.j).type != Waste) move(id_alive,d);
}

bool ok(Pos p) {
  if (pos_ok(p)) {
    if (cell(p).type != Waste) {
      if(cell(p).id == -1 /*and ((cell(p).owner == me() and cell(p).id == -1) or cell(p).owner != -1)*/) {
        cerr << " there is anyone in " << p << endl;
        return true;
      }
    } 
  }
  return false;
}

void scape(int id_alive, Dir d) {

  if (d == Up) {
    if (pos_ok(unit(id_alive).pos + Down)) move(id_alive, Down);
    else if (pos_ok(unit(id_alive).pos + Right)) move(id_alive, Right);
    else if (pos_ok(unit(id_alive).pos + Left)) move(id_alive, Left);
  }

  if (d == Down) {
    if (pos_ok(unit(id_alive).pos + Up)) move(id_alive, Up);
    else if (pos_ok(unit(id_alive).pos + Right)) move(id_alive, Right);
    else if (pos_ok(unit(id_alive).pos + Left)) move(id_alive, Left);
  }

  if (d == Right) {
    if (pos_ok(unit(id_alive).pos + Left)) move(id_alive, Left);
    else if (pos_ok(unit(id_alive).pos + Up)) move(id_alive, Up);
    else if (pos_ok(unit(id_alive).pos + Down)) move(id_alive, Down);
  }

  if (d == Left) {
    if (pos_ok(unit(id_alive).pos + Right)) move(id_alive, Right);
    else if (pos_ok(unit(id_alive).pos + Up)) move(id_alive, Up);
    else if (pos_ok(unit(id_alive).pos + Down)) move(id_alive, Down);
  }
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

bool zombie_around(int alive_id) {
  Dir zombie_around; 
  Pos my_pos;
  for (int d = 0; d < diagonals.size(); ++d) {
    
    Pos new_pos = unit(alive_id).pos + diagonals[d];
    if (pos_ok(new_pos)) {
      
      int id_in_cell = cell(new_pos).id;

      if (id_in_cell != -1 and unit(id_in_cell).type == Zombie) {
        zombie_around = diagonals[d];
        my_pos = unit(alive_id).pos;

        cerr << "ESTIC A " << my_pos << endl; 
        cerr << "EL ZOMBIE A " << zombie_around << endl;

        if (zombie_around == DR) {
          if (ok(my_pos+Left)) {move(alive_id, Left);cerr << "hem moc L " << endl;return true;}
          else if (ok(my_pos+Up)) {move(alive_id, Up);cerr << "hem moc U " << endl;return true;}
          else if (ok(my_pos+Right)) {move(alive_id, Right);cerr << "hem moc R " << endl;return true;}
          else if (ok(my_pos+Down)) {move(alive_id, Down);cerr << "hem moc D " << endl;return true;}
        }
        
        else if (zombie_around == RU) {
          if (ok(my_pos+Left)) {move(alive_id, Left);cerr << "hem moc L " << endl;return true;}
          else if (ok(my_pos+Down)) {move(alive_id, Down);cerr << "hem moc D " << endl;return true;}
          else if (ok(my_pos+Right)) {move(alive_id, Right);cerr << "hem moc R " << endl;return true;}
          else if (ok(my_pos+Up)) {move(alive_id, Up);cerr << "hem moc U " << endl;return true;}
        }
        else if (zombie_around == UL) {
          if (ok(my_pos+Right)) {move(alive_id, Right);cerr << "hem moc R " << endl;return true;}
          else if (ok(my_pos+Down)) {move(alive_id, Down);cerr << "hem moc D " << endl;return true;}
          else if (ok(my_pos+Up)) {move(alive_id, Up);cerr << "hem moc U " << endl;return true;}
          else if (ok(my_pos+Left)) {move(alive_id, Left);cerr << "hem moc L " << endl;return true;}
        }
        else if (zombie_around == LD) {
          if (ok(my_pos+Right)) {move(alive_id, Right);cerr << "hem moc R " << endl; return true;}
          else if (ok(my_pos+Up)) {move(alive_id, Up);cerr << "hem moc U " << endl;return true;}
          else if (ok(my_pos+Left)) {move(alive_id, Left);cerr << "hem moc L " << endl;return true;}
          else if (ok(my_pos+Down)) {move(alive_id, Down);cerr << "hem moc D " << endl;return true;}
        }
      }
    }
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
        move(alive_id,directions[d]);
        atack = true;
        break;
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
void strategy(int id_alive) {

  bool food = false;
  bool stop = false;
  bool zombie = false;

  Pos pos_f, pos_z;

  int dist_food, dist_zombie;
  grafDir dist(60,vector<pair<int,Dir>>(60,{-1,Up}));

  Pos pos_ini = unit(id_alive).pos;

  int fila_ini = pos_ini.i; 
  int columna_ini = pos_ini.j;
  dist[fila_ini][columna_ini].first = 0;

  queue<Pos> q;
  q.push(pos_ini);

  Dir where_f, where_z;

  while (not q.empty() and not stop) {

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

        if (not food) if (cell(n_f,n_c).food) {food = true; pos_f = new_pos; where_f = direction_to(dist, new_pos);dist_food = dist[n_f][n_c].first;}
        if (not zombie) if (is_zombie(id_alive,new_pos)) {zombie = true; pos_z = new_pos; where_z = direction_to(dist, new_pos); dist_zombie = dist[n_f][n_c].first;}
        if (zombie and food) {stop = true; break;}
      }
    }
  }

  if (dist_zombie < dist_food) {
    if (dist_zombie > 2) move(id_alive,where_z);
    else move(id_alive, where_f);
  }
  else move(id_alive, where_f); 
}

/**
 * Play method, invoked once per each round.
 */
virtual void play () {

  int my_player = me();
  vec alive = alive_units(my_player); //alive units vector
  vec move_last; 
  vec move_after;

  for (int id = 0; id < int(alive.size()); ++id) {
      if (not adjacent_enemy(alive[id])) move_last.push_back(alive[id]);
  }

  // for (int j = 0; j < int(move_after.size()); ++j) {
  //     if (not zombie_around(move_after[j])) move_last.push_back(move_after[j]);
  // }

  for (int i = 0; i < int(move_last.size()); ++i) {

    if (not zombie_around(move_last[i]))
      if (not adjacent_zombie(move_last[i])) 
        strategy(move_last[i]); 
  }
}

};

/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);