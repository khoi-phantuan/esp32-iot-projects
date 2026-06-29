#include <Arduino.h>
#include <WiFi.h> // thư viện WiFi của package ESP32 -> include để sử dụng bộ tính năng WiFi của bo mạch
#include <LittleFS.h> // điều khiển hệ thống file LittleFS

const char* ssid = "khangkhoi";
const char* password = "1007@htkk";
// biến lưu tên và mật khẩu của mạng mà ESP32 sẽ xin kết nối vào - kiểu char* (lưu trong bộ nhớ Flash), thay vì String (tốn RAM)

WiFiServer server(80); // khởi tạo listening socket 'server' hoạt động ở cổng 80 trên ESP32, nhận và quản lý các yêu cầu được truyền tới qua sóng WiFi

void setup() 
{
  Serial.begin(115200);
  delay(10); // thời gian chờ để chip ESP32 chuẩn bị cho công tác truyền thông dữ liệu Serial với máy tính

  // --- Kích hoạt (mount) thư viện LittleFS để nó nhận biết và quản lý partition của nó trong bộ nhớ Flash ---
  if (!LittleFS.begin(true)) // Tham số "true" nghĩa là: Nếu phân vùng Flash chưa được định dạng theo chuẩn LittleFS, thì hãy tự động định dạng nó luôn ở lần chạy đầu tiên này.
  { 
    Serial.println("Error occurred when mounting LittleFS!");
    return;
  }
  Serial.println("Successfully mounted LittleFS!");

  Serial.println();
  Serial.print("Connecting to the WiFi: ");
  Serial.print(ssid);

  // đối tượng WiFi được thư viện WiFi.h khởi tạo sẵn khi ta include, đại diện cho Card mạng WiFi vật lý / bộ tính năng WiFi của bo mạch ESP32 
  WiFi.begin(ssid, password); // card mạng WiFi gửi tín hiệu chứa dữ liệu ssid và password ra không gian -> router của mạng hứng được

  while (WiFi.status() != WL_CONNECTED) // hàm .status() trả về trạng thái kết nối WiFi của ESP32 hiện tại
  {
    delay(500);
    Serial.print("."); // nếu chưa được vào mạng, liên tục in ra các dấu chấm mỗi 500ms - kí hiệu loading...
  }

  Serial.println("");
  Serial.println("Wi-Fi successfully connected!");  
  Serial.print("The IP Address of the ESP32 is: ");
  Serial.println(WiFi.localIP()); // hàm .localIP() trả về địa chỉ IP mà router cấp cho ESP32 này

  server.begin(); // listening socket ở port 80 bắt đầu hoạt động
  Serial.println("Web Server finishes starting, now opening the Port 80...");
}

void loop() 
{
  WiFiClient client = server.available(); 
  // hàm .available() của listening socket trả về thông tin kết nối của máy khách đầu tiên trong hàng đợi máy khách (backlog queue) đang muốn kết nối đến server ESP32 này, hoặc "rỗng"
  // khai báo connecting socket 'client' ở phía ESP32 -> gán giá trị trả về bên trên vào socket này để nó làm "đường ống" giao tiếp với máy khách đó
  // socket này sẽ đóng gói và truyền đi dữ liệu truyền thông giữa 2 máy bằng sóng WiFi

  // ĐỂ HIỂU ĐOẠN CODE XỬ LÝ DƯỚI ĐÂY, VUI LÒNG XEM QUA CẤU TRÚC TIÊU CHUẨN CỦA MỘT HTTP REQUEST !!!

  if (client) 
  {
    Serial.println("Có Client mới kết nối!");
    String currentLine = ""; // biến chuỗi lưu nội dung một dòng Header
  
    while (client.connected()) // hàm connected() trả về trạng thái kết nối (true/false) của connecting socket hiện tại
    {
      if (client.available()) // hàm available() của c.socket trả về số byte hiện có trong buffer nhận 
      {
        char c = client.read(); // hàm read() đọc và trả về ký tự đầu tiên trong buffer nhận
        Serial.write(c); // gửi ký tự thô nhận được về máy tính, chưa encode như print() (chuyển thành chuỗi ký tự rồi gửi từng ký tự của chuỗi)
      
        if (c == '\n') // nếu đọc được ký tự '\n' (kết thúc một dòng - chưa biết header hay trống)...
        {
          // Nếu đó là dòng trống (chỉ có \r\n - dùng để ngăn cách Header-Body trong cấu trúc HTTP Request), nghĩa là trình duyệt đã gửi xong toàn bộ HTTP Request Header
          if (currentLine.length() == 0) 
          {  
            // 1. Gửi HTTP Response Header tiêu chuẩn để báo cho Trình duyệt biết là mọi thứ OK
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");

            client.println(); // Dòng trống bắt buộc để phân tách giữa Header và Body

            // 2. MỞ FILE index.html TỪ PHÂN VÙNG LITTLEFS TRONG BỘ NHỚ FLASH ĐỂ ĐỌC DỮ LIỆU
            File webFile = LittleFS.open("/index.html", "r"); // "r" nghĩa là chế độ chỉ đọc (Read)
          
            if (webFile) 
            {
              // Dùng vòng lặp quét từ byte đầu tiên đến byte cuối cùng của file HTML
              while (webFile.available()) // hàm available() trả về true nếu còn ký tự trong file để quét
              {
                // Đọc được byte nào từ Flash là đổ thẳng vào đường ống client để bắn về trình duyệt byte đó
                client.write(webFile.read()); 
              }

              webFile.close(); // Đọc xong phải đóng file lại để giải phóng RAM cho hệ thống
            } 
            else 
            {
              client.println("<h1>Lỗi: Không tìm thấy file index.html trong Flash!</h1>");
            }
    
            break; // Thoát vòng lặp để ngắt kết nối với client này
          } 
          else // nếu không phải dòng trống, tức vừa đọc xong một dòng Header bình thường...
          {
            currentLine = ""; // reset biến lưu chuỗi để chuẩn bị lưu nội dung dòng Header mới
          }
        } 
        else 
        {
          if (c != '\r') // nếu ký tự đọc được không phải là '\n' mà cũng không phải là '\r' 
          {
            currentLine += c; // nối nó vào chuỗi lưu nội dung Header (dùng phương pháp ghép chuỗi để lưu nội dung Header chuẩn, bỏ qua '\n' hay '\r')
          }
        }
      }
    }

    client.stop(); // dừng hoạt động connecting socket đang phục vụ máy khách
    Serial.println();
    Serial.println("Client disconnected.");
  }
}