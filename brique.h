#pragma once
#include <Minitel1B_Hard.h>


class brique {

public:

  brique(Minitel* m, int X, int Y);

  virtual void draw();
  void erase();
  int hit();

  int getPos_Y() const;
  int getPos_X() const;
  int getSize_X() const;
  int getLife() const;
  bool isRedraw() const;

  bool isInside(int x, int y);

protected:

  Minitel* m;

  bool redraw;

  int pos_X, pos_Y;

  int life;
};

class brique_incassable : public brique {

public:

  brique_incassable(Minitel* m, int X, int Y);

  void draw() override;

};