#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <MPU6050_tockn.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>

/* WIFI + FIREBASE */
#define WIFI_SSID "NhanEnten"
#define WIFI_PASSWORD "Nhan2311"

#define USER_EMAIL "test@test.com"
#define USER_PASSWORD "123456"

#define API_KEY "AIzaSyC8vcxpJBbG1PRFnk_GTd8KETXxrCuW-JU"
#define FIREBASE_PROJECT_ID "my-project-rtos"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

/* PIN */
#define LED1 9
#define LED2 10
#define LED3 11
#define BTN1 16
#define BTN2 17
#define BTN3 18
#define SDA_PIN 4
#define SCL_PIN 5

/* OLED */
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

/* MPU */
MPU6050 mpu6050(Wire);

/* RTOS */
SemaphoreHandle_t semLED3;
SemaphoreHandle_t semSend;
SemaphoreHandle_t semRecv;

QueueHandle_t msgQueue;

/* GLOBAL */
float mpuAngleX = 0;
bool led2State = false;
float led3Countdown = 0;

volatile int oledMode = 0;     // 0 normal | 1 downloading | 2 done | 3 show list
volatile int downloadCount = 0;

/* TASK 1 – LED1 BLINK T = 200ms */
void taskLED1(void *pv) {
  while (1) {
    digitalWrite(LED1, !digitalRead(LED1));
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

/* TASK 2 – READ MPU6050 */
void taskMPU(void *pv) {
  while (1) {
    mpu6050.update();
    mpuAngleX = mpu6050.getAngleX();
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}

/* TASK 3 – BTN1 TOGGLE LED2 (HIGH PRIORITY) */
void taskBTN1(void *pv) {
  int prev = HIGH;
  while (1) {
    int cur = digitalRead(BTN1);

    if (prev == HIGH && cur == LOW) {
      vTaskDelay(pdMS_TO_TICKS(30));
      if (digitalRead(BTN1) == LOW) {
        led2State = !led2State;
        digitalWrite(LED2, led2State);
      }
      while (digitalRead(BTN1) == LOW) vTaskDelay(pdMS_TO_TICKS(10));
    }

    prev = cur;
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

/* TASK 4 – BTN2 GIVE SEMAPHORE */
void taskBTN2(void *pv) {
  int prev = HIGH;
  while (1) {
    int cur = digitalRead(BTN2);

    if (prev == HIGH && cur == LOW) {
      vTaskDelay(pdMS_TO_TICKS(30));
      if (digitalRead(BTN2) == LOW) {
        xSemaphoreGive(semLED3);
      }
      while (digitalRead(BTN2) == LOW) vTaskDelay(pdMS_TO_TICKS(10));
    }

    prev = cur;
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

/* BTN3 */
void taskBTN3(void *pv) {
  int prev = HIGH;
  while (1) {
    int cur = digitalRead(BTN3);

    if (prev == HIGH && cur == LOW) {
      vTaskDelay(pdMS_TO_TICKS(30));
      if (digitalRead(BTN3) == LOW) {
        xSemaphoreGive(semSend);
      }
      while (digitalRead(BTN3) == LOW) vTaskDelay(pdMS_TO_TICKS(10));
    }

    prev = cur;
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

/* TASK 5 – TAKE SEMAPHORE → LED3 sáng 3 giây */
void taskLED3(void *pv) {
  while (1) {
    if (xSemaphoreTake(semLED3, portMAX_DELAY)) {

      digitalWrite(LED3, HIGH);

      for (led3Countdown = 3; led3Countdown > 0; led3Countdown -= 0.5) {
        vTaskDelay(pdMS_TO_TICKS(500));
      }

      digitalWrite(LED3, LOW);
      led3Countdown = 0;
    }
  }
}

/* TASK 6 – OLED DISPLAY */
void taskOLED(void *pv) {
  while (1) {

    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

    if (oledMode == 0) {
      display.setCursor(0, 0);
      display.print("LED2: ");
      display.println(led2State ? "ON" : "OFF");

      display.setCursor(0, 12);
      display.print("MPU X: ");
      display.println(mpuAngleX, 1);

      display.setCursor(0, 24);
      display.print("LED3: ");
      if (led3Countdown > 0) {
        display.print(led3Countdown, 1);
        display.println("s");
      } else display.println("OFF");
    }

    else if (oledMode == 1) {
      display.setCursor(0, 0);
      display.print("LED2: ");
      display.println(led2State ? "ON" : "OFF");

      display.setCursor(0, 12);
      display.print("MPU X: ");
      display.println(mpuAngleX, 1);

      display.setCursor(0, 24);
      display.print("LED3: ");
      if (led3Countdown > 0) {
        display.print(led3Countdown, 1);
        display.println("s");
      } else display.println("OFF");

      display.setCursor(0, 40);
      display.print("Downloading ");
      display.print(downloadCount);
      display.print("/5");
    }

    else if (oledMode == 2) {
      display.setCursor(0, 0);
      display.print("LED2: ");
      display.println(led2State ? "ON" : "OFF");

      display.setCursor(0, 12);
      display.print("MPU X: ");
      display.println(mpuAngleX, 1);

      display.setCursor(0, 24);
      display.print("LED3: ");
      if (led3Countdown > 0) {
        display.print(led3Countdown, 1);
        display.println("s");
      } else display.println("OFF");
      
      display.setCursor(0, 40);
      display.print("Done");
    }

    else if (oledMode == 3) {

      String temp[5];
      String *msg;

      for (int i = 0; i < 5; i++) {
        if (xQueueReceive(msgQueue, &msg, 0)) {
          temp[i] = *msg;
          delete msg; 
        }
      }

      for (int i = 0; i < 5; i++) {
        display.setCursor(0, i * 12);
        display.println(temp[i]);

        // push lai queue
        String *copy = new String(temp[i]);
        if (uxQueueSpacesAvailable(msgQueue) > 0) {
          xQueueSend(msgQueue, &copy, 0);
        }
      }
    }

    display.display();
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}

/* INIT FIREBASE FIRESTORE */
void initFirebase() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  config.api_key = API_KEY;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  Serial.print("Waiting Firebase");

  while (!Firebase.ready()) {
    Serial.print(".");
    delay(300);
  }

  Serial.println("\nFirebase ready!");
}

/* TASK 7 – GỬI DỮ LIỆU LÊN FIREBASE FIRESTORE */
void taskSend(void *pv) {
  while (1) {
    if (xSemaphoreTake(semSend, portMAX_DELAY)) {

      if (Firebase.ready()) {

        FirebaseJson content;
        content.set("fields/led2/booleanValue", led2State);
        content.set("fields/mpuX/doubleValue", mpuAngleX);
        content.set("fields/led3/doubleValue", led3Countdown);

        if (Firebase.Firestore.patchDocument(
              &fbdo,
              FIREBASE_PROJECT_ID,
              "(default)",
              "sensor_data/device1",
              content.raw(),
              "led2,mpuX,led3")) {

          xSemaphoreGive(semRecv);
        }
      }
    }
  }
}

/* TASK 8 – NHẬN DỮ LIỆU TỪ FIREBASE (sau mỗi lần Task7 gửi) */
void taskReceive(void *pv) {

  while (1) {

    // Cho task 7 gui du lieu len FB
    if (xSemaphoreTake(semRecv, portMAX_DELAY)) {

      if (Firebase.ready()) {

        // Xoa du lieu trong queue
        String *oldMsg;

        while (xQueueReceive(msgQueue, &oldMsg, 0)) {
          delete oldMsg;
        }

        // Chuyen OLED sang downloading
        oledMode = 1;
        downloadCount = 0;
        vTaskDelay(pdMS_TO_TICKS(100));

        for (int i = 1; i <= 5; i++) {

          bool received = false;

          // Cho nhan data thu i
          while (!received) {

            if (Firebase.ready()) {

              // Moi vong se GET database mot lan
              if (Firebase.Firestore.getDocument(
                    &fbdo,
                    FIREBASE_PROJECT_ID,
                    "(default)",
                    "sensor_data/device1")) {

                FirebaseJson json;
                json.setJsonData(fbdo.payload());

                FirebaseJsonData result;

                // Tao duong dan data1, data2, ..., data5
                String path = "fields/data" + String(i) + "/stringValue";

                if (json.get(result, path)) {

                  // Luu data vao queue
                  String *data = new String(result.stringValue);

                  xQueueSend(msgQueue, &data, portMAX_DELAY);

                  // Hien thi Downloading i/5
                  downloadCount = i;

                  received = true;
                }

                else {
                  Serial.println("Missing: " + path);
                }
              }

              else {
                Serial.println("GET FAIL: " + fbdo.errorReason());
              }
            }

            if (!received) {
              vTaskDelay(pdMS_TO_TICKS(200));
            }
          }

          if (i < 5) {
            vTaskDelay(pdMS_TO_TICKS(random(500, 1000)));
          }
        }

        // Nhan du 5 data -> Done
        oledMode = 2;
        vTaskDelay(pdMS_TO_TICKS(500));

        // list 5 data trong queue
        oledMode = 3;
        vTaskDelay(pdMS_TO_TICKS(2000));

        // Normal
        oledMode = 0;
      }
    }
  }
}

/* SETUP */
void setup() {
  Serial.begin(115200);

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(BTN1, INPUT_PULLUP);
  pinMode(BTN2, INPUT_PULLUP);
  pinMode(BTN3, INPUT_PULLUP);

  Wire.begin(SDA_PIN, SCL_PIN);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.display();

  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);

  initFirebase();

  semLED3 = xSemaphoreCreateBinary();
  semSend = xSemaphoreCreateBinary();
  semRecv = xSemaphoreCreateBinary();

  msgQueue = xQueueCreate(5, sizeof(String*));

  xTaskCreatePinnedToCore(taskLED1, "LED1", 2048, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(taskMPU, "MPU", 4096, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(taskBTN1, "BTN1", 2048, NULL, 2, NULL, 1);
  xTaskCreatePinnedToCore(taskBTN2, "BTN2", 2048, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(taskBTN3, "BTN3", 2048, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(taskLED3, "LED3", 2048, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(taskOLED, "OLED", 4096, NULL, 1, NULL, 1);

  xTaskCreatePinnedToCore(taskSend, "Send", 8192, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(taskReceive, "Recv", 8192, NULL, 1, NULL, 1);
}

void loop() {}