#include "morse.h"
#include <map>
#include "MG90S.h"

// Table Morse vers lettres
static std::map<String, char> morseTable = {
  {".-", 'A'},   {"-...", 'B'}, {"-.-.", 'C'}, {"-..", 'D'},  {".", 'E'},
  {"..-.", 'F'}, {"--.", 'G'},  {"....", 'H'}, {"..", 'I'},   {".---", 'J'},
  {"-.-", 'K'},  {".-..", 'L'}, {"--", 'M'},   {"-.", 'N'},   {"---", 'O'},
  {".--.", 'P'}, {"--.-", 'Q'}, {".-.", 'R'},  {"...", 'S'},  {"-", 'T'},
  {"..-", 'U'},  {"...-", 'V'}, {".--", 'W'},  {"-..-", 'X'}, {"-.--", 'Y'},
  {"--..", 'Z'}
};

static std::map<char, String> letterToMorse = {
  {'A', ".-"},   {'B', "-..."}, {'C', "-.-."}, {'D', "-.."},  {'E', "."},
  {'F', "..-."}, {'G', "--."},  {'H', "...."}, {'I', ".."},   {'J', ".---"},
  {'K', "-.-"},  {'L', ".-.."}, {'M', "--"},   {'N', "-."},   {'O', "---"},
  {'P', ".--."}, {'Q', "--.-"}, {'R', ".-."},  {'S', "..."},  {'T', "-"},
  {'U', "..-"},  {'V', "...-"}, {'W', ".--"},  {'X', "-..-"}, {'Y', "-.--"},
  {'Z', "--.."}
};


static String secretWord = "";
static String currentMessage = "";

void initMorse(const String &secret) {
  secretWord = secret;
  currentMessage = "";
}


void handleMorseInput() {
  String morseInput = "";
  unsigned long pressStart = 0;
  unsigned long lastActionTime = millis();
  bool isPressed = false;

  print_screen("Tapez en Morse");

  while (true) {
    bool btnState = digitalRead(BUTTON_PIN) == LOW;
    unsigned long now = millis();

    // Détection de l'appui
    if (btnState && !isPressed) {
      isPressed = true;
      pressStart = now;
    }

    // Détection du relâchement
    if (!btnState && isPressed) {
      isPressed = false;
      unsigned long duration = now - pressStart;
      morseInput += (duration < 500) ? '.' : '-';

      print_screen("Saisie: " + morseInput);
      lastActionTime = now;
    }

    // Fin de saisie après 3 secondes d'inactivité
    if (!isPressed && morseInput.length() > 0 && now - lastActionTime > 3000) {
      print_screen("Fin de saisie");
      delay(1000);

      // Traduction du mot secret en morse concaténé
      static std::map<char, String> letterToMorse = {
        {'A', ".-"},   {'B', "-..."}, {'C', "-.-."}, {'D', "-.."},
        {'E', "."},    {'F', "..-."}, {'G', "--."},  {'H', "...."},
        {'I', ".."},   {'J', ".---"}, {'K', "-.-"},  {'L', ".-.."},
        {'M', "--"},   {'N', "-."},   {'O', "---"},  {'P', ".--."},
        {'Q', "--.-"}, {'R', ".-."},  {'S', "..."},  {'T', "-"},
        {'U', "..-"},  {'V', "...-"}, {'W', ".--"},  {'X', "-..-"},
        {'Y', "-.--"}, {'Z', "--.."}
      };

      String secretMorse = "";
      for (char c : secretWord) {
        char uc = toupper(c);
        if (letterToMorse.count(uc)) {
          secretMorse += letterToMorse[uc];
        }
      }

      print_screen("Vous: " + morseInput);
      delay(1000);
      print_screen("Attendu: " + secretMorse);
      delay(1000);

      if (morseInput == secretMorse) {
        print_screen("Bravo !");
        servo_write(180);
        delay(5000);
        servo_write(-180);  // Activation du moteur
      } else {
        print_screen("Raté");
      }

      delay(2000);
      print_screen("Prêt");
      return;
    }

    delay(10);
  }
}




// //TEST INO
// #include "morse.h"

// // Définis ton pin bouton ici (exemple GPIO0)
// #define BUTTON_PIN 0

// void print_screen(String message) {
//   // Ta fonction d'affichage sur écran ici
//   Serial.println("[Ecran] " + message);
// }

// void setup() {
//   Serial.begin(115200);
//   pinMode(BUTTON_PIN, INPUT_PULLUP);

//   initMorse("AB");  // Initialise le mot secret "AB"
// }

// void loop() {
//   // Exemple : détecte appui simple et lance la saisie morse
//   if (digitalRead(BUTTON_PIN) == LOW) {
//     delay(50); // Anti-rebond basique
//     if (digitalRead(BUTTON_PIN) == LOW) {
//       while (digitalRead(BUTTON_PIN) == LOW); // Attend relâchement
//       handleMorseInput();
//     }
//   }
// }
