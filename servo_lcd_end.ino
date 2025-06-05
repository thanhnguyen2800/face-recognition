#include <WiFi.h>
#include <ESP32Servo.h>
#include <LiquidCrystal_I2C.h>
#include <AsyncTCP.h>

// Thông tin Wi-Fi
const char* ssid = "Redmi 13C";
const char* password = "tinh030892";

// Địa chỉ IP và cổng của server TCP
const char* host = "192.168.89.246";
const uint16_t port = 8584;

// Đối tượng AsyncTCPClient
AsyncClient *tcpClient;

// Đối tượng LCD I2C
LiquidCrystal_I2C lcd(0x27, 16, 4);

// Servo
Servo servo;
int servoPin = 18;
int servoAngle = 0;
bool servoOn = false;

// Web Server
WiFiServer webServer(80);

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Ket noi toi WiFi...");
  }

  Serial.println("Da ket noi toi WiFi");
  Serial.print("Dia chi IP cua ESP32: ");
  Serial.println(WiFi.localIP());

  servo.attach(servoPin);
  servo.write(servoAngle);

  webServer.begin();
  Serial.println("Truy cap trinh duyet va dieu khien servo.");

  lcd.init();
  lcd.backlight();

  delay(5000);

  // Khởi tạo AsyncTCPClient
  tcpClient = new AsyncClient();
  tcpClient->onConnect(onTcpConnect);
  tcpClient->onDisconnect(onTcpDisconnect);
  tcpClient->onData(onTcpData);
  tcpClient->connect(host, port);
}

void loop() {
  // Xử lý Web Server
  WiFiClient webClient = webServer.available();
  if (webClient) {
    String request = webClient.readStringUntil('\r');
    Serial.println(request);

    if (request.indexOf("/on") != -1) {
      servoAngle = 90;
      servo.write(servoAngle);
      servoOn = true;
    }
    if (request.indexOf("/off") != -1) {
      servoAngle = 0;
      servo.write(servoAngle);
      servoOn = false;
    }

    webClient.println("HTTP/1.1 200 OK");
    webClient.println("Content-Type: text/html");
    webClient.println();
    webClient.println("<html><body>");
    webClient.println("<h1>Dieu Khien Thiet Bi</h1>");
    webClient.println("<p><a href='/on'><button>Bat Servo</button></a></p>");
    webClient.println("<p><a href='/off'><button>Tat Servo</button></a></p>");

    if (servoOn) {
      webClient.println("<p>Servo: Da bat</p>");
    } else {
      webClient.println("<p>Servo: Da tat</p>");
    }

    webClient.println("</body></html>");
    webClient.stop();
  }
}

void onTcpConnect(void *arg, AsyncClient *client) {
  Serial.println("Connected to server");
}

void onTcpDisconnect(void *arg, AsyncClient *client) {
  Serial.println("Disconnected from server");
  client->connect(host, port); // Tự động kết nối lại
}

void onTcpData(void *arg, AsyncClient *client, void *data, size_t len) {
  String response = String((char *)data, len);
  response.trim(); // Loại bỏ khoảng trắng dư thừa

  Serial.println("Received: " + response);

  // Tách chuỗi thành các từ ngăn bởi dấu cách
  String words[100];
  int wordCount = 0;
  int startIdx = 0;
  
  // Tách từ từ chuỗi
  for (int i = 0; i < response.length(); i++) {
    if (response.charAt(i) == ' ' || i == response.length() - 1) {
      if (i == response.length() - 1) i++; // Để lấy từ cuối cùng nếu không có khoảng trắng sau cùng
      words[wordCount++] = response.substring(startIdx, i);
      startIdx = i + 1;
    }
  }

  // Hiển thị các từ trên màn hình LCD
  lcd.clear();
  lcd.setCursor(0, 0);  // Hàng 1
  String displayLine1 = "";
  for (int i = 0; i < wordCount - 3; i++) {
    displayLine1 += words[i] + " "; // Nối các từ lại với nhau
  }
  lcd.print(displayLine1);

  // Hiển thị 3 từ cuối trên hàng 3
  lcd.setCursor(0, 2);  // Hàng 3
  String displayLine3 = "";
  for (int i = wordCount - 3; i < wordCount; i++) {
    displayLine3 += words[i] + " "; // Nối 3 từ cuối
  }
  lcd.print(displayLine3);
}