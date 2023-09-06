#include <ArduinoJson.h>

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



void setup() {
  Serial.begin(9600);

  String jsonString = "{\"message\":\"get_profile_list received successfully\",\"profile_list\":[{\"id\":1,\"start_time\":1000,\"end_time\":2000},{\"id\":2,\"start_time\":1000,\"end_time\":2000},{\"id\":3,\"start_time\":1000,\"end_time\":2000}]}";

  parseJSON(jsonString);
}

void loop() {
  // Ваш код здесь
}