#ifndef TAQUINSOLVER_H
#define TAQUINSOLVER_H

#include <vector>
#include <cstdint>

#define EMPTY 9

std::vector<uint8_t> a_star(const std::vector<uint8_t>& start, const std::vector<uint8_t>& goal);
void get_first_move(const std::vector<uint8_t>& start, const std::vector<uint8_t>& goal);


#endif

// //TEST RESOLUTION TAQUIN / OK

//   // Etat de départ : une configuration "mélangée"
//   std::vector<uint8_t> start = {
//     8, 6, 7,
//     2, 5, 4,
//     3, EMPTY, 1
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
// }
// delay(5000);
