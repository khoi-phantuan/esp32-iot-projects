/*
Project 3: 
Kết nối Wi-Fi và đóng vai trò của một Web Server - nhận và xử lý yêu cầu từ trình duyệt web của máy trạm, và trả về phản hồi tương ứng
*/

#include <WiFi.h> // thư viện WiFi của package ESP32 -> include để sử dụng tính năng WiFi của bo mạch

const char* ssid = "KnightP";
const char* password = "KhoiPhan1104";
// biến lưu tên và mật khẩu của mạng mà ESP32 sẽ xin kết nối vào - kiểu char* (lưu trong bộ nhớ Flash), thay vì String (tốn RAM)

WiFiServer server(80); // socket lắng nghe 'server' hoạt động ở cổng 80 trên ESP32, nhận và quản lý các yêu cầu được truyền tới qua sóng WiFi

void setup() 
{
  Serial.begin(115200);
  delay(10); // thời gian chờ để chip ESP32 chuẩn bị cho công tác truyền thông dữ liệu Serial với máy tính

  Serial.println();
  Serial.print("Dang ket noi vao mang: ");
  Serial.println(ssid);

  // đối tượng WiFi được thư viện WiFi.h khởi tạo sẵn khi ta include, đại diện cho Card mạng WiFi vật lý / bộ tính năng WiFi của bo mạch ESP32 
  WiFi.begin(ssid, password); // card mạng WiFi gửi dữ liệu chứa ssid và password ra không gian -> router của mạng hứng được

  while (WiFi.status() != WL_CONNECTED) // hàm .status() trả về trạng thái kết nối WiFi của ESP32 hiện tại
  {
    delay(500);
    Serial.print("."); // nếu chưa được vào mạng, liên tục in ra các dấu chấm mỗi 500ms - kí hiệu loading...
  }

  Serial.println("");
  Serial.println("Wi-Fi da ket noi thanh cong!");  
  Serial.print("Dia chi IP cua ESP32 la: ");
  Serial.println(WiFi.localIP()); // hàm .localIP() trả về địa chỉ IP mà router cấp cho ESP32 này

  server.begin(); // socket lắng nghe ở port 80 bắt đầu hoạt động
  Serial.println("Web Server da khoi dong xong, dang mo cua Port 80...");
}

void loop() 
{
  WiFiClient client = server.available(); 
  // hàm .available() của socket lắng nghe
  // -> trả về thông tin kết nối của máy khách đầu tiên trong hàng đợi máy khách muốn kết nối đến server ESP32 này, hoặc "rỗng"
  // khai báo socket kết nối 'client' phía ESP32; socket này sẽ đóng gói và truyền đi dữ liệu truyền thông giữa 2 máy bằng sóng WiFi
  // -> gán giá trị trả về bên trên vào socket kết nối này để làm "đường ống" giao tiếp với máy khách đó

  if (client) // nếu socket kết nối này khác rỗng (đang thực sự phục vụ một máy khách)...
  {
    Serial.println("Co Client moi go cua!"); 
    
    while (client.connected()) // trong khi trạng thái kết nối của socket kết nối vẫn đang là true...
    {
      if (client.available()) 
      // hàm .available() của socket kết nối trả về số byte dữ liệu đang nằm trong buffer mà máy trạm gửi qua (ứng với các kí tự trong HTTP Request)
      // nếu 0 byte dữ liệu trong buffer -> hàm trả về false -> máy khách chưa gửi gì -> bỏ qua khối if này
      {             
        char c = client.read(); // đọc ký tự đầu tiên trong buffer
        Serial.print(c); // gửi ký tự này lên máy tính để hiển thị trên Serial Monitor
        
        if (c == '\n') 
        // nếu ký tự này là '\n' (quy ước là trong HTTP Request, giữa header và body bắt buộc phải có 1 dòng chỉ có ký tự '\n')
        // tức là nếu đã đọc xong header và gặp ký tự '\n'...
        {
          // gửi các chuỗi "hệ thống" về trình duyệt để cung cấp thông tin cho trình duyệt
          client.println("HTTP/1.1 200 OK"); // trạng thái kết nối với máy trạm
          client.println("Content-Type: text/plain"); // loại văn bản chuẩn bị hiển thị lên trình duyệt
          client.println(); // kết thúc việc gửi chuỗi "hệ thống" bằng một cái xuống dòng
          
          client.println("Chao Khoi, he thong da chay vao Web Server!"); // gửi các chuỗi "nội dung" để trình duyệt hiển thị lên
          
          break; // dừng vòng lặp, không cần đọc tiếp body của HTTP Request nữa
        }
      }
    }
    
    client.stop(); // dừng hoạt động socket kết nối đang phục vụ máy khách
    Serial.println();
    Serial.println("Client da ngat ket noi.");
  }
}