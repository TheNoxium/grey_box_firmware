#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "PR1HOD1 NA BYH14";
const char* password = "88888888q";
const char* serverUrl = "http://192.168.0.110:80/api/remove_profile";


#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>


#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const int jsonCapacity = JSON_OBJECT_SIZE(2);

void setup() {
  Serial.begin(115200);

  // Подключение к Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Создание JSON-объекта
  StaticJsonDocument<jsonCapacity> jsonDoc;
  jsonDoc["id"] = 6;
  

  // Преобразование JSON-объекта в строку
  String jsonString;
  serializeJson(jsonDoc, jsonString);

  // Отправка DELETE-запроса на сервер
  HTTPClient http;
  http.begin(serverUrl);
  http.addHeader("Content-Type", "application/json");

  int httpResponseCode = http.sendRequest("DELETE", jsonString);

  if (httpResponseCode > 0) {
    String response = http.getString();
    Serial.println("Server response: " + response);
  } else {
    Serial.println("Error sending DELETE request");
  }

  http.end();
}

void loop() {
  // Ваш код здесь
}