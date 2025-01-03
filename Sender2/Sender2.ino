#define LASER_PIN 10

int waitMillis = 10;
double frequency = 1 / (waitMillis / 1000);

byte startCode[24];
byte stopCode[25];

void laser(int state) {
  digitalWrite(LASER_PIN, state);
}

void sendMsg(String sInput) {
  // typeconversion String -> char*
  int inputLen = sInput.length();
  char *cInput = (char*)malloc(inputLen * sizeof(char));
  sInput.toCharArray(cInput, inputLen);

  // OOK
  digitalWrite(13, HIGH);
  // sent startcode
  for (int i = 0; i < 24; i++) {
    laser(startCode[i]);
    delay(waitMillis);
  }
  // send data package
  int j = 0;
  for (int i = 0; i < sizeof(cInput) * 8; i++) {
    j = floor(i / 8);
    laser(bitRead(cInput[j], i % 8));
    delay(waitMillis);
  }
  // sent stopcode
  for (int i = 0; i < 25; i++) {
    laser(stopCode[i]);
    delay(waitMillis);
  }
  digitalWrite(13, LOW);
  free(cInput);
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
  if (Serial.available()) {
    String userInput = Serial.readStringUntil('\n');
    sendMsg(userInput);
    Serial.print("Message: '");
    Serial.print(userInput);
    Serial.println("' has been send.");
  }
}
