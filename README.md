# 🔐 Smart Taquin Puzzle Project

This repository contains a smart, interactive **3x3 Taquin (sliding puzzle)** developed as part of a team project in engineering school. Once solved, the puzzle delivers a **hidden Morse code**, ultimately revealing a **secret key**.

## 🧠 Project Objective

The goal of this project was to design and build a physical 3x3 sliding puzzle game (Taquin) which, upon completion, reveals a secret code in **Morse**. This code can be used by the player to unlock a hidden key. The game is meant to simulate a blend of mechanical interaction and embedded intelligence.

## 🎮 How It Works

- The puzzle consists of **8 numbered pieces** and one blank slot.
- The initial state of the puzzle is scrambled.
- The player must rearrange the tiles into correct order (1 to 8, left to right, top to bottom).
- After **5 minutes of inactivity or failure**, hints become available.
- Once completed, the puzzle **emits a Morse code** through LEDs, screen, or sound (e.g., piezo buzzer).
- This Morse code corresponds to a secret message or key.

## 🛠️ Technical Overview

- The hardware is powered by an **ESP32-S3 microcontroller**.
- A **color sensor matrix** detects the arrangement of tiles.
- An **OLED display** or **LED system** provides user feedback and hint prompts.
- Servo motors and buttons allow for potential future automation and interactions.
- The enclosure and tile structure were modeled and prototyped using **3D printing**.
- Future expansions could include a **camera version** for tile detection.

## 📁 Project Structure

├── CODE_PROJET/ # All Arduino & C++ logic
│ ├── TaquinSolver.cpp # Puzzle logic
│ ├── morse.cpp # Morse code generation
│ ├── MG90S.cpp # Servo control
│ └── ...
├── Images/ # Diagrams and prototype photos
├── Documentation/ # Component datasheets and manuals
├── Explication_Projet.pdf # Original project description (in French)
├── Rapport.md # Detailed technical report
└── README.md # Project overview


## 👨‍💻 Team & Contributions

This project was developed as part of a **team engineering assignment**, with each member contributing to:
- Embedded programming
- Electronic circuit design
- Mechanical prototyping
- Documentation and testing

> This repository was maintained and uploaded by **[your name]**, with key contributions in software development and system integration.

## 📷 Images

<img src="Images/taquin.png" alt="Puzzle Layout" width="400"/>
<img src="Images/ESP32S3.jpg" alt="ESP32 Microcontroller" width="300"/>

## 📜 License

This project is licensed under the MIT License – see the [LICENSE](LICENSE) file for details.

## 🚀 Future Enhancements

- Add automatic tile shuffling mechanism
- Implement audio Morse playback
- Create a mobile interface for hints and result display
- Improve camera version with real-time tile detection

---

Feel free to fork, improve, or suggest features. This was a great learning experience in merging physical computing with game logic.


