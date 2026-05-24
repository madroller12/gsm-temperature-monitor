#include <SoftwareSerial.h>                     // Библиотека програмной реализации обмена по UART-протоколу
SoftwareSerial SIM800(2, 3);                    // RX, TX
int temp;
float grad;

String _response = "";                          // Переменная для хранения ответа модуля
void setup() {
  temp = analogRead(A0);
  grad = ( temp/1023.0 )*5.0*1000/10;
  Serial.begin(9600);                           // Скорость обмена данными с компьютером
  SIM800.begin(9600);                           // Скорость обмена данными с модемом
  Serial.println("Start!");

  sendATCommand("AT", true);                    // Отправили AT для настройки скорости обмена данными
  pinMode(A0, INPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(12, OUTPUT);
  digitalWrite(12, HIGH);
  sendATCommand("AT+CMGF=1", true);
  sendATCommand("AT+CMGS=\"+380950352591\"", true);
  sendATCommand("Start Arduino!", true);
  SIM800.write(26);

  // Команды настройки модема при каждом запуске
  //_response = sendATCommand("AT+CLIP=1", true);  // Включаем АОН
  //_response = sendATCommand("AT+DDET=1", true);  // Включаем DTMF
}

String sendATCommand(String cmd, bool waiting) {
  String _resp = "";                            // Переменная для хранения результата
  Serial.println(cmd);                          // Дублируем команду в монитор порта
  SIM800.println(cmd);                          // Отправляем команду модулю
  if (waiting) {                                // Если необходимо дождаться ответа...
    _resp = waitResponse();                     // ... ждем, когда будет передан ответ
    // Если Echo Mode выключен (ATE0), то эти 3 строки можно закомментировать
    if (_resp.startsWith(cmd)) {                // Убираем из ответа дублирующуюся команду
      _resp = _resp.substring(_resp.indexOf("\r", cmd.length()) + 2);
    }
    Serial.println(_resp);                      // Дублируем ответ в монитор порта
  }
  return _resp;                                 // Возвращаем результат. Пусто, если проблема
}

String waitResponse() {                         // Функция ожидания ответа и возврата полученного результата
  String _resp = "";                            // Переменная для хранения результата
  long _timeout = millis() + 10000;             // Переменная для отслеживания таймаута (10 секунд)
  while (!SIM800.available() && millis() < _timeout)  {}; // Ждем ответа 10 секунд, если пришел ответ или наступил таймаут, то...
  if (SIM800.available()) {                     // Если есть, что считывать...
    _resp = SIM800.readString();                // ... считываем и запоминаем
  }
  else {                                        // Если пришел таймаут, то...
    Serial.println("Timeout...");               // ... оповещаем об этом и...
  }
  return _resp;                                 // ... возвращаем результат. Пусто, если проблема
}

void loop() {
  if (SIM800.available())   {                   // Если модем, что-то отправил...
    _response = waitResponse();                 // Получаем ответ от модема для анализа
    _response.trim();                           // Убираем лишние пробелы в начале и конце
    Serial.println(_response);                  // Если нужно выводим в монитор порта
    if (_response.startsWith("RING")) {         // Есть входящий вызов
      sendATCommand("ATH", true);
      sendATCommand("AT+CMGF=1", true);
      sendATCommand("AT+CMGS=\"+380950352591\"", true);
      SIM800.print(grad);
      sendATCommand(" *C", true);
      SIM800.write(26);
      teMp();
    }
  }
  if (Serial.available())  {                    // Ожидаем команды по Serial...
    SIM800.write(Serial.read());                // ...и отправляем полученную команду модему
  };
}

void teMp() {
  digitalWrite(12, LOW);
  digitalWrite(9, LOW);
  digitalWrite(8, LOW);
  while (grad <= 25) {
    temp = analogRead(A0);
    grad = ( temp/1023.0 )*5.0*1000/10;
    Serial.println(grad);
    digitalWrite(9, HIGH);
    delay(1000);
    digitalWrite(9, LOW);
    delay(300);
  }
  digitalWrite(8, HIGH);
  sendATCommand("AT+CMGF=1", true);
  sendATCommand("AT+CMGS=\"+380950352591\"", true);
  SIM800.print(grad);
  sendATCommand(" *C", true);
  SIM800.write(26);
  delay(3000);
  digitalWrite(8, LOW);
}
