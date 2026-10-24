#include "raquette.h"

raquette::raquette(Minitel* m) {

  this->m = m;

  pos_X = (max_X/ 2) - (size /2);
  pos_Y = 24;
  lastPos_X = pos_X;

  redraw = true;
}
raquette::~raquette() {

  lastPos_X = pos_X;
  erase();
}

void raquette::moveLeft() {

  pos_X--;

  if (pos_X < 1) {
    pos_X = 1;
  }
}
void raquette::moveRight() {

  pos_X++;

  if (pos_X > (max_X - size)) {
    pos_X = max_X - size;
  }
}

// Dessine la raquette
//
void raquette::draw() {

  if (redraw || lastPos_X != pos_X) {
    
    m->attributs(FIN_LIGNAGE);

    if (lastPos_X != pos_X) { // La position de la raquette à changée

      if (pos_X > lastPos_X) {  // Déplacement vers la droite
        m->graphic(graph[0], pos_X, pos_Y);
        m->graphic(graph[size - 2], pos_X + size - 2, pos_Y);
        m->graphic(graph[size - 1], pos_X + size - 1, pos_Y);
      } else {                 // Déplacement vers la gauche
        m->graphic(graph[0], pos_X, pos_Y);
        m->graphic(graph[1], pos_X + 1, pos_Y);
        m->graphic(graph[size - 1], pos_X + size - 1, pos_Y);
      }

      // Efface l'ancienne position
      erase();

    } else {
      // Dessine toute la raquette
      for (int i = 0; i < size; i++) {
        m->graphic(graph[i], pos_X + i, pos_Y);
      }
    }

    redraw = false;
  }
}

int raquette::getPos_Y() const {
  return pos_Y;
}
int raquette::getPos_X() const {
  return pos_X;
}
int raquette::getSize() const {
  return size;
}

bool raquette::isInside(int x, int y) {

  return (y == pos_Y) && (x >= pos_X) && (x < pos_X + getSize());
}

// Efface la raquette
//
void raquette::erase() {

  if (pos_X > lastPos_X)  // Déplacement vers la droite
    m->graphic(0, lastPos_X, pos_Y);  // Efface le bord gauche
  else if (pos_X < lastPos_X)
    m->graphic(0, lastPos_X + size - 1, pos_Y); // Efface le bord droit
  else {
    // Efface toute la raquette
    for (int i = 0; i < size; i++) {
      m->graphic(0, lastPos_X + i, pos_Y);
    }
  }

  lastPos_X = pos_X;
  redraw = true;

} 