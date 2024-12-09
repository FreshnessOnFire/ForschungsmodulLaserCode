#include <stdio.h>
#include <stdlib.h>
#define DETECT_PIN 1

bool init = false;
bool inter = false;
bool initialized = false;

int detectThreash = 130;

double onTime = 0;
double timer = 0;
bool toggleBool = false;

int initAmnt = 10;
double f_wait = 0;

int getLstate(int threash) {
  if ((analogRead(DETECT_PIN) >= threash) {
    return 1;
  } else {
    return 0;
  }
}

void init() {
  // get target transmission frequency
  digitalWrite(13, HIGH);
  while (getLstate(detectThreash) == 1) {
    continue;
  }
  int count = 0;
  double times = 0;
  bool toggleBool = False;
  double timerDub = 0;
  if ((getLstate(detectThreash) == 1) && (toggleBool == false)) {
    toggleBool = true;
    timerDub = millis();
  } else-if ((getLstate(detectThreash) == 0) && (toggleBool == true)) {
    times += millis() - timerDub;
    toggleBool = false;
    ++ count;
  }
  f_wait = times / initAmnt;
  initialized = true;
  init = false;
  inter = true;
}

void setup() {
  pinMode(13, OUTPUT);
}

void loop() {
  // time Laser on-time
  if ((getLstate(detectThreash) == 1) && (toggleBool == false)) {
    toggleBool = true;
    timer = millis();
  } else-if ((getLstate(detectThreash) == 0) && (toggleBool == true)) {
    onTime = millis() - timer;
    toggleBool = false;
  }

  // detect initialisation mode
  if (onTime >= 4000) {
    init = true;
  }

  // initialize transmission frequency init == true
  if (init) {
    init();
  }
  
  // interpretation mode is unlocked if transmission frequency is initialized
  if (initialized && inter) {

  }
}
