#include <stdio.h>
#include <stdlib.h>
#define LASER_PIN 10
#define MODESWITCH_PIN 5

double frequency = 5;  //[Hz]
double f_wait = (1 / frequ) * 1000; //on-Time for transmission [ms] 
int initAmnt = 10;

void setup() {
  pinMode(LASER_PIN, OUTPUT);
}

void laser(int on) {
  digitalWrite(LASER_PIN, on);
}

void initiate(double wait, double t_wait) {
  laser(1);
  delay(t_wait);
  laser(0);
  delay(500);
  int i;
  for (i = 0; i < initAmnt; i++) {
    laser(1);
    delay(wait);
    laser(0);
  }
}

void sendString(String sInput) {
  // typeconversion String -> char*
  int inputLen = sInput.length();
  char *cInput = (char*)malloc(inputLen * sizeof(char));
  sInput.toCharArray(cInput, inputLen);

  // typeconversion char* -> byte*
  byte bytes[sizeof(cInput)];
  for (byte i = 0; i < sizeof(cInput); i++) {
    char currChar = cInput[i];
    bytes[i] = (byte)currChar;
  }
  free(cInput);

  // OOK
  int bits[sizeof(bytes) * 8]
  for (int i = 0; i < sizeof(bytes) * 8; i++) {
    laser(bitRead(bytes[floor(i / 8)], i));
    delay(wait);
  }
}

void loop() {
  if (digitalRead(MODESWITCH_PIN) == HIGH) {
    initiate(f_wait, 5000);
  } else {
    if (Serial.available()) {
      String userInput = Serial.readStringUntil('\n');
      sendString(userInput);
    }
  }
}
