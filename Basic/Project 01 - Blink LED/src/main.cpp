void setup() {
  pinMode(2, OUTPUT); // khai báo chân 2 sẽ phát tín hiệu (chân 2 nối với LED xanh dương - xem cụ thể hơn trong sơ đồ chân cắm của mạch)
}

void loop() {
  digitalWrite(2, HIGH); // bật đèn LED
  delay(500); // đợi 500ms
  digitalWrite(2, LOW); // tắt đèn LED
  delay(500);
}