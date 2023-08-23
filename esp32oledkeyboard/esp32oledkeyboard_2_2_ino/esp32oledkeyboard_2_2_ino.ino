#define OLED_SOFT_BUFFER_64
#include <GyverOLED.h>
GyverOLED<SSD1306_128x64, OLED_BUFFER> oled;

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
int back;
int pointer;


String str_pass = "";
byte pass_lenght, j;
unsigned long int_pass;

const uint8_t ptr_bmp[] PROGMEM = {
  0x3C,
  0x3C,
  0x3C,
  0x3C,
  0x3C,
  0xFF,
  0xFF,
  0x7E,
  0x3C,
  0x18,
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
  ok = 0;
  back = 0;
  pointer = 2;
  int_pass = 1234;
}


void loop() {
  up = 0;
  down = 0;
  ok = 0;
  back = 0;
  char key = keypad.getKey();

  if (key != NO_KEY) {
    Serial.println(key);
  }


  if (up == 1) {
    pointer--;
    if (pointer < 2) pointer = 2;
  }

  if (down == 1) {
    pointer++;
    if (pointer > 4) pointer = 4;
  }

  if (ok == 1) {              // Нажатие на ОК - переход в пункт меню
    switch (pointer) {        // По номеру указателей располагаем вложенные функции (можно вложенные меню)
      case 2: func(); break;  // По нажатию на ОК при наведении на 0й пункт вызвать функцию
      case 3: break;
      case 4:
        break;

        // И все остальные
    }
  }




  oled.home();  // курсор в 0,0
  oled.setCursor(0, 2);
  oled.print(F(
    "  Parameter 0:\n\r"  // Не забываем про '\n' - символ переноса строки
    "  Parameter 1:\n\r"
    "  Parameter 2:\n\r"));
  printPointer(pointer);
  oled.update();
}

void keypadEvent(KeypadEvent key) {
  switch (keypad.getState()) {
    case PRESSED:
      switch (key) {
        case '2':
          up = 1;
          Serial.println("up");

          break;

        case '8':
          down = 1;
          Serial.println("down");

          break;

        case '5':
          ok = 1;
          Serial.println("ok");

          break;

        case '#':
          back = 1;
          Serial.println("#");

          break;
      }
      break;
    case RELEASED:
      switch (key) {
        case '2':
          // up = 0;
          //Serial.println(up);
          break;

        case '8':
          // down = 0;
          //Serial.println(down);
          break;

        case '5':
          // ok = 0;
          //Serial.println(ok);
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

void printPointer(int pointer) {
  // Символьный указатель - лучший выбор в многостраничном меню
  // Указатель в начале строки
  oled.setCursor(0, pointer);
  oled.print(">");
  // Можно еще в конце
  //oled.setCursor(120, pointer);
  //oled.print("<");

  // oled.drawBitmap(0, ptr * 8, ptr_bmp, 10, 8);
}

void func(void) {

  str_pass = String(int_pass, DEC);
  pass_lenght = str_pass.length();

  oled.clear();
  oled.update();
  oled.home();
  oled.setCursor(0, 0);
  oled.print(F("Press # to return"));
  oled.setCursor(0, 3);
  oled.print(F("Enter time"));
  oled.update();
  oled.setCursor(50, 4);
  oled.print(int_pass);
  oled.setCursor(0, 4);
  oled.print("время");
  oled.setCursor(0, 5);
  oled.print("длинна");
  oled.setCursor(50, 5);
  oled.print(pass_lenght);
  oled.setCursor(0, 6);
  oled.print("ввожу");
  oled.update();
  str_pass = "";

  while (1) {
    char key = keypad.getKey();



    if (key != NO_KEY) {
      Serial.println(key);



      if (key == '*') {  // если нажата *


        int_pass = str_pass.toInt();
        pass_lenght = str_pass.length();  // получиаем длину пароля


        Serial.println("Пароль записан");
        Serial.println(int_pass);
        Serial.println(pass_lenght);

        oled.setCursor(50, 4);
        oled.print(int_pass);
        oled.setCursor(50, 5);
        oled.print(pass_lenght);
        oled.update();
        while (1) {
          char key = keypad.getKey();

          if (key != NO_KEY) {
            Serial.println(key);
            if (key == '#') {
              oled.clear();
              oled.update();
              return;
            }
          }
        }





        //break;  // выйти из цикла
      } else if (key == '#') {
        oled.clear();
        oled.update();
        return;
      } else {  // если * не нажата
        str_pass += key;
        Serial.print("Пароль:");
        Serial.println(str_pass);

        oled.setCursor(50, 6);
        oled.print(str_pass);
        oled.update();
      }
    }
  }
}