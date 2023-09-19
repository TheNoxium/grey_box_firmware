#include <Arduino.h>

int hours = 2;    // Пример: 2 часа
int minutes = 30; // Пример: 30 минут
int seconds = 45; // Пример: 45 секунд

void setup() {
  Serial.begin(115200);
  
  // Переводим время в секунды
  int totalSeconds = (hours * 3600) + (minutes * 60) + seconds;
  
  Serial.print("Total seconds: ");
  Serial.println(totalSeconds);

  unsigned long seconds = 12345;

// Вычисляем количество часов, минут и секунд
int hours = seconds / 3600;
int minutes = (seconds % 3600) / 60;
int secs = seconds % 60;

// Выводим результат
Serial.print("Время: ");
Serial.print(hours);
Serial.print(":");
if (minutes < 10) {
  Serial.print("0");
}
Serial.print(minutes);
Serial.print(":");
if (secs < 10) {
  Serial.print("0");
}
Serial.println(secs);
}

void loop() {
  // Ваш код здесь
}
