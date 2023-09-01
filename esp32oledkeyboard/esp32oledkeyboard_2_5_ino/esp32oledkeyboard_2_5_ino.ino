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


String str_hour = "";
byte hour_lenght, j;
unsigned long int_hour;

String str_min = "";
byte min_lenght, k;
unsigned long int_min;



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
    "  TIMER 0:\n\r"  // Не забываем про '\n' - символ переноса строки
    "  Parameter 1:\n\r"
    "  Parameter 2:\n\r"));
  oled.setCursor(90, 2);
  oled.print(int_hour);
  oled.setCursor(104, 2);
  oled.print(F(":"));
  oled.setCursor(110, 2);
  oled.print(int_min);
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
          Serial.println("Решетка");

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

  oled.setCursor(0, pointer);
  oled.print(">");
}

void func(void) {


  oled.clear();
  oled.update();
  oled.home();
  oled.setCursor(0, 0);
  oled.print(F("Press # to return"));
  oled.setCursor(0, 1);
  oled.print(F("Press * to save"));
  oled.setCursor(0, 3);
  oled.print(F("enter time on keyboard"));
  oled.setCursor(0, 4);
  oled.print(F("time"));

  oled.setCursor(50, 4);
  oled.print(int_hour);
  oled.setCursor(70, 4);
  oled.print(int_min);

  oled.update();

  str_hour = "";
  str_min = "";

  while (1) {
    char key = keypad.getKey();



    if (key != NO_KEY) {
      Serial.println(key);



      if (key == '*') {  // если нажата *


        int_hour = str_hour.toInt();
        hour_lenght = str_hour.length();


        Serial.println("Часы записаны");
        Serial.println(int_hour);

        oled.setCursor(50, 4);
        oled.print(int_hour);


        oled.setCursor(30, 7);
        oled.print(F("Hour saved!"));

        oled.update();

        while (1) {
          char key = keypad.getKey();

          if (key != NO_KEY) {
            Serial.println(key);

            if (key == '*') {

              int_min = str_min.toInt();
              min_lenght = str_min.length();


              Serial.println("минуты записаны");
              Serial.println(int_min);



              oled.setCursor(70, 4);
              oled.print(int_min);

              oled.setCursor(30, 7);
              oled.print(F("min saved!"));

              oled.update();

              while (1) {
                char key = keypad.getKey();

                if (key != NO_KEY) {
                  Serial.println(key);
                }
                if (key == '#') {
                  oled.clear();
                  oled.update();
                  return;
                }
              }
            }

            if (key == '#') {
              oled.clear();
              oled.update();
              return;

            } else {
              Serial.println(key);

              str_min += key;
              int_min = str_min.toInt();


              oled.clear(1, 64, 127, 55);
              oled.clear(70, 4 * 8, 80, 5 * 8);
              oled.update();

              if (int_min > 60) {

                oled.setCursor(10, 7);
                oled.print(F("ERROR! min > 60 "));
                Serial.println("ошибка");
                oled.update();
                str_min = "";
                int_min = 1;
                delay(2000);
                oled.clear(1, 64, 127, 55);
                oled.update();
                oled.setCursor(30, 7);
                oled.print(F("Try again"));
                oled.setCursor(50, 4);
                oled.print(str_min);
                oled.update();
                Serial.println("повторнный ввод");
              }
              if (int_min < 0) {

                oled.setCursor(10, 7);
                oled.print(F("ERROR! min < 0"));
                Serial.println("ошибка");
                oled.update();
                str_min = "";
                int_min = 1;
                delay(2000);
                oled.clear(1, 64, 127, 55);
                oled.update();
                oled.setCursor(30, 7);
                oled.print(F("Try again"));
                oled.setCursor(50, 4);
                oled.print(str_min);
                oled.update();
                Serial.println("повторнный ввод");
              }

              Serial.print("минуты:");
              Serial.println(int_min);

              oled.setCursor(70, 4);
              oled.print(int_min);

              oled.update();
            }
          }
        }





        //break;  // выйти из цикла
      } else if (key == '#') {
        oled.clear();
        oled.update();
        return;

      } else {


        str_hour += key;
        int_hour = str_hour.toInt();




        oled.clear(1, 64, 127, 55);
        oled.clear(50, 4 * 8, 60, 5 * 8);
        oled.update();



        if (int_hour > 24) {

          oled.setCursor(10, 7);
          oled.print(F("ERROR! time > 24 "));
          Serial.println("ошибка");
          oled.update();
          str_hour = "";
          int_hour = 1;
          delay(2000);
          oled.clear(1, 64, 127, 55);
          oled.update();
          oled.setCursor(30, 7);
          oled.print(F("Try again"));
          oled.setCursor(50, 4);
          oled.print(str_hour);
          oled.update();
          Serial.println("повторнный ввод");
        }
        if (int_hour < 1) {

          oled.setCursor(10, 7);
          oled.print(F("ERROR! time < 1 "));
          Serial.println("ошибка");
          oled.update();
          str_hour = "";
          int_hour = 1;
          delay(2000);
          oled.clear(1, 64, 127, 55);
          oled.update();
          oled.setCursor(30, 7);
          oled.print(F("Try again"));
          oled.setCursor(50, 4);
          oled.print(str_hour);
          oled.update();
          Serial.println("повторнный ввод");
        }
      }


      Serial.print("часы:");
      Serial.println(int_hour);

      oled.setCursor(50, 4);
      oled.print(int_hour);

      oled.update();
    }
  }
}
