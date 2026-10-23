#include <Minitel1B_Hard.h>

#include "raquette.h"
#include "balle.h"
#include "brique.h"
#include "mur.h"
#include "timer.h"

#define MAXLIFE 3
#define MAXLEVEL 2

Minitel minitel(Serial2,16,17);

raquette* raq = NULL;
balle* bal = NULL;
mur* mr = NULL;

int score = 0;
int levelNum = 0;
int life = MAXLIFE;

String cheat = "";

int level[MAXLEVEL][WALLSIZE] = {
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0  },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 1, 0, 0, 0, 0, 1, 0, 0,
    0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

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
  mr = new mur(&minitel, level[levelNum]);

  score = 0;
  life = MAXLIFE;

  cheat = "";

  while(minitel.getKeyCode());  // Attend que le buffer de touches soit vide

}

void levelup() {

  minitel.bip();

  drawVictory();

  delay(3000);

  levelNum++;
  if (levelNum >= MAXLEVEL)
    levelNum = 0;

  resetGame();
}

void gameover() {
  // Met à jour le HUD
  drawHUD();
  
  // Affiche le game over
  minitel.textMode();
  minitel.attributs(FIN_LIGNAGE);
  minitel.attributs(DOUBLE_GRANDEUR);
  minitel.moveCursorXY(12, 12);
  minitel.print("GAME OVER");

  delay(3000);

  levelNum = 0;

  // Relance le jeux
  resetGame();
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

void drawVictory() {

  const int c = 31;

  minitel.attributs(FIN_LIGNAGE);
  minitel.textMode();

  String text[6] = {
    { "#    # #   ### ###  ##  ###  ### #" },
    { "#    # #  #     #  #  # #  # #   #" },
    { " #  #  # #      #  #  # ###  ### #" },
    { " #  #  # #      #  #  # # #  #   #" },
    { " #  #  #  #     #  #  # #  # #    " },
    { "  #    #   ###  #   ##  #  # ### #" }
  };

  for (int x = 40; x > 0; x--) {
    for (int i = 0; i < 6; i++) {
      minitel.moveCursorXY(x, 10 + i);
      minitel.print(text[i].substring(0, 40 - x));
    }
  }
}

void setup() {

  Serial.begin(115200);  // Le port de débug

  int speed = minitel.searchSpeed();
  if (speed > 0) {
    minitel.changeSpeed(4800); // 4800 bauds accélère considérablement l'affichage
  }

  levelNum = 0;
  
  resetGame();

}

void loop() {
  
  static timer t(80);
  static timer g(100);
  static timer c(1000);
 
  timer::update();

  /*---------------------*/
  /* Gestion des entrées */
  /*---------------------*/
  unsigned long touche = minitel.getKeyCode();

  if (touche) {
    if (t.isElapsed()) {
      t.reset();

      if (touche == TOUCHE_FLECHE_GAUCHE || touche == 'Q') {
        raq->moveLeft();
      }
      if (touche == TOUCHE_FLECHE_DROITE || touche == 'D') {
        raq->moveRight();
      }
    }
    if (touche == ' ') {
      bal->launch();
    }
  }
  if (touche >= 'A' && touche <= 'Z') {
    c.reset();   

    cheat = cheat + static_cast<char>(touche);

    if (cheat == "LEVELUP") {
      levelup();
    }
  } else if (c.isElapsed()) {
    c.reset();

    cheat = "";
  }

  if (g.isElapsed()) {
    g.reset();

    bal->update(raq);

    mr->checkCollision(*bal, score);
  }

  // Mise à jour de l'affichage
  raq->draw();
  bal->draw();
  mr->draw();
  drawHUD();
  
  if (mr->getLife() <= 0) {
    levelup();
  }
  if (bal->isOut()) {
    
    minitel.bip();

    life--;

    if (life <= 0) {
      gameover();
    } else {

      delete raq;
      delete bal;

      raq = new raquette(&minitel);
      bal = new balle(&minitel, raq);
    }
  }
}
