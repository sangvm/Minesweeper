-tạo 1 mennu: Play, High score
-Play
	+ Độ khó: 
		Dễ (easy): bảng 8 × 8 rải 8 quả mìn
		Trung bình (medium): bảng 14 × 14 rải 28 quả mìn
		Khó (hard): bảng 20 x 20 rải 69 quả mìn
		Custom: player tự chọn kích thước bảng và số mìn
	+ Quản lý game:
		+ Sinh thuật toán random mìn
		+ Quản lý các ô trong bảng
		+ Bfs để mở các ô xung quanh ô được chọn
		+ Xử lý menu game
		+ Lưu trữ ảnh, font, nhạc game
	+ Cách chơi: 
		+ Click chuột trái vào mỗi ô chưa chọn mà player cho đó k phải là mìn
		+ Click chuột phải để đánh dấu 1 ô mà player cho là mìn
	+ Tính thời gian player đang chơi
	+ Đếm số mìn còn lại dựa trên số cờ đã sử dụng
	+ Sang tạo <Lựa chọn nhân vật>: mỗi nhân vật có 1 bộ kĩ năng riêng để tạo thêm sự thú vị cho trò chơi
<Ở các nhân vật 2, 3, 4: player sẽ thua nếu như chọn ô mìn hoặc ô trống cuối cùng của bảng bị đặt thêm mìn>
- High score : lưu trữ thời gian ngắn nhất mà player đạt được với từng độ khó

Nguồn tham khảo:
https://github.com/thanhtvt/minesweeperSDL/tree/Minesweeper-(so-called)/Minesweeper_with_SDL <Cấu trúc cơ bản của 1 game minesweeper>
https://lazyfoo.net/tutorials/SDL/index.php <Sử dụng cơ bản SDL>

Tự đánh giá: 9.5



