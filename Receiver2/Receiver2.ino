#define DETECT_PIN 1

bool init = false;
bool inter = false;
bool initialized = false;

int detectThreash = 130;

double onTime = 0;
double timer = 0;
bool toggleBool = false;
int elapsedTimeThreash = 100;

double f_wait = 0;
int inputBuffer[256];

int getLstate(int threash) {
  if ((analogRead(DETECT_PIN) >= threash) {
    return 1;
  } else {
    return 0;
  }
}

void setup() {
  pinMode(13, OUTPUT);
}

void loop() {
  int onCounter = 0;
  double timeElapsed = 0;
  while onCounter != 10 {
    if ((getLstate(detectThreash) == 1) && (toggleBool == false)) {
      toggleBool = true;
      timer = millis();
      timeElapsed = millis();
    } else if ((getLstate(detectThreash) == 0) && (toggleBool == true)) {
      onTime += (millis() - timer);
      toggleBool = false;
      ++ onCounter;
      timeElapsed = 0;
    }
    if (timeElapsed > elapsedTimeThreash) {
      onCounter = 0;
    }
  }
  onTime = onTime / 10;
  delay(onTime * 5);



}