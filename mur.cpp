#include "mur.h"

mur::mur(Minitel* m, int* l) {

  this->m = m;

  // Construction du mur
  life = 0;
  for (int i = 0; i < WALLSIZE; i++) {

    if (bri[i])
      delete bri[i];

    bri[i] = NULL;

    if (l[i] == 0)
      bri[i] = new brique(m, ((i % 10) * 4) + 1, (i / 10) + 1);
    if (l[i] == 1)
      bri[i] = new brique_incassable(m, ((i % 10) * 4) + 1, (i / 10) + 1);

    if (bri[i])
      if (bri[i]->getLife() > 0)
        life++;
  }
}
mur::~mur() {

  for (int i = 0; i < WALLSIZE; i++) {

    if (bri[i])
      delete bri[i];

  }
}

void mur::draw() {

  bool lignage = true;

  for (int i = 0; i < WALLSIZE; i++) {
    
    if (bri[i])

      if (bri[i]->getLife() == 0)
        // Détruit la brique
        removeBrique(i);
      else {
        if (bri[i]->isRedraw() && lignage) {
          // Si une brique doit être redessiner,
          // passe en mode lignage
          m->attributs(DEBUT_LIGNAGE);
          lignage = false;
        }
        bri[i]->draw();
      }
  }
}

bool mur::checkCollision(balle& b, int& score) {

  for (int i = WALLSIZE - 1; i >= 0; i--) {

    if (bri[i]) {

      if (b.getSpeed_Y() == -1 && b.getPos_Y() <= bri[i]->getPos_Y() + 1) { // Collision sur une brique par la balle venant du bas
        if (b.getPos_X() >= bri[i]->getPos_X() && b.getPos_X() <= bri[i]->getPos_X() + bri[i]->getSize_X()) {
          
          int l = bri[i]->hit();

          b.bounceY();

          if (l == 0) {
            score++;
          }

          return true;
        }
      }
      if (b.getSpeed_Y() == 1 && b.getPos_Y() <= bri[i]->getPos_Y() + 1) { // Collision sur une brique par la balle venant du dessus
        if (b.getPos_X() >= bri[i]->getPos_X() && b.getPos_X() <= bri[i]->getPos_X() + bri[i]->getSize_X()) {

          int l = bri[i]->hit();

          b.bounceY();

          if (l == 0) {
            score++;
          }

          return true;
        }
      }
      if (b.getPos_Y() == bri[i]->getPos_Y()) { // Collision sur le côté
        if (b.getPos_X() == bri[i]->getPos_X() - 1 || b.getPos_X() == bri[i]->getPos_X() + bri[i]->getSize_X() + 1) {

          int l = bri[i]->hit();

          b.bounceX();
          
          if (l == 0) {
            score++;
          }

          return true;
        }
      }
    }
  }

  return false;
}

void mur::removeBrique(int i) {
  // Détruit la brique
  bri[i]->erase();

  delete bri[i];
  bri[i] = NULL;
}
