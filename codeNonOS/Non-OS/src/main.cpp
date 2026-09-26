#include <Arduino.h>
#include <Wire.h>
#include <MPU6050_tockn.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define LED1 9
#define LED2 10
#define LED3 11
#define BTN1 16
#define BTN2 17
#define SDA_PIN 4  
#define SCL_PIN 5

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR 0x3C

MPU6050 mpu6050(Wire);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

unsigned long previousmillis_LED1 = 0;
unsigned long previousmillis_MPU = 0;
unsigned long previousmillis_OLED = 0;
unsigned long startMillis_LED3 = 0;

bool previous_BTN1 = HIGH;
bool previous_BTN2 = HIGH;

float angle_X = 0;
unsigned long LED3_count = 0; 
bool binarySemaphore = false; 

void setup() {
    Serial.begin(115200);

    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
    pinMode(LED3, OUTPUT);
    pinMode(BTN1, INPUT_PULLUP);
    pinMode(BTN2, INPUT_PULLUP);

    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);

    Wire.begin(SDA_PIN, SCL_PIN);
    mpu6050.begin();
    mpu6050.calcGyroOffsets(true);

    display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
    display.clearDisplay();
    display.display();
}

void loop() {
    unsigned long currentMillis = millis();
     
    // TASK 1 
    if (currentMillis - previousmillis_LED1 >= 100) {
        digitalWrite(LED1, !digitalRead(LED1));
        previousmillis_LED1 = currentMillis;
    }

    // TASK 2
    if (currentMillis - previousmillis_MPU >= 500) {
        mpu6050.update();
        angle_X = mpu6050.getAccAngleX(); 
        previousmillis_MPU = currentMillis;
    }

    // TASK 3
    bool current_BTN1 = digitalRead(BTN1);

    if (previous_BTN1 == HIGH && current_BTN1 == LOW) {
        delay(20);  // debounce

        if (digitalRead(BTN1) == LOW) {
            digitalWrite(LED2, !digitalRead(LED2));
        }
    }

    previous_BTN1 = current_BTN1;


    // TASK 4
    bool current_BTN2 = digitalRead(BTN2);

    if (previous_BTN2 == HIGH && current_BTN2 == LOW) {
        delay(20);  // debounce

        if (digitalRead(BTN2) == LOW) {
            binarySemaphore = true;
        }
    }

    previous_BTN2 = current_BTN2;

    // TASK 5
    if (binarySemaphore && digitalRead(LED3) == LOW) {
        digitalWrite(LED3, HIGH);
        startMillis_LED3 = currentMillis; 
        binarySemaphore = false;          
    }

    if (digitalRead(LED3) == HIGH) {
        unsigned long timeElapsed = currentMillis - startMillis_LED3;
        if (timeElapsed >= 3000) {
            digitalWrite(LED3, LOW);
            LED3_count = 0;
        } else {
            LED3_count = 3000 - timeElapsed;
        }
    } else {
        LED3_count = 0; 
    }

    // TASK 6
    if (currentMillis - previousmillis_OLED >= 500) {
        previousmillis_OLED = currentMillis;

        display.clearDisplay();
        display.setTextColor(SSD1306_WHITE);

        // dòng 1: LED2 
        display.setCursor(0, 0);
        display.setTextSize(1);
        display.print("LED2: ");
        display.println(digitalRead(LED2) ? "ON" : "OFF");

        // dòng 2: MPU 
        display.setCursor(0, 20);
        display.print("MPU: ");
        display.print(angle_X, 1); 
        display.print(" deg");

        // dòng 3: đếm LED3
        display.setCursor(0, 40);
        display.print("LED3: ");
        if (digitalRead(LED3) == HIGH) {
            display.print(LED3_count / 1000.0, 1);
            display.println("s");
        } else {
            display.println("OFF");
        }
   
        display.display();
    }
}