#define DETECT_PIN 1

int detectThreash = 130;
bool toggleBool = false;
double timer = 0;
double onTime = 0;

void setup() {
  // put your setup code here, to run once:

}

int getLstate(int threash) {
  if (analogRead(DETECT_PIN) >= threash) {
    return 1;
  } else {
    return 0;
  }
}

void loop() {
  if ((getLstate(detectThreash) == 1) && (toggleBool == false)) {
    toggleBool = true;
    timer = millis();
  } else if ((getLstate(detectThreash) == 0) && (toggleBool == true)) {
    onTime = millis() - timer;
    toggleBool = false;
  }
  Serial.println(onTime);
}
