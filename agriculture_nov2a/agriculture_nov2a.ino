#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <ESP8266WiFi.h>

// Define pins
#define DHTPIN D4          // DHT11 pin
#define DHTTYPE DHT11
#define SOIL_MOISTURE_PIN A0  // Soil moisture sensor pin
#define RAIN_SENSOR_PIN D5    // Rain sensor pin
#define RELAY_MOTOR_PIN D6    // Relay for motor
#define RELAY_FAN_PIN D7      // Relay for fan

// OLED display settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// ThingSpeak settings
const char* ssid = "YOUR_WIFI_SSID";
const char* pass = "YOUR_WIFI_PASSWORD";
const char* server = "api.thingspeak.com";
String apiKey = "YOUR_THINGSPEAK_API_KEY";

// Initialize sensors
DHT dht(DHTPIN, DHTTYPE);
Adafruit_BMP280 bmp;  // Pressure sensor

WiFiClient client;

void setup() {
  Serial.begin(115200);

  // Initialize OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED display allocation failed!");
    for (;;);  // Halt if display fails
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Initializing...");
  display.display();

  // Initialize sensors and actuators
  pinMode(RELAY_MOTOR_PIN, OUTPUT);
  pinMode(RELAY_FAN_PIN, OUTPUT);
  pinMode(RAIN_SENSOR_PIN, INPUT);
  dht.begin();
  if (!bmp.begin(0x76)) {  // BMP280 I2C address
    Serial.println("Could not find a valid BMP280 sensor, check wiring!");
  }

  // Connect to WiFi
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("WiFi Connected!");
  display.display();
  delay(2000);
}

void loop() {
  // Read sensor data
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  int soilMoisture = analogRead(SOIL_MOISTURE_PIN);
  int rainStatus = digitalRead(RAIN_SENSOR_PIN);
  float pressure = bmp.readPressure() / 100.0F;  // Convert to hPa

  // Control motor based on soil moisture
  if (soilMoisture < 500) {  // Adjust threshold as needed
    digitalWrite(RELAY_MOTOR_PIN, LOW);  // Turn ON motor
  } else {
    digitalWrite(RELAY_MOTOR_PIN, HIGH);  // Turn OFF motor
  }

  // Control fan based on temperature
  if (temperature > 30) {  // Adjust threshold as needed
    digitalWrite(RELAY_FAN_PIN, LOW);  // Turn ON fan
  } else {
    digitalWrite(RELAY_FAN_PIN, HIGH);  // Turn OFF fan
  }

  // Display data on OLED
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Soil: " + String(soilMoisture) + " RH");
  display.println("Air: " + String(humidity) + " %");
  display.println("Temp: " + String(temperature) + " C");
  display.println("Press: " + String(pressure) + " hPa");
  display.println("Rain: " + String(rainStatus ? "No" : "Yes"));
  display.display();

  // Print data to Serial Monitor
  Serial.println("Soil Moisture: " + String(soilMoisture));
  Serial.println("Humidity: " + String(humidity) + " %");
  Serial.println("Temperature: " + String(temperature) + " C");
  Serial.println("Pressure: " + String(pressure) + " hPa");
  Serial.println("Rain Status: " + String(rainStatus ? "No Rain" : "Rain Detected"));
  Serial.println("-------------------");

  // Send data to ThingSpeak
  if (client.connect(server, 80)) {
    String postStr = apiKey;
    postStr += "&field1=" + String(soilMoisture);
    postStr += "&field2=" + String(humidity);
    postStr += "&field3=" + String(temperature);
    postStr += "&field4=" + String(pressure);
    postStr += "&field5=" + String(rainStatus);
    postStr += "\r\n\r\n";

    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);
  }
  client.stop();

  delay(20000);  // Wait 20 seconds before next update
}
