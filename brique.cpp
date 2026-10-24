#include "brique.h"

brique::brique(Minitel* m, int X, int Y) {

  this->m = m;

  redraw = true;
  pos_X = X;
  pos_Y = Y;

  life = 1;
}

void brique::draw() {

  if (redraw) {
    m->graphic(59, pos_X, pos_Y);
    m->graphic(51, pos_X + 1, pos_Y);
    m->graphic(51, pos_X + 2, pos_Y);
    m->graphic(55, pos_X + 3, pos_Y);
    redraw = false;
  }
}

void brique::erase() {
  m->graphic(0, pos_X, pos_Y);
  m->graphic(0, pos_X + 1, pos_Y);
  m->graphic(0, pos_X + 2, pos_Y);
  m->graphic(0, pos_X + 3, pos_Y);
}

int brique::hit() {

  redraw = true;

  if (life > 0) {
    --life;
  }

  return life;
}

int brique::getPos_Y() const {
  return pos_Y;
}
int brique::getPos_X() const {
  return pos_X;
}
int brique::getSize_X() const {
  return 4;
}
int brique::getLife() const {
  return life;
}
bool brique::isRedraw() const {
  return redraw;
}

bool brique::isInside(int x, int y) {

  return (y == pos_Y) && (x >= pos_X) && (x < pos_X + getSize_X());
}

brique_incassable::brique_incassable(Minitel* m, int X, int Y) : brique(m, X, Y) { 

  life = -1;
}

void brique_incassable::draw() {

  if (redraw) {
    m->graphic(63, pos_X, pos_Y);
    m->graphic(63, pos_X + 1, pos_Y);
    m->graphic(63, pos_X + 2, pos_Y);
    m->graphic(63, pos_X + 3, pos_Y);
    redraw = false;
  }
}

