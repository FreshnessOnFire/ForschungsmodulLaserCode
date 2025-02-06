#define DETECT_PIN 1

bool inter = false;
bool initialized = false;

int detectThreash = 130;

int onTime = 0;

double timer = 0;
bool toggleBool = false;
int elapsedTimeThreash = 2000000;
double f_wait = 0;

const int bufferSize = 32850;
uint8_t inputBuffer[bufferSize];

int stopSize = 25;
byte stopCode[25];

int getLstate(int threash) {
  if ((analogRead(DETECT_PIN) >= threash)) {
    return 1;
  } else {
    return 0;
  }
}

void decode(int idx, char *msg, uint8_t *buf) {
  // turn bits into characters
  for (int k = 0; k < idx / 8; ++k) {
    msg[k] = '\0';
  }
  int charIdx = 0;
  for (int bitIdx = 0; bitIdx < idx; bitIdx++) {
    msg[charIdx] |= (buf[bitIdx] << (7 - (bitIdx % 8)));
    if ((bitIdx % 8) == 7) {
      for (int k = 7; k >= 0; --k) {
        Serial.print(bitRead(msg[charIdx], k));
      }
      charIdx++;
    }
  }
  msg[charIdx] = '\0';
  Serial.println("");
}

void debug(uint8_t* arr, int len) {
  //Serial.println("Bits in inputBuffer: ");
  for (int i = 0; i < len; ++i) {
    Serial.print((int)arr[i]);
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

void deBPPMify(uint8_t* binMsg) {
  // turns BPP modulated message into decodeable bitstring
  for (int i = 0; i < bufferSize / 2; i++) {
    binMsg[i] = inputBuffer[i * 2];
  }
}

void setup() {
  pinMode(13, OUTPUT);
  Serial.begin(9600);

  // 0 0 0 0 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1
  stopCode[0]  = 0;
  stopCode[1]  = 0;
  stopCode[2]  = 0;
  stopCode[3]  = 0;
  stopCode[4]  = 0;
  stopCode[5]  = 1;
  stopCode[6]  = 0;
  stopCode[7]  = 1;
  stopCode[8]  = 0;
  stopCode[9]  = 1;
  stopCode[10] = 0;
  stopCode[11] = 1;
  stopCode[12] = 0;
  stopCode[13] = 1;
  stopCode[14] = 0;
  stopCode[15] = 1;
  stopCode[16] = 0;
  stopCode[17] = 1;
  stopCode[18] = 0;
  stopCode[19] = 1;
  stopCode[20] = 0;
  stopCode[21] = 1;
  stopCode[22] = 0;
  stopCode[23] = 1;
  stopCode[24] = 0;

}

void loop() {
  // detect start code of possible data transmission
  int onCounter = 0;
  double timeElapsed = 0;
  int timing[10];
  while (onCounter != 10) {
    if ((getLstate(detectThreash) == 1) && (toggleBool == false)) {
      // rising edge detected
      toggleBool = true;
      timer = micros();
    } else if ((getLstate(detectThreash) == 0) && (toggleBool == true)) {
      // falling edge detected
      timing[onCounter] = micros() - timer;
      onTime = onTime + (micros() - timer);
      toggleBool = false;
      ++ onCounter;
      timeElapsed = micros();
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
    digitalWrite(9, i % 2);
    inputBuffer[i] = getLstate(detectThreash);
    delay(onTime);
  }
  digitalWrite(9, LOW);
  digitalWrite(13, LOW);

  Serial.print("Average clock tick [milli seconds]: ");
  Serial.println(onTime);
  Serial.println("Individual clock ticks [micro seconds]: ");
  for (int i = 0; i < 10; ++i){
    Serial.print("Tick ");
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.println(timing[i]);
  }

  // interprete received PPM code as binary
  uint8_t binMsg[bufferSize / 2];
  deBPPMify(binMsg);

  // read binary message
  int indx = findMatchIndex(binMsg);
  if (indx == -1) {
    Serial.println("Error: Buffer overflown or package lost");
    debug(inputBuffer, bufferSize);
    debug(binMsg, bufferSize / 2);
  } else {
    char message[indx];
    Serial.print("Message received: ");
    decode(indx, message, binMsg);
    Serial.println(message);
    Serial.print("Package size: ");
    Serial.print(indx / 8);
    Serial.println(" Byte");
  }
}