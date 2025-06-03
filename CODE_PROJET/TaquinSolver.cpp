#include "TaquinSolver.h"
#include <queue>
#include <unordered_set>
#include <algorithm>
#include <Arduino.h>

//Ecran
#include "oled_display.h"


#define SIZE 3
#define EMPTY 9

struct State {
  uint8_t board[SIZE * SIZE];
  int g_cost;
  int h_cost;
  int f_cost;
  int empty_pos; // position de la case vide (0..8)
  std::vector<uint8_t> path; // liste des tuiles déplacées

  State(const std::vector<uint8_t>& b, int g, int h, int empty_p, const std::vector<uint8_t>& p)
    : g_cost(g), h_cost(h), f_cost(g+h), empty_pos(empty_p), path(p) {
      std::copy(b.begin(), b.end(), board);
  }

  // Pour priority_queue (tri par f_cost croissant)
  bool operator>(const State& other) const {
    return f_cost > other.f_cost;
  }
};

// Hash pour unordered_set avec std::vector<uint8_t>
struct StateHash {
  size_t operator()(const std::vector<uint8_t>& v) const {
    size_t hash = 0;
    for (auto x : v) {
      hash = hash * 31 + x;
    }
    return hash;
  }
};

// Heuristique Manhattan
int manhattan(const std::vector<uint8_t>& board) {
  int dist = 0;
  for (int i = 0; i < SIZE * SIZE; i++) {
    uint8_t val = board[i];
    if (val != EMPTY) {
      int goal_row = (val - 1) / SIZE;
      int goal_col = (val - 1) % SIZE;
      int cur_row = i / SIZE;
      int cur_col = i % SIZE;
      dist += abs(goal_row - cur_row) + abs(goal_col - cur_col);
    }
  }
  return dist;
}

std::vector<uint8_t> a_star(const std::vector<uint8_t>& start, const std::vector<uint8_t>& goal) {
  int empty_pos = std::find(start.begin(), start.end(), EMPTY) - start.begin();

  std::priority_queue<State, std::vector<State>, std::greater<State>> openSet;
  std::unordered_set<std::vector<uint8_t>, StateHash> closedSet;

  openSet.emplace(start, 0, manhattan(start), empty_pos, std::vector<uint8_t>());

  // Déplacements possibles (haut, bas, gauche, droite)
  const int moves[4] = {-SIZE, SIZE, -1, 1};

  while (!openSet.empty()) {
    State current = openSet.top();
    openSet.pop();

    std::vector<uint8_t> cur_board(current.board, current.board + SIZE * SIZE);

    if (cur_board == goal) {
      return current.path; // chemin trouvé
    }

    if (closedSet.find(cur_board) != closedSet.end()) {
      continue;
    }
    closedSet.insert(cur_board);

    int er = current.empty_pos;
    int er_row = er / SIZE;
    int er_col = er % SIZE;

    for (int i = 0; i < 4; i++) {
      int nr = er_row + (moves[i] == -SIZE ? -1 : (moves[i] == SIZE ? 1 : 0));
      int nc = er_col + (moves[i] == -1 ? -1 : (moves[i] == 1 ? 1 : 0));

      if (nr >= 0 && nr < SIZE && nc >= 0 && nc < SIZE) {
        int new_pos = nr * SIZE + nc;
        std::vector<uint8_t> new_board = cur_board;
        std::swap(new_board[er], new_board[new_pos]);

        if (closedSet.find(new_board) != closedSet.end())
          continue;

        std::vector<uint8_t> new_path = current.path;
        new_path.push_back(new_board[er]); // tuile déplacée

        int g = current.g_cost + 1;
        int h = manhattan(new_board);

        openSet.emplace(new_board, g, h, new_pos, new_path);
      }
    }
  }

  return {}; // pas de solution
}

void get_first_move(const std::vector<uint8_t>& start, const std::vector<uint8_t>& goal) {
  // Vérifie que la matrice de départ contient bien les 9 numéros uniques de 0 à 8
  auto is_valid = [](const std::vector<uint8_t>& board) {
    if (board.size() != SIZE * SIZE) return false;
    std::vector<bool> seen(SIZE * SIZE, false);
    for (uint8_t val : board) {
      if (val >= SIZE * SIZE || seen[val]) {
        return false;
      }
      seen[val] = true;
    }
    return true;
  };

  if (!is_valid(start)) {
    print_screen("Erreur lecture caméra");
    return;
  }

  std::vector<uint8_t> path = a_star(start, goal);
  if (!path.empty()) {
    print_screen("Move: " + String(path[0])); // Affiche la première tuile à déplacer
  } else {
    print_screen("Pas de solution");
  }
  Serial.println("Move: " + String(path[0]));
}



// //TEST INO
// #include <Arduino.h>
// #include "TaquinSolver.h"

// #define EMPTY 9

// void setup() {
//   Serial.begin(115200);

//   // Etat de départ : une configuration "mélangée"
//   std::vector<uint8_t> start = {
//     EMPTY, 1, 2,
//     4, 5, 3,
//     7, 8, 6
//   };

//   // Etat objectif : taquin ordonné classique
//   std::vector<uint8_t> goal = {
//     1, 2, 3,
//     4, 5, 6,
//     7, 8, EMPTY
//   };

//   Serial.println("Début résolution Taquin 3x3...");

//   std::vector<uint8_t> solution = a_star(start, goal);

//   if (solution.empty()) {
//     Serial.println("Aucune solution trouvée.");
//   } else {
//     Serial.print("Solution trouvée en ");
//     Serial.print(solution.size());
//     Serial.println(" coups :");

//     for (auto move : solution) {
//       Serial.print(move);
//       Serial.print(" ");
//     }
//     Serial.println();
//   }
// }

// void loop() {
//   // Rien à faire en boucle
// }
