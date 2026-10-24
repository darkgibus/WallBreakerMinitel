#pragma once
#include <Minitel1B_Hard.h>


class raquette {

public:

  raquette(Minitel* m);
  ~raquette();

  void moveLeft();
  void moveRight();

  void draw();

  int getPos_Y() const;
  int getPos_X() const;
  int getSize() const;

  bool isInside(int x, int y);
  
protected:

  Minitel* m;

  int pos_X, pos_Y, lastPos_X;
  static const int max_X = 41;
  static const int size = 6;
  bool redraw;

  void erase();

  char graph[size] = { 27, 51, 51, 51, 51, 39 };
};