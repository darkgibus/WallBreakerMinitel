#pragma once
#include "balle.h"
#include "brique.h"

#define WALLSIZE 40


class mur {

public:

  mur(Minitel* m, int* l);
  ~mur();

  void draw();
  bool checkCollision(balle& b, int& score);

protected:

  Minitel* m;

  brique* bri[WALLSIZE] = { NULL };

  void removeBrique(int i);
};