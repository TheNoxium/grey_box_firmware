#define OLED_SOFT_BUFFER_64
#include <GyverOLED.h>
GyverOLED<SSD1306_128x64, OLED_NO_BUFFER> oled;

#include "Keypad.h"  //библиотека клавиатуры
char key;
char timer1ent;
char keys[3][4] = {
  { '0', '8', '5', '2' },
  { '#', '9', '6', '3' },
  { '*', '7', '4', '1' },

};
byte rowPins[] = { 14, 12, 27 };                                   // Подключены строки (4 пина)
byte colPins[] = { 26, 25, 33, 32 };                               // подключены столбцы (4 пина)
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, 3, 4);  //иниициализировать клавиатуру


int up;
int down;
int ok;
int pointer;

const uint8_t ptr_bmp[] PROGMEM = {
  0x3C, 0x3C, 0x3C, 0x3C, 0x3C, 0xFF, 0xFF, 0x7E, 0x3C, 0x18,
};


void setup() {
  Serial.begin(115200);
  oled.init();  // инициализация
  oled.clear();
  oled.update();  // очистка
  keypad.addEventListener(keypadEvent);
  keypad.setDebounceTime(250);
  keypad.setHoldTime(500);  
  up = 0;
  down = 0;
  
}
void loop() {
 

  char key = keypad.getKey();

  if (key != NO_KEY) {
    Serial.println(key);

  }

  if (up == 1) {  
    pointer--; 
     Serial.println("up");             
     
  }

  if (down == 1) {
    pointer++;
     Serial.println("down");
    
  }



  oled.home();  // курсор в 0,0
  oled.print(F(
    "  Parameter 0:\n\r"  // Не забываем про '\n' - символ переноса строки
    "  Parameter 1:\n\r"));
  printPointer(pointer); 
  oled.update();
  
}

void keypadEvent(KeypadEvent key) {
  switch (keypad.getState()) {
    case PRESSED:
      switch (key) {
        case '2':
          up = 1;
          Serial.println(up);
          delay(100);
          break;

        case '8':
          down = 1;
          Serial.println(down);
          delay(100);
          break;

        case '5':
          ok = 1;
          Serial.println(ok);
          break;
      }
      break;
    case RELEASED:
      switch (key) {
        case '2':
          up = 0;
          Serial.println(up);
          break;

        case '8':
          down = 0;
          Serial.println(down);
          break;

        case '5':
          ok = 0;
          Serial.println(ok);
          break;
      }
      break;
    case HOLD:
      switch (key) {
        case '*': break;
      }
      break;
  }
}

void printPointer(int ptr) {
  // Символьный указатель - лучший выбор в многостраничном меню
  // Указатель в начале строки
  oled.setCursor(0, ptr);


  oled.drawBitmap(0, ptr * 8, ptr_bmp, 10, 8);
}