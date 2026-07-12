#include <Arduino.h>

void setup()
{
  pinMode(2, OUTPUT); // khai báo chân 2 sẽ phát tín hiệu (chân 2 nối với LED xanh dương - xem cụ thể hơn trong sơ đồ chân cắm của mạch)
}

void blink(unsigned int interval)
{
  digitalWrite(2, HIGH);
  delay(interval);
  digitalWrite(2, LOW);
  delay(interval);
}

void loop()
{
  blink(1000);
}