# 🚀 FreeRTOS Multi-Task System on ESP32 — Nhóm 1

> **Môn học:** Hệ điều hành thời gian thực (RTOS)  
> **Trường:** Đại học Sư phạm Kỹ thuật TP.HCM (HCM-UTE)  
> **Học kỳ:** HK2 — Năm học 2025–2026  
> **Nhóm:** 1 — Đợt 1

---

## 📋 Mô tả đề tài

Xây dựng hệ thống đa tác vụ trên ESP32 theo **2 phiên bản**:

| Phiên bản | Số Task | Mô tả |
|-----------|---------|-------|
| **OS (FreeRTOS)** | 8 task | Sử dụng FreeRTOS với Semaphore, Queue, đa tác vụ thực sự |
| **Non-OS** | 6 task | Không dùng OS, dùng `millis()` để giả lập đa nhiệm trong `loop()` |

### Phần cứng sử dụng
- **ESP32-S3**
- 3 LED (LED1, LED2, LED3)
- 2 Button (BTN1, BTN2) — phiên bản OS có thêm BTN3
- Cảm biến **MPU6050** (gia tốc / gyroscope, giao tiếp I2C)
- Màn hình **OLED SSD1306** 128×64 (I2C)
- Kết nối WiFi + **Firebase Firestore** (chỉ phiên bản OS)

---

## 🗂️ Cấu trúc thư mục

```
RTOS_git/
├── README.md
├── codeOS/                         # Phiên bản FreeRTOS (8 task)
│   ├── final_version/
│   │   └── final_version.ino       # Code chính ESP32 (Arduino IDE)
│   └── QT/
│       ├── window.py               # Ứng dụng PyQt5 — gửi data lên Firebase
│       ├── window.ui               # Giao diện Qt Designer
│       ├── image.py                # Resource ảnh (compiled)
│       ├── image.qrc               # Qt resource file
│       ├── icon.png                # Icon ứng dụng
│       └── my-project-rtos-firebase-adminsdk-*.json  # Firebase credential
│
└── codeNonOS/                      # Phiên bản Non-OS (6 task)
    └── Non-OS/
        ├── platformio.ini          # Cấu hình PlatformIO
        └── src/
            └── main.cpp            # Code chính ESP32 (PlatformIO)
```

---

## ⚡ Chi tiết các Task

### 🔵 Phiên bản OS — FreeRTOS (8 Task)

Tất cả các task chạy **liên tục, không dừng**, được lập lịch bởi FreeRTOS scheduler trên **1 lõi CPU** (Core 1).

| Task | Chức năng | Chi tiết |
|------|-----------|----------|
| **Task 1** | LED1 Blink | Nháy LED1 với chu kỳ T = 200ms (toggle mỗi 100ms) |
| **Task 2** | Đọc MPU6050 | Đọc góc nghiêng trục X từ cảm biến, lưu vào biến toàn cục `mpuAngleX` (500ms/lần) |
| **Task 3** | BTN1 → Toggle LED2 | Nhấn BTN1 đảo trạng thái LED2 (có debounce 30ms, priority = 2) |
| **Task 4** | BTN2 → Give Semaphore | Nhấn BTN2 cấp `BinarySemaphore` cho Task 5 |
| **Task 5** | Take Semaphore → LED3 | Đợi nhận semaphore, bật LED3 sáng 3 giây rồi tắt |
| **Task 6** | Hiển thị OLED | Hiển thị trạng thái LED2 (ON/OFF), giá trị MPU, LED3 countdown (500ms/lần) |
| **Task 7** | Gửi data lên Firebase | Nhấn BTN3 → gửi `led2State`, `mpuAngleX`, `led3Countdown` lên Firestore |
| **Task 8** | Nhận data từ Firebase | Sau khi Task 7 gửi xong → nhận 5 data từ server, hiển thị "Downloading", "Done", rồi list 5 data lên OLED |

#### Cơ chế RTOS sử dụng

| Cơ chế | Sử dụng |
|--------|---------|
| `xSemaphoreCreateBinary()` | `semLED3` (BTN2 → LED3), `semSend` (BTN3 → gửi Firebase), `semRecv` (gửi xong → nhận) |
| `xQueueCreate()` | `msgQueue` — lưu 5 data string nhận từ Firebase |
| `xTaskCreatePinnedToCore()` | Tất cả task được pin vào Core 1 |
| `vTaskDelay()` | Sleep task, nhường CPU cho task khác (khác với `delay()` — block toàn bộ CPU) |

### 🟢 Phiên bản Non-OS (6 Task)

Tất cả logic chạy trong `loop()`, dùng `millis()` để giả lập thời gian, **không có hệ điều hành**.

| Task | Chức năng | Chi tiết |
|------|-----------|----------|
| **Task 1** | LED1 Blink | Nháy LED1 chu kỳ 200ms bằng `millis()` |
| **Task 2** | Đọc MPU6050 | Đọc góc nghiêng trục X mỗi 500ms |
| **Task 3** | BTN1 → Toggle LED2 | Nhấn BTN1 đảo trạng thái LED2 (debounce bằng `delay(20)`) |
| **Task 4** | BTN2 → Set flag | Nhấn BTN2 set `binarySemaphore = true` (mô phỏng semaphore bằng biến bool) |
| **Task 5** | LED3 sáng 3 giây | Khi flag = true → bật LED3, đếm ngược 3s rồi tắt |
| **Task 6** | Hiển thị OLED | Hiển thị LED2, MPU, LED3 countdown mỗi 500ms |

> ⚠️ Phiên bản Non-OS **không có Task 7, 8** (WiFi/Firebase) do giới hạn kiến trúc single-threaded.

---

## 🖥️ Ứng dụng PyQt5 — Server (Client gửi data cho ESP32)

Thư mục `codeOS/QT/` chứa ứng dụng desktop **PyQt5** đóng vai trò **client/server**, cho phép:

- Nhập 5 dữ liệu (Data 1 → Data 5) qua giao diện đồ họa
- Preview realtime trước khi gửi
- Gửi dữ liệu lên **Firebase Firestore** (`sensor_data/device1`)
- ESP32 (Task 8) sẽ đọc 5 data này từ Firestore

### Chạy ứng dụng PyQt5

```bash
# Cài đặt thư viện
pip install PyQt5 firebase-admin anyio

# Chạy
cd codeOS/QT
python window.py
```

---

## 🔧 Hướng dẫn cài đặt & chạy

### Phiên bản OS (Arduino IDE)

1. Cài đặt **Arduino IDE** và thêm board **ESP32**
2. Cài đặt các thư viện:
   - `Adafruit SSD1306`
   - `Adafruit GFX`
   - `MPU6050_tockn`
   - `Firebase ESP Client`
3. Mở `codeOS/final_version/final_version.ino`
4. Cấu hình WiFi và Firebase trong code (thay SSID, password, API key)
5. Upload lên ESP32

### Phiên bản Non-OS (PlatformIO)

1. Cài đặt **PlatformIO** (VS Code Extension)
2. Mở thư mục `codeNonOS/Non-OS/`
3. Build & Upload:

```bash
cd codeNonOS/Non-OS
pio run --target upload
```

---

## 📌 Sơ đồ kết nối phần cứng

| Thành phần | GPIO ESP32 |
|-----------|-----------|
| LED1 | GPIO 9 |
| LED2 | GPIO 10 |
| LED3 | GPIO 11 |
| BTN1 | GPIO 16 (INPUT_PULLUP) |
| BTN2 | GPIO 17 (INPUT_PULLUP) |
| BTN3 | GPIO 18 (INPUT_PULLUP) — chỉ OS |
| SDA (I2C) | GPIO 4 |
| SCL (I2C) | GPIO 5 |
| OLED SSD1306 | I2C — Addr 0x3C |
| MPU6050 | I2C — cùng bus |

---

## 🧠 So sánh OS vs Non-OS

| Tiêu chí | FreeRTOS (OS) | Non-OS |
|----------|---------------|--------|
| Đa nhiệm | Thực sự (preemptive multitasking) | Giả lập bằng `millis()` |
| Blocking | `vTaskDelay()` — sleep task, nhường CPU | `delay()` — block toàn bộ CPU |
| Đồng bộ | Semaphore, Queue | Biến `bool` toàn cục |
| WiFi/Firebase | ✅ Có (Task 7, 8) | ❌ Không |
| Độ phức tạp | Cao hơn | Đơn giản hơn |
| Ưu điểm | Quản lý tài nguyên tốt, mở rộng dễ | Nhẹ, dễ hiểu |

---

## 👥 Thành viên nhóm

> Nhóm 1 — Đợt 1 — HK2 năm học 2025–2026

---

## 📄 License

Project phục vụ mục đích học tập môn **Hệ điều hành thời gian thực (RTOS)** tại **HCM-UTE**.
