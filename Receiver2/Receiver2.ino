#define DETECT_PIN 1

bool inter = false;
bool initialized = false;

int detectThreash = 130;

double onTime = 0;
double timer = 0;
bool toggleBool = false;
int elapsedTimeThreash = 100;
double f_wait = 0;

int bufferSize = 512;
byte inputBuffer[512];

int stopSize = 25;
byte stopCode[25];

int getLstate(int threash) {
  if ((analogRead(DETECT_PIN) >= threash)) {
    return 1;
  } else {
    return 0;
  }
}

char *decode(int idx) {
  // interprete message without stop code
  byte inputMsg[idx];
  for (int i = 0; i <= idx; i++) {
    inputMsg[i] = inputBuffer[i];
  }

  // turn bits into characters
  int charlen = sizeof(inputMsg) / 8;
  char msg[charlen];
  int i = 0;
  for (int j = 0; j < charlen; j++) {
    msg[i] | inputBuffer[i];
    msg[i] << 1;
    if ((j != 0) && (j % 8 == 0)) {
      ++i;
    }
  }
  return msg;
}

int findMatchIndex(byte* inptArr) {
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
  while (onCounter != 10) {
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

  // receive data package
  digitalWrite(13, HIGH);
  onTime = onTime / 10;
  delay(onTime * 4);
  int i = 0;
  while (i < bufferSize) {
    inputBuffer[i] = getLstate(detectThreash);
    delay(onTime);
  }
  digitalWrite(13, LOW);

  // read binary message
  int indx = findMatchIndex(inputBuffer);
  if (indx == -1) {
    Serial.println("Error: Buffer overflown or package lost");
  }
  char *message = decode(indx);
  Serial.print("Message received: ");
  Serial.println(message);
}