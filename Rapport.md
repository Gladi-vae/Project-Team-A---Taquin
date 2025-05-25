# Projet d'électronique 1A : Jeu du taquin
#### Par:
-Victor FOUREL \
-Agheles MEKDACHE \
-Rania BENSLIMAN \
-Anya ADJOU
#### Réalisé sous la direction de M. Antoine TAUVEL
# Table des matières
###  Introduction 
### 1- Caméra 
### 2- PCB
### 3- Modèle 3D
### 4- Prototype
### Conclusion
### Bilan
# Introduction 
Dans le cadre du projet d’électronique, nous devions créer un escape game composé de trois chambres. Pour notre chambre, nous avons décidé de réaliser un jeu de taquin. Une fois le puzzle résolu, une clef est délivrée. Cette clef est indispensable pour gagner l’escape game.

<p style="text-align:center;">
  <img src="./taquin.png" alt="Jeu de taquin" style="width:50%;" />
</p>



Le principe du jeu est simple : il s’agit d’un puzzle composé de 8 pièces numérotées de 1 à 8, placées dans une grille de 3x3 avec une case vide. Le joueur doit faire glisser les pièces en utilisant la case vide pour les remettre dans le bon ordre. Le jeu commence avec les pièces dans le désordre, et le joueur doit les réorganiser correctement pour réussir.
## Structure technique

Notre dispositif est composé de plusieurs couches :

- **En dessous** de la structure de jeu se trouve un **capteur**, chargé de suivre les mouvements des pièces et de détecter l’état du puzzle.
- **Le capteur** se trouve sur  un **PCB (circuit imprimé)** qui assure la liaison entre les différents composants électroniques.
- Enfin, la **structure du jeu de taquin en 3D** est positionnée au-dessus de l’ensemble.

Une fois la bonne combinaison atteinte, un mécanisme intégré permet la **libération automatique de la clé**, constituant ainsi la récompense et la progression vers la salle suivante.

# 1-Caméra 
Pour assurer un suivi en temps réel de la progression du joueur, nous avons utilisé la caméra ESP32-S3 Sense. Ce module caméra joue le rôle de capteur intelligent : il analyse l’état du puzzle à chaque instant et détecte automatiquement lorsque le joueur a correctement résolu le jeu. Une fois le bon agencement reconnu, la caméra déclenche la libération de la clé.
<p style="text-align:center;">
  <img src="./ESP32S3.jpg" alt="ESP32S3" style="width:50%;" />
</p>
Ce choix technologique nous a permis d’ajouter une dimension interactive et autonome à notre chambre d’escape game, tout en intégrant des compétences en traitement d’image, microcontrôleurs et automatisation.

# 2- PCB
Notre carte PCB 


