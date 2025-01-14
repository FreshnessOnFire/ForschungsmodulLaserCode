#define LASER_PIN 10

int waitMillis = 4;
double frequency = 1 / (waitMillis / 1000);

byte startCode[24];
byte stopCode[25];

int bufferSize = 5150;
//int lState = 0;

void laser(int state) {
  digitalWrite(LASER_PIN, state);
  //lState = state;
}

void sendMsg(uint8_t *input, int len) {
  digitalWrite(13, HIGH);
  // sent startcode
  for (int i = 0; i < 24; i++) {
    laser(startCode[i]);
    delay(waitMillis);
  }

  // send data package
  for (int i = 0; i < len; ++i) {
    switch (input[i]) {
      // binary PPM
      case 0:
        // pulse in second time slot represents 0
        laser(0);
        delay(waitMillis);
        laser(1);
        break;
      case 1:
        // pulse in first time slot represents 1
        laser(1);
        delay(waitMillis);
        laser(0);
        break;
      default:
        break;
    }
    delay(waitMillis);
  }
  
  // sent stopcode
  for (int i = 0; i < 25; i++) {
    switch (stopCode[i]) {
      // binary PPM
      case 0:
        // pulse in second time slot represents 0
        laser(0);
        delay(waitMillis);
        laser(1);
        break;
      case 1:
        // pulse in first time slot represents 1
        laser(1);
        delay(waitMillis);
        laser(0);
        break;
      default:
        break;
    }
    delay(waitMillis);
  }
  laser(0);
  digitalWrite(13, LOW);
}

void encode(char *cInput, int len, uint8_t *bytes) {
  // encodes sequence of chars in sequence of bits
  int counter = 0;
  for (int charIdx = 0; charIdx < len; ++charIdx) {
    for (int bitIdx = 7; bitIdx >= 0; --bitIdx) {
      bytes[counter] = (cInput[charIdx] >> bitIdx) & 1;
      Serial.print(bytes[counter]);
      ++counter;
    }
  }
}

void debug(uint8_t *bin, int len) {
  for (int i = 0; i < len; i++) {
    switch(bin[i]) {
      case 0:
        Serial.print("0");
        Serial.print("1");
        break;
      case 1:
        Serial.print("1");
        Serial.print("0");
        break;
    }
  }
  Serial.println("");
}

void setup() {
  Serial.begin(9600);
  pinMode(LASER_PIN, OUTPUT);
  pinMode(13, OUTPUT);

  startCode[0]  = 1;
  startCode[1]  = 0;
  startCode[2]  = 1;
  startCode[3]  = 0;
  startCode[4]  = 1;
  startCode[5]  = 0;
  startCode[6]  = 1;
  startCode[7]  = 0;
  startCode[8]  = 1;
  startCode[9]  = 0;
  startCode[10] = 1;
  startCode[11] = 0;
  startCode[12] = 1;
  startCode[13] = 0;
  startCode[14] = 1;
  startCode[15] = 0;
  startCode[16] = 1;
  startCode[17] = 0;
  startCode[18] = 1;
  startCode[19] = 0;
  startCode[20] = 0;
  startCode[21] = 0;
  startCode[22] = 0;
  startCode[23] = 0;

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
  char inputBuffer[bufferSize];

  // fetch input from serial buffer
  while(!Serial.available());
  int i = 0;
  /*while((inputBuffer[i] = Serial.read()) != '\n') {
    ++i;
  }*/
  char temp;
  while((temp = Serial.read()) != '\n') {
    inputBuffer[i] = temp;
    ++i;
  }
  inputBuffer[i] = '\0';
  int bitLen = i * 8;

  // extract message from inputBuffer
  char userInput[i];
  for (int j = 0; j <= i; ++j) {
    userInput[j] = inputBuffer[j];
  }

  // encode message to binary
  uint8_t inputBin[bitLen];
  Serial.print("Message: '");
  Serial.print(userInput);
  Serial.print(" | ");
  encode(userInput, i, inputBin);
  Serial.print("'");

  // send message through fso
  sendMsg(inputBin, bitLen);
  Serial.print(" has been send.");
  Serial.print(" (");
  Serial.print(i);
  Serial.println(" Byte in size)");
  debug(inputBin, bitLen);
}
