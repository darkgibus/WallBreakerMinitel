#include "balle.h"

const int balle::up = -1;
const int balle::down = 1;

balle::balle(Minitel* m, raquette* r) {

  this->m = m;
  this->r = r;

  offset_X = r->getSize() / 2;
  pos_X = r->getPos_X() + offset_X;
  pos_Y = r->getPos_Y() - 1;
  lastPos_X = pos_X;
  lastPos_Y = pos_Y;
  speed_Y = -1;
  speed_X = 1;
  isLaunch = false;
  redraw = true;
  out = false;

  sm = false;
  st = true;
}
balle::~balle() {
  erase();
}

void balle::bounceX() {
  speed_X = -speed_X;
}
void balle::bounceY() {
  speed_Y = -speed_Y;
}

void balle::update(raquette* r) {

  if (out)
    return;

  if (isLaunch) {

    if (!st)
      return;

    if (sm)
      st = false;
    else
      st = true;

    pos_Y += speed_Y;
    pos_X += speed_X;

    if (pos_X <= 1) {   // Rebond à gauche
      pos_X = 1;
      bounceX();
    }
    if (pos_X >= 40) {  // Rebond à droite
      pos_X = 40;
      bounceX();
    } 

    if (pos_Y <= 1) {   // Rebond sur le haut de l'écran
      pos_Y = 1;
      bounceY();
    }
    if (pos_Y >= r->getPos_Y() - 1 && pos_X >= r->getPos_X() && pos_X <= r->getPos_X() + r->getSize()) {  // Collision avec la raquette
      bounceY();
      if (pos_X < (r->getPos_X() + (r->getSize() / 2)))
        speed_X = -1;
      else
        speed_X = 1;
    }
    if (pos_Y >=25 ) {  // bas de l'écran
      pos_Y = 25;
      speed_Y = 0;
      speed_X = 0;
      out = true;   // Balle perdu !
    }

  } else {

    pos_Y = r->getPos_Y() - 1;
    pos_X = r->getPos_X() + offset_X;
  }
}

void balle::launch() {
  isLaunch = true;
  speed_Y = -1;
  speed_X = 1;
}

void balle::draw() {

  if (pos_X != lastPos_X || pos_Y != lastPos_Y) 
    erase();

  if (redraw) {

    m->attributs(FIN_LIGNAGE);

    m->graphic(63, pos_X, pos_Y);
    redraw = false;
  }
}

void balle::stepMode(bool s) {
  sm = s;
}
void balle::step() {
  st = true;
}

int balle::getPos_Y() const {
  return pos_Y;
}
int balle::getPos_X() const {
  return pos_X;
}
int balle::getSpeed_Y() const {
  return speed_Y;
}

bool balle::isOut() {
  return out;
}

void balle::erase() {

  m->graphic(0, lastPos_X, lastPos_Y);

  lastPos_X = pos_X;
  lastPos_Y = pos_Y;

  redraw = true;
}