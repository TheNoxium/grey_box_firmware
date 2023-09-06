#include <WiFi.h>

const char* ssid = "PR1HOD1 NA BYH14";
const char* password = "88888888q";
const char* host = "192.168.0.110";

unsigned long timer;

#include "ArduinoJson.h"



void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.printf("Подключение к Wi-Fi сети %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" Модуль успешно подключен к Wi-Fi сети. ");
}

void loop() {

  if (millis() - timer > 20000) {
    timer = millis();
    wificonnectdata();
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

  // Выводим значения переменных
  Serial.print("Сообщение: ");
  Serial.println(message);
  for (int i = 0; i < numProfiles; i++) {
    Serial.print("Профиль ");
    Serial.print(i + 1);
    Serial.print(": ID=");
    Serial.print(ids[i]);
    Serial.print(", Время начала=");
    Serial.print(startTimes[i]);
    Serial.print(", Время окончания=");
    Serial.println(endTimes[i]);
  }

  // Освобождаем память, выделенную под динамические массивы
  delete[] ids;
  delete[] startTimes;
  delete[] endTimes;
}
