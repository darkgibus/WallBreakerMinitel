#include <Minitel1B_Hard.h>

#include "raquette.h"
#include "balle.h"
#include "brique.h"
#include "mur.h"

#define MAXLIFE 3

Minitel minitel(Serial2,16,17);

raquette* raq = NULL;
balle* bal = NULL;
mur* mr = NULL;

int score = 0;
int life = MAXLIFE;

int level[WALLSIZE] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 1, 0, 0, 0, 0, 1, 0, 0,
                        0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

void resetGame() {

  minitel.graphicMode();      // Mode graphique
  minitel.clearScreen();      // Efface l'écran
  minitel.noCursor();         // Cache le curseur
  minitel.echo(false);        // Désactive l'écho local
  minitel.extendedKeyboard();

  // Redémarre le jeux
  if (raq)
    delete raq;
  if (bal)
    delete bal;
  if (mr)
    delete mr;

  raq = new raquette(&minitel);
  bal = new balle(&minitel, raq);
  mr = new mur(&minitel, level);

  score = 0;
  life = MAXLIFE;

  while(minitel.getKeyCode());  // Attend que le buffer de touches soit vide

}

void drawHUD() {

  static int oldLife = -1;
  static int oldScore = -1;

  // Affiche la vie et le score
  if (oldLife != life || oldScore != score) {
    minitel.textMode();
    minitel.newXY(1, 0);  // Ligne de status

    for (int i = 0; i < MAXLIFE; i++) {
      if (i < life) {
        minitel.print(" #");
      } else {
        minitel.print("  ");
      }
    }
    
    minitel.print("    Score: ");
    minitel.print(String(score));
    minitel.print("     ");
    minitel.newXY(1, 1);  // Repositionner le curseur hors de la ligne de status, sinon ça fait planter le minitel ??!!
    minitel.graphicMode();

    oldLife = life;
    oldScore = score;
  }
}

void setup() {

  Serial.begin(115200);  // Le port de débug

  int speed = minitel.searchSpeed();
  if (speed > 0) {
    minitel.changeSpeed(4800); // 4800 bauds accélère considérablement l'affichage
  }

  resetGame();

}

void loop() {

  /*---------------------*/
  /* Gestion des entrées */
  /*---------------------*/
  unsigned long touche = minitel.getKeyCode();

  if (touche) {

    if (touche == TOUCHE_FLECHE_GAUCHE || touche == 'Q') {
      raq->moveLeft();
    }
    if (touche == TOUCHE_FLECHE_DROITE || touche == 'D') {
      raq->moveRight();
    }
    if (touche == ' ') {
      bal->launch();
    }
  }

  const unsigned long interval = 100;
  unsigned long currentTime = millis();
  static unsigned long memo_currentTime = 0;

  if (currentTime - memo_currentTime >= interval) {
    memo_currentTime = currentTime;

    bal->update(raq);

    mr->checkCollision(*bal, score);
  }

  // Mise à jour de l'affichage
  raq->draw();
  bal->draw();
  mr->draw();
  drawHUD();
  
  if (bal->isOut()) {
    
    minitel.bip();

    life--;

    if (life <= 0) {
      // Met à jour le HUD
      drawHUD();
      
      // Affiche le game over
      minitel.textMode();
      minitel.attributs(FIN_LIGNAGE);
      minitel.attributs(DOUBLE_GRANDEUR);
      minitel.moveCursorXY(12, 12);
      minitel.print("GAME OVER");

      delay(3000);

      // Relance le jeux
      resetGame();
    } else {

      delete raq;
      delete bal;

      raq = new raquette(&minitel);
      bal = new balle(&minitel, raq);
    }
  }
}
