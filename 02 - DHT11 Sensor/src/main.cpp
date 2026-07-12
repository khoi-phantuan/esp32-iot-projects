#include "DHT.h"

#define DHTPIN 4     // Khai báo chân sẽ nhận dữ liệu từ chân 'out' của DHT11 là chân 4 (D4) trên ESP32
#define DHTTYPE DHT11   // Khai báo loại cảm biến là DHT11

DHT dht(DHTPIN, DHTTYPE); // Khởi tạo đối tượng 'dht' kiểu DHT với cấu hình trên

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
  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  |  "));
  Serial.print(F("Temperature: "));
  Serial.print(t);
  Serial.println(F("°C"));
}