#define DETECT_PIN 1

bool inter = false;
bool initialized = false;

int detectThreash = 0;
int erroneosFilter = 3000;

int onTime = 0;

double timer = 0;
bool toggleBool = false;
int elapsedTimeThreash = 2000000;
double f_wait = 0;

const int bufferSize = 1024;
uint8_t inputBuffer[bufferSize];

int stopSize = 25;
byte stopCode[25] = {0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0};

int getLstate(int threash) {
  // returns if photodiode detects light above a certain threashold
  if (analogRead(DETECT_PIN) >= threash) {
    return 1;
  } else {
    return 0;
  }
}

int getLnAmpState(int threash, int *amp) {
  // returns if photodiode detects light above a certain threashold and its amplitude
  if ((*amp = analogRead(DETECT_PIN)) >= threash) {
    return 1;
  } else {
    return 0;
  }
}

int Lamp() {
  return analogRead(DETECT_PIN);
}

void decode(int idx, char *msg) {
  // turn bits into characters
  for (int k = 0; k < idx / 8; ++k) {
    msg[k] = '\0';
  }
  int charIdx = 0;
  for (int bitIdx = 0; bitIdx < idx; bitIdx++) {
    msg[charIdx] |= (inputBuffer[bitIdx] << (7 - (bitIdx % 8)));
    if ((bitIdx % 8) == 7) {
      for (int k = 7; k >= 0; --k) {
        Serial.print(bitRead(msg[charIdx], k));
      }
      charIdx++;
    }
  }
  msg[charIdx] = '\0';
}

void debug(uint8_t* buff) {
  Serial.println("Bits in inputBuffer: ");
  for (int i = 0; i < bufferSize; ++i) {
    Serial.print((int)buff[i]);
  }
  Serial.println("");
}

int findMatchIndex(uint8_t* inptArr) {
  // find index where input array matches the predefined stopcode 
  for (int i = 0; i <= bufferSize - stopSize; i++) {
    bool match = true;
    for (int j = 0; j < stopSize; j++) {
      if (inptArr[i + j] != stopCode[j]) {
        match = false;
        break;
      }
    }
    if (match) {
      return i;  
    }
  }
  // return -1 when unmatched
  return -1;
}

void setup() {
  pinMode(13, OUTPUT);
  Serial.begin(9600);

  // adapt toggle threashold
  delay(1000);
  int amp;
  int maxAmp = 0;
  for (int i = 0; i < 100; i++) {
    amp = analogRead(DETECT_PIN);
    if (amp > maxAmp) {
      maxAmp = amp;
    }
    delay(10);
  }
  detectThreash = maxAmp + 50;
  Serial.print("Adaptive threashold initiated: ");
  Serial.println(detectThreash);
  Serial.print("Current ambient amlitude: ");
  Serial.println(analogRead(DETECT_PIN));
  Serial.println("-- Switching to listening mode --");
}

void loop() {
  // detect start code of possible data transmission
  int onCounter = 0;
  double timeElapsed = 0;
  int timing[10];

  // debug
  int i = 0;
  while (onCounter != 10) {
    if ((getLstate(detectThreash) == 1) && (toggleBool == false)) {
      // rising edge detected
      toggleBool = true;
      timer = micros();

      //debug
      digitalWrite(9, (i + 1) % 2);
      ++i;
    } else if ((getLstate(detectThreash) == 0) && ((micros() - timer) > erroneosFilter) && (toggleBool == true)) {
      // falling edge detected
      timing[onCounter] = micros() - timer;
      onTime = onTime + (micros() - timer);
      toggleBool = false;
      ++ onCounter;
      timeElapsed = micros();

      //debug
      digitalWrite(9, (i + 1) % 2);
      ++i;
    }
    if ((toggleBool == false) && ((micros() - timeElapsed) > elapsedTimeThreash)) {
      // resetting counter after too much elapsed time
      onCounter = 0;
    }
  }

  // receive data package
  digitalWrite(13, HIGH);

  onTime = ceil(onTime / 10000) + 1;
  delay(onTime * 6);

  for (int i = 0; i < bufferSize; i++) {
    digitalWrite(9, (i + 1) % 2);
    inputBuffer[i] = getLstate(detectThreash);
    delay(onTime);
  }
  digitalWrite(13, LOW);
  digitalWrite(9, 0);

  // print metadata
  Serial.print("Average clock tick [ms]: ");
  Serial.println(onTime);
  Serial.println("Individual clock ticks [micro seconds]: ");
  for (int i = 0; i < 10; ++i){
    Serial.print("Tick ");
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.println(timing[i]);
  }


  // read binary message
  int indx = findMatchIndex(inputBuffer);
  if (indx == -1) {
    Serial.println("Error: Buffer overflown or package lost");
  } else {
    char message[indx];
    Serial.print("Message received: ");
    decode(indx, message);
    Serial.println(" | ");
    Serial.println(message);
    Serial.print("Package size: ");
    Serial.print(indx / 8);
    Serial.println(" Byte");
  }
}