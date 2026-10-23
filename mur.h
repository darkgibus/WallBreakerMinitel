#pragma once
#include "balle.h"
#include "brique.h"

#define WALLSIZE 40


class mur {

public:

  mur(Minitel* m, int* l);
  ~mur();

  void draw();
  void checkCollision(balle& b, int& score);

  int getLife() const;
  
protected:

  Minitel* m;

  brique* bri[WALLSIZE] = { NULL };
  int life;

  void removeBrique(int i);
};