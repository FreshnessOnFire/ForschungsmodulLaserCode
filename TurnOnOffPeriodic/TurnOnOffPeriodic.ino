int t_dl = 5; 

void setup() {
  // put your setup code here, to run once:
  pinMode(10, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(10, HIGH);
  delay(t_dl);
  digitalWrite(10, LOW);
  delay(t_dl);
}
