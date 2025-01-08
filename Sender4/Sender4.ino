#define LASER_PIN 10

int waitMillis = 100;
double frequency = 1 / (waitMillis / 1000);

byte startCode[24];
byte stopCode[25];

void laser(int state) {
  digitalWrite(LASER_PIN, state);
}

void sendMsg(uint8_t *input, int len) {
  // OOK
  digitalWrite(13, HIGH);
  // sent startcode
  for (int i = 0; i < 24; i++) {
    laser(startCode[i]);
    delay(waitMillis);
  }

  // send data package
  for (int i = 0; i < len; ++i) {
    laser(input[i]);
    delay(waitMillis);
  }
  
  // sent stopcode
  for (int i = 0; i < 25; i++) {
    laser(stopCode[i]);
    delay(waitMillis);
  }
  digitalWrite(13, LOW);
}

void encode(char *cInput, int len, uint8_t *bytes) {
  // encodes sequence of chars in sequence of bits
  int counter = 0;
  for (int i = 0; i < len; ++i) {
    for (int j = 7; j >= 0; --j) {
      bytes[counter] = (cInput[i] >> j) & 1;
      Serial.print(bytes[counter]);
      ++counter;
    }
  }
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
  char inputBuffer[512];

  // fetch input from serial buffer
  while(!Serial.available());
  int i = 0;
  while((inputBuffer[i] = Serial.read()) != '\n') {
    ++i;
  }
  inputBuffer[i] = '\0';
  int bitLen = (i - 1) * 8;

  // extract message from inputBuffer
  char userInput[i];
  for (int j = 0; j <= i; ++j) {
    userInput[j] = inputBuffer[j];
  }

  // encode message to binary
  uint8_t inputBin[bitLen];
  Serial.print("Message: '");
  encode(userInput, i - 1, inputBin);
  Serial.print(" | ");

  // send message through fso
  Serial.print(userInput);
  sendMsg(inputBin, bitLen);
  Serial.println(" has been send.");
}
