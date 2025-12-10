int buzzer = 5;

void setup() {
  pinMode(buzzer, OUTPUT);
}

void loop() {
  tone(buzzer, 1000); // 蜂鸣器发出 1kHz 音调
  delay(1000);
  noTone(buzzer);
  delay(500);
}
