#include <Arduino.h> // thư viện của Arduino framework

const int ledPin = 2; // biến lưu số hiệu của chân điều khiển đèn LED trên mạch (2)
int ledState = LOW;   // biến lưu trạng thái của chân điều khiển LED hiện tại
                      // (LOW là 0 - mức điện áp 0V; HIGH là 1 - mức điện áp nguồn (3.3V đối với ESP32 này))

unsigned long previousMillis = 0; // biến lưu mốc thời gian gần nhất LED đổi trạng thái
const long interval = 1000;       // biến lưu thời gian duy trì trạng thái

void setup()
{
  pinMode(ledPin, OUTPUT); // khai báo chân 2 là chân xuất điện áp
}

void loop()
{
  unsigned long currentMillis = millis(); // biến lưu thời gian đã trôi qua kể từ khi chương trình chạy ở lần lặp này

  if (currentMillis - previousMillis >= interval) // nếu khoảng cách từ lần gần nhất LED đổi trạng thái tới hiện tại đã vượt quá thời gian duy trì trạng thái...
                                                  // -> đã đến lúc đảo trạng thái LED
  {
    previousMillis = currentMillis; // gán mốc thời gian hiện tại vào biến lưu mốc thời gian gần nhất LED đổi trạng thái để phục vụ cho lần đo khoảng cách thời gian sau

    // đảo trạng thái LED
    if (ledState == LOW)
    {
      ledState = HIGH;
    }
    else
    {
      ledState = LOW;
    }

    digitalWrite(ledPin, ledState); //  ra tín hiệu áp dụng xuống mạch
  }
}
