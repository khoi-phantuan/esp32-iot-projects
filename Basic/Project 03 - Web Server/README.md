Loại 1: Listening Socket (Socket lắng nghe - Ví dụ: WiFiServer)
Nhiệm vụ duy nhất của nó là "gác cổng" để xếp hàng các yêu cầu kết nối tới, nó không trực tiếp truyền nhận dữ liệu ứng dụng.

Nó lưu trữ:

Local IP & Local Port (Để biết nó phải canh gác ở cổng nào của máy mình).

Socket Descriptor (Mã định danh quản lý).

Connection State (Mặc định luôn ở trạng thái LISTEN).

Backlog Queue (Hàng đợi kết nối): Đây là thông tin đặc thù của riêng nó! Một danh sách các ô nhớ để chứa tạm thông tin của các máy khách đang xếp hàng chờ được chấp nhận (hàm .available() chính là vào hàng đợi này để nhặt ra).

Nó KHÔNG lưu: Remote IP và Remote Port (Vì nó gác cổng chung cho tất cả mọi người, ai đến nó cũng tiếp chứ không bắt cặp riêng với ai cả), và nó cũng không cần RX/TX Buffer.

Loại 2: Connected Socket / Stream Socket (Socket kết nối - Ví dụ: WiFiClient)
Đây là loại Socket mà Khôi và mình đã mổ xẻ nhiều nhất. Nó sinh ra từ giao thức TCP để phục vụ giao tiếp đường ống kết nối 1-1.

Nó lưu trữ đầy đủ nhất:

Bộ 4 định danh toàn vẹn: [Local IP, Local Port, Remote IP, Remote Port].

Socket Descriptor.

Connection State (Thay đổi liên tục: SYN_SENT, ESTABLISHED, CLOSE_WAIT...).

Receive Buffer (RX) & Transmit Buffer (TX) (Bắt buộc phải có để chứa các byte dữ liệu thô truyền nhận).

Loại 3: Datagram Socket (Socket phi kết nối - Ví dụ: WiFiUDP trong Arduino hoặc UdpClient trong C#)
Loại này dùng cho giao thức UDP. Vì UDP là giao thức "bắn rồi bỏ", không cần thiết lập đường ống kết nối, không cần bắt tay.

Nó lưu trữ:

Local IP & Local Port (Để hệ điều hành biết gói tin UDP gửi đến cổng nào thì ném vào cho nó).

Socket Descriptor.

Receive Buffer (RX) (Để hứng các gói tin bay tới).

Nó KHÔNG lưu: Remote IP và Remote Port cố định trong cấu trúc ô nhớ của nó! Khi bạn muốn bắn dữ liệu đi bằng UDP, bạn phải truyền địa chỉ Remote trực tiếp vào hàm gửi (ví dụ: udp.beginPacket(remoteIP, remotePort)). Gửi xong là nó xóa sạch ký ức về đối tác đó luôn, Socket lại trở về trạng thái tự do để sẵn sàng nhận/gửi với một IP khác.