<p align="center"> 
 <h1 align="center">Ninja Adventure - 24020359 Hoàng Quốc Việt</h1>
</p>
Dự án trò chơi này là một phần của môn học "Lập trình nâng cao" tại Trường Đại học Công nghệ - Đại học Quốc gia Hà Nội.

# Mô tả
- Dự án này được phát triển bằng __ngôn ngữ lập trình C++__ và sử dụng __thư viện SDL2__.
- Dự án được lấy cảm hứng từ tựa game __Savage Pursuit__.
- Trong __Ninja Adventure__, người chơi sẽ bước vào một thế giới phong phú đầy màu sắc và mạo hiểm. Họ sẽ nhập vai vào một ninja dũng mãnh, sẵn sàng đối đầu với những thử thách khốc liệt, những quái vật cản đường và tiến lên phía trước.

# Hướng dẫn chơi  
## Giao diện ban đầu
![image](https://github.com/HoangQuocVietIT24020359/Ninja_Adventure_3/blob/main/tutorial/images/menu_levels.jpg)
Gồm có 10 levels cho người chơi lựa chọn.

## Giao diện chơi game
__1. Thanh máu__

![image](https://github.com/HoangQuocVietIT24020359/Ninja_Adventure_3/blob/main/tutorial/images/health_bar.jpg)

Người chơi sẽ bắt đầu với 3 mạng, được hiển thị dưới dạng thanh màu: xanh, vàng và đỏ. Khi mất cả 3 mạng, trò chơi sẽ kết thúc.


__2. Quái vật__

| Quái vật | Mô tả |
| -------- | ----- |
| ![image](https://github.com/HoangQuocVietIT24020359/Ninja_Adventure_3/blob/main/tutorial/images/fox.jpg) | fox |
| ![image](https://github.com/HoangQuocVietIT24020359/Ninja_Adventure_3/blob/main/tutorial/images/snail.jpg) | snail |

Tốt nhất là đừng chạm vào chúng nếu bạn không muốn mất mạng!!!

__3. Bẫy__

![image](https://github.com/HoangQuocVietIT24020359/Ninja_Adventure_3/blob/main/tutorial/images/trap.jpg)

Rất sắc nhọn và có thể khiến bạn chảy máu nếu chạm vào chúng!!!

__4. Thuốc hồi máu__

<img src="https://github.com/HoangQuocVietIT24020359/Ninja_Adventure_3/blob/main/tutorial/assets/Health/heath_potion.png" width="200">


Xuất hiện ngẫu nhiên trên map, khi sử dụng sẽ hồi lại cho người chơi 1 lượng máu.

__5. Save point__

![image](https://github.com/HoangQuocVietIT24020359/Ninja_Adventure_3/blob/main/tutorial/images/save_point.jpg)

Khi người chơi mất 1 mạng, Save point sẽ hồi sinh người chơi về lại chỗ của nó.

__6. Cổng dịch chuyển__

<img src="https://raw.githubusercontent.com/HoangQuocVietIT24020359/Ninja_Adventure_3/main/tutorial/assets/checkpoint/door.png" width="200">


Có khả năng giúp người chơi dịch chuyển sang màn chơi khác.

__7. Màn hình tạm dừng__

![image](https://github.com/HoangQuocVietIT24020359/Ninja_Adventure_3/blob/main/tutorial/images/menu_levels.jpg)

Khi người chơi bấm nút `q` màn hình menu sẽ hiện ra:
- Người chơi có thể chọn 1 trong 10 levels để chơi.
- Hoặc bấm lại nút `q` để tiếp tục trò chơi.

__8. Màn hình thua cuộc__

![image](https://github.com/HoangQuocVietIT24020359/Ninja_Adventure_3/blob/main/tutorial/images/menu_loss.jpg)

Khi nhân vật sử dụng hết 3 mạng của mình. Màn hình kết thúc sẽ hiện ra:
- Nếu chọn `return to menu` người chơi sẽ quay lại menu levels để chọn level cho mình.
- Nếu chọn `restart level` người chơi sẽ được hồi sinh tại vị trí ban đầu của Level và trò chơi sẽ bắt đầu lại từ đầu.

__9. Màn hình Chiến thắng__

![image](https://github.com/HoangQuocVietIT24020359/Ninja_Adventure_3/blob/main/tutorial/images/menu_win.jpg)

Khi người chơi đi đến cuối bản đồ (Đã hoàn thành màn chơi). Màn hình hoàn thành sẽ hiện ra:
- Nếu chọn `return to menu` người chơi sẽ quay lại menu levels để chọn level cho mình.
- Nếu chọn `restart level` người chơi sẽ được hồi sinh tại vị trí ban đầu của Level và trò chơi sẽ bắt đầu lại từ đầu.
- Nếu chọn `Next Level` thì `Cổng dịch chuyển` sẽ đưa người chơi đến màn chơi tiếp theo.

# Cấu trúc project 
- [assets](https://github.com/HoangQuocVietIT24020359/Ninja_Adventure_2/tree/main/tutorial/assets): Chứa các tài nguyên của game như âm thanh, hình ảnh, sprite sheet.
- [src](https://github.com/HoangQuocVietIT24020359/Ninja_Adventure_2/tree/main/tutorial/src): Chứa toàn bộ mã nguồn của trò chơi.
- [Animation](https://github.com/HoangQuocVietIT24020359/Ninja_Adventure_2/tree/main/tutorial/src/Animation): Xử lý hiệu ứng animation của nhân vật và quái vật, giúp chúng có thể di chuyển, nhảy, tấn công một cách mượt mà.
- [Camera](https://github.com/HoangQuocVietIT24020359/Ninja_Adventure_2/tree/main/tutorial/src/Camera): Điều khiển góc nhìn của người chơi, giúp nhân vật luôn hiển thị ở trung tâm màn hình.
- [Character](https://github.com/HoangQuocVietIT24020359/Ninja_Adventure_2/tree/main/tutorial/src/Characters): Chứa logic điều khiển nhân vật và quái vật như di chuyển, nhảy, nhận sát thương, hồi máu, hồi sinh và xử lý va chạm.
- [Checkpoint](https://github.com/HoangQuocVietIT24020359/Ninja_Adventure_2/tree/main/tutorial/src/Checkpoint): Thư mục code phần logic cho save point. Xử lý logic khi nhân vật chạm vào save point thì cho save point chạy animation.
- [Collision](https://github.com/HoangQuocVietIT24020359/Ninja_Adventure_2/tree/main/tutorial/src/Collision): Xác định xem hai đối tượng có đang va chạm hay không.
- [Core](https://github.com/HoangQuocVietIT24020359/Ninja_Adventure_2/tree/main/tutorial/src/Core): Chứa các thành phần cốt lõi của trò chơi, bao gồm cách khởi tạo nhân vật, bản đồ, xử lý vòng lặp game,...
- [Goal](https://github.com/HoangQuocVietIT24020359/Ninja_Adventure_2/tree/main/tutorial/src/Goal): Xử lý logic của cổng dịch chuyển trong game.
- [Graphics](https://github.com/HoangQuocVietIT24020359/Ninja_Adventure_2/tree/main/tutorial/src/Graphics): Quản lý hình ảnh và animation của nhân vật, quái vật, giúp hiển thị chúng lên màn hình.
- [Health](https://github.com/HoangQuocVietIT24020359/Ninja_Adventure_2/tree/main/tutorial/src/Health): Xử lý hệ thống thanh máu của nhân vật và quái vật.
- [Item](https://github.com/HoangQuocVietIT24020359/Ninja_Adventure_2/tree/main/tutorial/src/Item): Quản lý các vật phẩm trong game, chẳng hạn như item hồi máu.
- [Map](https://github.com/HoangQuocVietIT24020359/Ninja_Adventure_2/tree/main/tutorial/src/Map): Chứa dữ liệu về bản đồ, gồm 10 bản đồ tương ứng với 10 cấp độ. Khi người chơi chọn level, bản đồ tương ứng sẽ được tải lên.
- [Menu](https://github.com/HoangQuocVietIT24020359/Ninja_Adventure_2/tree/main/tutorial/src/Menu): Xử lý giao diện menu của trò chơi, bao gồm màn hình chọn level, menu thắng/thua,...
- [Object](https://github.com/HoangQuocVietIT24020359/Ninja_Adventure_2/tree/main/tutorial/src/Object): Tạo class cơ sở để cho các class nhân vật, quái vật sau có thể kế thừa.
- [Physics](https://github.com/HoangQuocVietIT24020359/Ninja_Adventure_2/tree/main/tutorial/src/Physics): Xử lý cơ chế vật lý trong game, bao gồm trọng lực, di chuyển trái/phải và sức nhảy của nhân vật/quái vật.
- [Sound](https://github.com/HoangQuocVietIT24020359/Ninja_Adventure_2/tree/main/tutorial/src/Sound):  Quản lý hiệu ứng âm thanh của trò chơi.
- [Timer](https://github.com/HoangQuocVietIT24020359/Ninja_Adventure_2/tree/main/tutorial/src/Timer): Xử lý bộ đếm thời gian để đồng bộ các animation và hành động trong game.
- [Input](https://github.com/HoangQuocVietIT24020359/Ninja_Adventure_2/tree/main/tutorial/src/inputs): Xử lý đầu vào từ người chơi, bao gồm bàn phím và chuột.
- [Main.cpp](https://github.com/HoangQuocVietIT24020359/Ninja_Adventure_2/blob/main/tutorial/main.cpp): File chính để chạy game.

