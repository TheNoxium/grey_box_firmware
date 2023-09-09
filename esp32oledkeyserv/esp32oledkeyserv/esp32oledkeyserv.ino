#include <WiFi.h>

const char* ssid = "PR1HOD1 NA BYH14";
const char* password = "88888888q";
const char* host = "192.168.0.110";

unsigned long timer;

byte flag;

#include "ArduinoJson.h"

int numProfiles;
int numProfilesmenu;
int numProfilesPointer;

int* ids;
int* startTimes;
int* endTimes;

int idmenu[5];
int startTimemenu[5];
int endTimemenu[5];


String message;
String messagemenu;
String jsonString;

#include <HTTPClient.h>

const char* serverUrldelete = "http://192.168.0.110:80/api/remove_profile";




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
  delay(1000);

  oled.init();
  oled.clear();
  oled.update();

  keypad.addEventListener(keypadEvent);
  keypad.setDebounceTime(250);
  keypad.setHoldTime(500);
  up = 0;
  down = 0;
  ok = 0;
  back = 0;
  pointer = 2;


  Serial.printf("Подключение к Wi-Fi сети %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" Модуль успешно подключен к Wi-Fi сети. ");
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
    if (pointer > numProfilesPointer) pointer = numProfilesPointer;
  }

  if (ok == 1) {

    func();
  }

  if (back == 1) {
    switch (pointer) {
      case 2: delete0(); break;
      case 3: delete1(); break;
      case 4: delete2(); break;
      case 5: delete3(); break;
      case 6: delete4(); break;
    }
  }


  if (numProfilesPointer > 0) {
    if (flag == 1) {
      oled.home();
      oled.clear(1, 64, 6, 15);
      printPointer(pointer);

      oled.update();
    }
  } else {
    oled.home();
    oled.print(F("Соеденение с сервером"));
    oled.update();
  }



  if (millis() - timer > 10000) {
    timer = millis();
    wificonnectdata();
    printMenu();
  }
}


void wificonnectdata() {
  WiFiClient client;

  Serial.printf("\n[Connecting to %s ... ", host);
  if (client.connect(host, 80)) {
    Serial.println("connected]");
    client.print("GET /api/get_profile_list");
    client.println(" HTTP/1.1");
    client.print("Host: ");
    client.println(host);
    client.println("Connection: close");
    client.println();
    client.flush();


    delay(200);

    while (client.available()) {
      String jsonString = client.readStringUntil('\n');
      Serial.println(jsonString);

      parseJSON(jsonString);
    }

    client.stop();
    client.flush();
    delay(50);
  } else {
    client.stop();
    client.flush();
    Serial.println("Ошибка подключения");
    delay(1000);
  }

  Serial.println("Остановка подключения");
}


void parseJSON(String jsonString) {
  // Размер буфера должен соответствовать размеру JSON пакета
  const size_t bufferSize = JSON_OBJECT_SIZE(2) + JSON_ARRAY_SIZE(3) + 560;

  // Создаем буфер для хранения JSON данных
  DynamicJsonDocument jsonBuffer(bufferSize);

  // Разбираем JSON из строки
  DeserializationError error = deserializeJson(jsonBuffer, jsonString);

  // Проверяем наличие ошибок при разборе JSON
  if (error) {
    Serial.print("Ошибка разбора JSON: ");
    Serial.println(error.c_str());
    return;
  }

  // Получаем значение простой записи
  String message = jsonBuffer["message"];

  messagemenu = message;


  // Получаем массив записей
  JsonArray profileListArray = jsonBuffer["profile_list"];

  // Объявляем динамические массивы для хранения значений записей
  int numProfiles = profileListArray.size();
  int* ids = new int[numProfiles];
  int* startTimes = new int[numProfiles];
  int* endTimes = new int[numProfiles];


  // Читаем значения записей из массива
  for (int i = 0; i < numProfiles; i++) {
    ids[i] = profileListArray[i]["id"];
    startTimes[i] = profileListArray[i]["start_time"];
    endTimes[i] = profileListArray[i]["end_time"];
  }



  for (int i = 0; i < 5; i++) {

    idmenu[i] = ids[i];
    startTimemenu[i] = startTimes[i];
    endTimemenu[i] = endTimes[i];
  }


  numProfilesPointer = numProfiles + 1;
  numProfilesmenu = numProfiles;

  // Освобождаем память, выделенную под динамические массивы
  delete[] ids;
  delete[] startTimes;
  delete[] endTimes;
}



void printMenu() {

  flag = 1;

  Serial.print("Сообщение: ");
  Serial.println(messagemenu);

  Serial.print("numProfilesPointer :  ");
  Serial.println(numProfilesPointer);

  Serial.print("numProfilesmenu :  ");
  Serial.println(numProfilesmenu);

  Serial.print("numProfiles :  ");
  Serial.println(numProfiles);


  Serial.print("ID - ");
  Serial.println(idmenu[0]);
  Serial.print("startTimemenu - ");
  Serial.println(startTimemenu[0]);
  Serial.print("endTimemenu - ");
  Serial.println(endTimemenu[0]);





  for (int i = 0; i < numProfilesmenu; i++) {
    Serial.print("Профиль ");
    Serial.print(i + 1);
    Serial.print(": ID=");
    Serial.print(idmenu[i]);
    Serial.print(", Время начала=");
    Serial.print(startTimemenu[i]);
    Serial.print(", Время окончания=");
    Serial.println(endTimemenu[i]);
    Serial.println(i);
  }




  oled.clear();
  oled.update();
  for (int i = 0; i < numProfilesmenu; i++) {

    oled.home();
    oled.print(F("Выберите профиль"));
    oled.setCursor(0, 1);
    oled.print(messagemenu);
    oled.setCursor(8, i + 2);
    oled.print("Профиль  ");
    oled.print(i + 1);
    oled.print(": ID = ");
    oled.println(idmenu[i]);
    oled.update();
  }
}


void func(void) {

  flag = 0;

  oled.clear();
  oled.update();
  oled.home();
  oled.setCursor(10, 0);
  oled.print(F("Press # to return"));
  oled.setCursor(10, 1);
  oled.print(F("Press * to save"));
  oled.setCursor(10, 3);
  oled.print(F("ADD profile?"));
  oled.setCursor(20, 5);
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

      if (key == '#') {
        printMenu();
        wificonnectdata();
        back = 2;

        return;
      }


      if (key == '*') {

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
                  printMenu();
                  wificonnectdata();
                  back = 2;
                  return;
                }
              }
            }

            if (key == '#') {
              printMenu();
              wificonnectdata();
              back = 2;
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



        Serial.print("часы:");
        Serial.println(int_hour);

        oled.setCursor(50, 4);
        oled.print(int_hour);

        oled.update();
      }
    }
  }
}

void delete0(void) {

  flag = 0;

  oled.clear();
  oled.update();
  oled.home();
  oled.setCursor(10, 0);
  oled.print(F("Press # to return"));
  oled.setCursor(10, 1);
  oled.print(F("Press * to save"));
  oled.setCursor(10, 3);
  oled.print(F("Delete  profile?"));
  oled.setCursor(20, 5);
  oled.print(F("ID - "));
  oled.setCursor(50, 5);
  oled.print(idmenu[pointer - 2]);
  oled.update();
  while (1) {
    char key = keypad.getKey();

    if (key != NO_KEY) {
      Serial.println(key);
    }
    if (key == '#') {
      printMenu();
      return;
    }
    if (key == '*') {
      remove_profile();

      while (1) {
        char key = keypad.getKey();

        if (key != NO_KEY) {
          Serial.println(key);
        }
        if (key == '#') {

          wificonnectdata();
          printMenu();
          return;
        }
      }
    }
  }
}

void delete1(void) {

  flag = 0;

  oled.clear();
  oled.update();
  oled.home();
  oled.setCursor(10, 0);
  oled.print(F("Press # to return"));
  oled.setCursor(10, 1);
  oled.print(F("Press * to save"));
  oled.setCursor(10, 3);
  oled.print(F("Delete  profile?"));
  oled.setCursor(20, 5);
  oled.print(F("ID - "));
  oled.setCursor(50, 5);
  oled.print(idmenu[pointer - 2]);
  oled.update();
  while (1) {
    char key = keypad.getKey();

    if (key != NO_KEY) {
      Serial.println(key);
    }
    if (key == '#') {
      printMenu();
      return;
    }
    if (key == '*') {
      remove_profile();

      while (1) {
        char key = keypad.getKey();

        if (key != NO_KEY) {
          Serial.println(key);
        }
        if (key == '#') {
          wificonnectdata();
          printMenu();
          return;
        }
      }
    }
  }
}

void delete2(void) {

  flag = 0;

  oled.clear();
  oled.update();
  oled.home();
  oled.setCursor(10, 0);
  oled.print(F("Press # to return"));
  oled.setCursor(10, 1);
  oled.print(F("Press * to save"));
  oled.setCursor(10, 3);
  oled.print(F("Delete  profile?"));
  oled.setCursor(20, 5);
  oled.print(F("ID - "));
  oled.setCursor(50, 5);
  oled.print(idmenu[pointer - 2]);
  oled.update();
  while (1) {
    char key = keypad.getKey();

    if (key != NO_KEY) {
      Serial.println(key);
    }
    if (key == '#') {
      printMenu();
      return;
    }
    if (key == '*') {
      remove_profile();

      while (1) {
        char key = keypad.getKey();

        if (key != NO_KEY) {
          Serial.println(key);
        }
        if (key == '#') {
          wificonnectdata();
          printMenu();
          return;
        }
      }
    }
  }
}

void delete3(void) {

  flag = 0;

  oled.clear();
  oled.update();
  oled.home();
  oled.setCursor(10, 0);
  oled.print(F("Press # to return"));
  oled.setCursor(10, 1);
  oled.print(F("Press * to save"));
  oled.setCursor(10, 3);
  oled.print(F("Delete  profile?"));
  oled.setCursor(20, 5);
  oled.print(F("ID - "));
  oled.setCursor(50, 5);
  oled.print(idmenu[pointer - 2]);
  oled.update();
  while (1) {
    char key = keypad.getKey();

    if (key != NO_KEY) {
      Serial.println(key);
    }
    if (key == '#') {
      printMenu();
      return;
    }
    if (key == '*') {
      remove_profile();

      while (1) {
        char key = keypad.getKey();

        if (key != NO_KEY) {
          Serial.println(key);
        }
        if (key == '#') {
          wificonnectdata();
          printMenu();
          return;
        }
      }
    }
  }
}

void delete4(void) {

  flag = 0;

  oled.clear();
  oled.update();
  oled.home();
  oled.setCursor(10, 0);
  oled.print(F("Press # to return"));
  oled.setCursor(10, 1);
  oled.print(F("Press * to save"));
  oled.setCursor(10, 3);
  oled.print(F("Delete  profile?"));
  oled.setCursor(20, 5);
  oled.print(F("ID - "));
  oled.setCursor(50, 5);
  oled.print(idmenu[pointer - 2]);
  oled.update();
  while (1) {
    char key = keypad.getKey();

    if (key != NO_KEY) {
      Serial.println(key);
    }
    if (key == '#') {
      printMenu();
      return;
    }
    if (key == '*') {
      remove_profile();

      while (1) {
        char key = keypad.getKey();

        if (key != NO_KEY) {
          Serial.println(key);
        }
        if (key == '#') {
          wificonnectdata();
          printMenu();
          return;
        }
      }
    }
  }
}

void printPointer(int pointer) {

  oled.setCursor(0, pointer);
  oled.print(">");
}

void remove_profile() {

  StaticJsonDocument<200> jsonDoc;
  jsonDoc["id"] = idmenu[pointer - 2];
  String jsonString;
  serializeJson(jsonDoc, jsonString);


  // Отправка DELETE-запроса на сервер
  HTTPClient http;
  http.begin(serverUrldelete);
  http.addHeader("Content-Type", "application/json");

  int httpResponseCode = http.sendRequest("DELETE", jsonString);

  if (httpResponseCode > 0) {
    String response = http.getString();
    Serial.println("Server response: " + response);



    oled.setCursor(10, 6);
    oled.print("Server response: ");
    oled.setCursor(10, 7);
    oled.print(response);
    oled.update();

  } else {
    Serial.println("Error sending DELETE request");
  }

  http.end();
  pointer = 2;
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

        case '*':
          ok = 1;
          Serial.println("ok");

          break;

        case '#':
          back = 1;
          Serial.println("back");

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