
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#include "Keypad.h"  //библиотека клавиатуры




char key;

char keys[3][4] = {
  { '0', '8', '5', '2' },
  { '#', '9', '6', '3' },
  { '*', '7', '4', '1' },

};
byte rowPins[] = { 14, 12, 27 };                                   // Подключены строки (4 пина)
byte colPins[] = { 26, 25, 33, 32 };                               // подключены столбцы (4 пина)
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, 3, 4);  //иниициализировать клавиатуру


#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET -1  // Reset pin # (or -1 if sharing Arduino resetpin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
void setup() {
  Serial.begin(115200);
}




void loop() {
  cleandisplay();
  char key = keypad.getKey();

  if (key != NO_KEY) {
    Serial.println(key);

    if (key == '0') {
      cleandisplay();
    }

    if (key == '*') {
      timer1();
    }
    if (key == '#') {
      timer2();
    }
  }
}

void cleandisplay()

{  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;  // Don't proceed, loop forever
  }
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("");
  display.display();
  delay(100);
  display.clearDisplay();
}

void timer1()

{
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Timer 1");
  display.display();
  delay(100);
  display.clearDisplay();
}

void timer2() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Timer 2");
  display.display();
  delay(100);
  display.clearDisplay();
}
