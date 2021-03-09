-tạo 1 màn hình với các option: Play, How to play, High score, tắt bật âm thanh game
-Play
	+ Độ khó: 
		Dễ (easy): bảng 8 × 8 rải 10 quả mìn
		Trung bình (medium): bảng 14 × 14 rải 36 quả mìn
		Khó (hard): bảng 20 x 20 rải 88 quả mìn
		Custom: player tự chọn kích thước bảng và số mìn
	+ Sinh thuật random để tạo mìn
	+ Cách chơi: sử dụng chuột trái để click vào mỗi ô chưa chọn mà player cho đó k phải là mìn
		+ với mỗi ô là mìn: -> thua
		+ với mỗi ô k phải là mìn: bfs để tính số ô mìn 8 ô xung quanh
	+ Tính thời gian player đang chơi
- How to play:  chuột trái để chọn 1 ô mà player cho k phải là mìn
	   	chuột phải để đánh dâu 1 ô mà player cho là mìn
- High score : lưu trữ thời gian ngắn nhất mà player đạt được với từng độ khó
		(có thể lưu trữ lịch sử chơi của player, tạo thêm 1 nút reset history)

