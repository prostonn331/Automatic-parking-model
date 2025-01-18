#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <FS.h>
#include <IPAddress.h>

// Настройки точки доступа
const char* ssid = "ESP8266_AP";
const char* password = "12345678";

// Настройки IP-адреса
IPAddress local_IP(192, 168, 4, 1);
IPAddress gateway(192, 168, 4, 1);
IPAddress subnet(255, 255, 255, 0);

// Переменные для управления
bool forwardFlag = false;
bool backwardFlag = false;
bool stopFlag = true;
bool parkFlag = false;

// Создаем веб-сервер на порту 80
ESP8266WebServer server(80);

// HTML страница
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <title>Car Control</title>
  <style>
    body {
      background-color: aqua;
      text-align: center;
      font-family: Arial, sans-serif;
    }
    img {
      width: 300px;
      margin-top: 20px;
    }
    button {
      display: block;
      margin: 20px auto;
      padding: 30px 60px;
      font-size: 40px;
      cursor: pointer;
    }
    h1 {
      font-size: 50px;
      margin-top: 20px;
    }
  </style>
</head>
<body>
  <h1>Управление машиной</h1>
  <img src="/car.jpg" alt="Car">
  <div>
    <button onclick="sendRequest('forward')">Вперед</button>
    <button onclick="sendRequest('backward')">Назад</button>
    <button onclick="sendRequest('stop')">Стоп</button>
    <button onclick="sendRequest('park')">Парковка</button>
  </div>
  <script>
    function sendRequest(action) {
      fetch('/' + action).then(response => {
        if (response.ok) {
          console.log(action + ' sent');
        } else {
          console.error('Error sending ' + action);
        }
      });
    }
  </script>
</body>
</html>
)rawliteral";

void handleRoot() {
  server.send_P(200, "text/html", index_html);
}

void handleImage() {
  File file = SPIFFS.open("/car.jpg", "r");
  if (!file) {
    server.send(404, "text/plain", "Image not found");
    return;
  }
  server.streamFile(file, "image/jpeg");
  file.close();
}

void handleForward() {
  forwardFlag = true;
  backwardFlag = false;
  stopFlag = false;
  parkFlag = false;
  server.send(200, "text/plain", "Forward command received");
}

void handleBackward() {
  forwardFlag = false;
  backwardFlag = true;
  stopFlag = false;
  parkFlag = false;
  server.send(200, "text/plain", "Backward command received");
}

void handleStop() {
  forwardFlag = false;
  backwardFlag = false;
  stopFlag = true;
  parkFlag = false;
  server.send(200, "text/plain", "Stop command received");
}

void handlePark() {
  forwardFlag = false;
  backwardFlag = false;
  stopFlag = false;
  parkFlag = true;
  server.send(200, "text/plain", "Park command received");
}

void setup() {
  Serial.begin(115200);

  // Инициализация файловой системы
  if (!SPIFFS.begin()) {
    Serial.println("Failed to mount file system");
    return;
  }

  // Настройка точки доступа с фиксированным IP-адресом
  if (!WiFi.softAPConfig(local_IP, gateway, subnet)) {
    Serial.println("Failed to configure AP");
  }
  WiFi.softAP(ssid, password);
  Serial.println("Access Point started");
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());

  // Настройка обработчиков запросов
  server.on("/", handleRoot);
  server.on("/car.jpg", handleImage);
  server.on("/forward", handleForward);
  server.on("/backward", handleBackward);
  server.on("/stop", handleStop);
  server.on("/park", handlePark);

  // Запуск сервера
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();

  // Вывод текущих состояний для отладки
  Serial.print("Forward: ");
  Serial.print(forwardFlag);
  Serial.print(" | Backward: ");
  Serial.print(backwardFlag);
  Serial.print(" | Stop: ");
  Serial.print(stopFlag);
  Serial.print(" | Park: ");
  Serial.println(parkFlag);
  delay(1000);
}
