char inputBuffer[512];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);  
}

byte *toBinary(char *cInput, int len) {
  //int inputLen = sizeof(cInput);
  byte bytes[len * 8];
  int counter = 0;
  for (int i = 0; i < len; ++i) {
    for (int j = 7; j >= 0; --j) {
      Serial.print(bitRead(cInput[i], j));
      bytes[counter] = bitRead(cInput[i], j);
      ++counter;
    }
  }
  return bytes;
}

char *fromBinary(byte *input, int idx) {
  unsigned char inputMsg[idx];
  for (int i = 0; i < idx; i++) {
    inputMsg[i] = input[i];
  }

  // turn bits into characters
  int charlen = sizeof(inputMsg) / 8;
  char msg[charlen];
  for (int k = 0; k < charlen; ++k) {
    msg[k] = '\0';
  }
  int i = 0;
  for (int j = 0; j < idx; j++) {
    //msg[i] = msg[i] << 1;
    bitWrite(msg[i], j % 8, inputMsg[j]);
    //msg[i] & inputMsg[j];
    if ((j != 0) && (j % 8 == 0)) {
      //Serial.print(msg[i]);
      for (int k = 7; k >= 0; --k) {
        Serial.print(bitRead(msg[i], k));
      }
      ++i;
    }
    msg[++i] = '\0';
  }
  return msg;
}

void loop() {
  // put your main code here, to run repeatedly:

  while(!Serial.available());
  int i = 0;
  while((inputBuffer[i] = Serial.read()) != '\n') {
    ++i;
  }
  inputBuffer[i] = '\0';
  char userInput[i];
  for (int j = 0; j <= i; ++j) {
    userInput[j] = inputBuffer[j];
  }
  Serial.print("Send: '");
  Serial.print(userInput);
  Serial.print(" | ");
  byte *binaryMsg = toBinary(userInput, i);
  Serial.println("'");
  Serial.print("Decoded: '");
  Serial.print(fromBinary(binaryMsg, i * 8));
  Serial.println("'");

}
