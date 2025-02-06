#define LASER_PIN 10

int waitMillis = 4;
double frequency = 1 / (waitMillis / 1000);

byte startCode[24] = {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0};
byte stopCode[25] = {0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0};

int bufferSize = 5150;

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
  for (int i = 0; i < len; i++) {
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
  for (int charIdx = 0; charIdx < len; ++charIdx) {
    for (int bitIdx = 7; bitIdx >= 0; --bitIdx) {
      bytes[counter] = (cInput[charIdx] >> bitIdx) & 1;
      Serial.print(bytes[counter]);
      ++counter;
    }
  }
}

void addParityBits(uint8_t *input, int inputLen, uint8_t *output) {
  // adds a parity bit every byte
  int bitCount = 0;
  int outputIdx = 0;
  for (int i = 0; i < inputLen; ++i) {
    if ((i + 1) % 8 == 0) {
      // 8th bit
      output[outputIdx] = input[i];
      if (input[i] == 1) {
        ++bitCount;
      }
      ++outputIdx;
      output[outputIdx] = bitCount % 2;
      bitCount = 0;
    } else {
      // count the 1s in the byte
      if (input[i] == 1) {
        ++bitCount;
      }
      output[outputIdx] = input[i];
    }
    ++outputIdx;
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(LASER_PIN, OUTPUT);
  pinMode(13, OUTPUT);
}

void loop() {
  char inputBuffer[bufferSize];

  // fetch input from serial buffer
  while(!Serial.available());
  int i = 0;
  
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

  // add parity bits for error detection
  int parLen = (bitLen + (bitLen / 8));
  uint8_t parBinMsg[(int)parLen];
  addParityBits(inputBin, bitLen, parBinMsg);

  // send message through fso
  sendMsg(parBinMsg, parLen);
  Serial.println(" has been send.");
}
