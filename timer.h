#pragma once


class timer {

public:

  timer(unsigned long interval);

  void reset();
  bool isElapsed();

  static void update();

protected:

  const unsigned long interval;
  unsigned long startTime;
  static unsigned long currentTime;

};