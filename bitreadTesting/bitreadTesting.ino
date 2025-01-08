#include <bitset>
char inputBuffer[512];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);  
}

void toBinary(char *cInput, int len, unsigned char *bytes) {
  int counter = 0;
  for (int i = 0; i < len; ++i) {
    for (int j = 7; j >= 0; --j) {
      bytes[counter] = (cInput[i] >> j) & 1;
      Serial.print(bytes[counter]);
      ++counter;
    }
  }
}

void fromBinary(unsigned char *input, int idx, char *msg) {
  // turn bits into characters
  for (int k = 0; k < idx / 8; ++k) {
    msg[k] = '\0';
  }
  int charIdx = 0;
  for (int bitIdx = 0; bitIdx < idx; bitIdx++) {
    msg[charIdx] |= (input[bitIdx] << (7 - (bitIdx % 8)));
    if ((bitIdx % 8) == 7) {
      for (int k = 7; k >= 0; --k) {
        Serial.print(bitRead(msg[charIdx], k));
      }
      charIdx++;
    }
  }
  msg[charIdx] = '\0';
}

void loop() {
  // put your main code here, to run repeatedly:

  while(!Serial.available());
  int i = 0;
  while((inputBuffer[i] = Serial.read()) != '\n') {
    ++i;
  }
  inputBuffer[++i] = '\0';
  char userInput[i];
  for (int j = 0; j <= i; ++j) {
    userInput[j] = inputBuffer[j];
  }
  Serial.print("Send: '");
  Serial.print(userInput);
  Serial.print(" | ");
  unsigned char binaryMsg[(i - 1) * 8];
  toBinary(userInput, i - 1, binaryMsg);
  Serial.println("'");
  /*for (int j = 0; j < (i - 1) * 8; ++j){
    Serial.println(binaryMsg[j]);
  }*/
  Serial.println("");
  Serial.print("Decoded: '");
  char msgFromBin[i];
  fromBinary(binaryMsg, (i - 1) * 8, msgFromBin);
  Serial.print(" | ");
  Serial.print(msgFromBin);
  Serial.println("'");

}
