/* 
Project: Đo nhiệt độ và độ ẩm không khí bằng DHT11 và gửi dữ liệu về máy tính

Nối dây: dùng 3 dây jumper loại female-female để nối:
- Chân Data của DHT11 vào chân GPIO 4 của ESP32;
- Chân VCC của DHT11 vào chân 3.3V của ESP32;
- Chân GND của DHT11 vào chân GND của ESP32.
*/

#include "DHT.h"

#define DHTPIN 4     // Bạn nối chân Data của DHT11 vào chân GPIO 4 của ESP32
#define DHTTYPE DHT11   // Khai báo loại cảm biến là DHT11

DHT dht(DHTPIN, DHTTYPE); // Khởi tạo một đối tượng dht với cấu hình trên

void setup() {
  Serial.begin(115200); // Mở đường ống giao tiếp Serial với máy tính trên ESP32 ở tốc độ 115200 baud (115200 bit/s)
  Serial.println(F("--- BẮT ĐẦU TEST CẢM BIẾN DHT11 ---")); // bọc chuỗi cần in bằng hàm F() để tiết kiệm RAM cho ESP32
  dht.begin(); // Ra lệnh cho ESP32 kích hoạt, cấp xung điện để "đánh thức" DHT11
}

void loop() {
  delay(2000); // DHT11 cần khoảng 2 giây để cập nhật dữ liệu mới, không nên đọc quá nhanh

  // Đọc độ ẩm (Humidity)
  float h = dht.readHumidity();
  // Đọc nhiệt độ theo độ C (Celsius)
  float t = dht.readTemperature();

  // Kiểm tra xem việc đọc dữ liệu từ cảm biến có bị lỗi không (isnan = is-not-a-number, trả về true nếu giá trị của tham số không là số)
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Lỗi! Không đọc được dữ liệu từ cảm biến DHT11. Kiểm tra lại dây nối!"));
    return; // Thoát khỏi hàm loop hiện tại để chạy lại vòng mới
  }

  // Nếu mọi thứ mượt mà, dùng đường ống Serial đẩy dữ liệu nhị phân đã dịch ngược lên màn hình laptop
  Serial.print(F("Độ ẩm: "));
  Serial.print(h);
  Serial.print(F("%  |  "));
  Serial.print(F("Nhiệt độ: "));
  Serial.print(t);
  Serial.println(F("°C"));
}

// Chương trình trên sẽ đọc dữ liệu từ cảm biến DHT11 và gửi về máy tính qua cổng Serial.
// -> Mở Serial Monitor trong Arduino IDE để xem dữ liệu nhiệt độ và độ ẩm được cập nhật mỗi 2 giây.