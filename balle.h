#pragma once
#include <Minitel1B_Hard.h>
#include "raquette.h"


class balle {

public:

  balle(Minitel* m, raquette* r);
  ~balle();

  void bounceX();
  void bounceY();
  void update(raquette* r);
  void launch();
  void draw();

  int getPos_Y() const;
  int getPos_X() const;
  int getSpeed_Y() const;

  bool isOut();

protected:

  Minitel* m;
  raquette* r;

  int pos_X, pos_Y, lastPos_X, lastPos_Y, offset_X;
  int speed_X, speed_Y;
  bool isLaunch;
  bool redraw;

  bool out;   // Balle perdue

  void erase();
};