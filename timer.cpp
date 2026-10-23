#include <sys/_types.h>
#include "esp32-hal.h"
#include "timer.h"

unsigned long timer::currentTime;

timer::timer(unsigned long interval) : interval(interval) {
  startTime = 0;
}

void timer::reset() {
  startTime = currentTime;
}

bool timer::isElapsed() {
  return currentTime - startTime >= interval;
}

void timer::update() {
  currentTime = millis();
}