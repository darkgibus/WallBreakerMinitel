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

      if (bri[i]->getLife() == 0) {
        life--;
        // Détruit la brique
        removeBrique(i);
      } else {
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

void mur::checkCollision(balle& b, int& score) {

  int bx = b.getPos_X();
  int by = b.getPos_Y();

  for (int i = WALLSIZE - 1; i >= 0; i--) {

    if (bri[i]) {

      int brLeft = bri[i]->getPos_X();
      int brRight = bri[i]->getPos_X() + bri[i]->getSize_X() - 1;
      int brTop = bri[i]->getPos_Y() - 1;
      int brBottom = bri[i]->getPos_Y() + 1;

      if (bx >= brLeft && bx <= brRight) {

        if ((by == brBottom && b.getSpeed_Y() == balle::up) || (by == brTop && b.getSpeed_Y() == balle::down)) {

          int l = bri[i]->hit();
          if (l == 0) {
            score++;
          }

          b.bounceY();
          return;
        }
      }
      if (bx == brLeft - 1 || bx == brRight + 1) {

        if (by == bri[i]->getPos_Y()) {
          
          int l = bri[i]->hit();
          if (l == 0) {
            score++;
          }
          
          b.bounceX();
          return;
        }

        if (by == brTop || by == brBottom) {

          int l = bri[i]->hit();
          if (l == 0) {
            score++;
          }

          b.bounceX();
          b.bounceY();
          
          return;
        }
      }
    }
  }
}

void mur::removeBrique(int i) {
  // Détruit la brique
  bri[i]->erase();

  delete bri[i];
  bri[i] = NULL;
}

int mur::getLife() const {

  return life;
}