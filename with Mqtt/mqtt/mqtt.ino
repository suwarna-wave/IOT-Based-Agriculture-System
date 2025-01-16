#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>  // MQTT library

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

// WiFi and MQTT settings
const char* ssid = "YOUR_WIFI_SSID";       // Replace with your WiFi SSID
const char* pass = "YOUR_WIFI_PASSWORD";   // Replace with your WiFi password
const char* mqtt_server = "broker.hivemq.com";  // Public MQTT broker
const int mqtt_port = 1883;  // Default MQTT port
const char* mqtt_user = "";  // Leave blank for public brokers
const char* mqtt_pass = "";  // Leave blank for public brokers

// MQTT topics
const char* soil_moisture_topic = "smart_agri/soil_moisture";
const char* humidity_topic = "smart_agri/humidity";
const char* temperature_topic = "smart_agri/temperature";
const char* pressure_topic = "smart_agri/pressure";
const char* rain_topic = "smart_agri/rain";
const char* motor_status_topic = "smart_agri/motor_status";
const char* fan_status_topic = "smart_agri/fan_status";

// Initialize sensors
DHT dht(DHTPIN, DHTTYPE);
Adafruit_BMP280 bmp;  // Pressure sensor

WiFiClient espClient;
PubSubClient client(espClient);

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
  digitalWrite(RELAY_MOTOR_PIN, HIGH);  // Ensure motor is OFF initially
  digitalWrite(RELAY_FAN_PIN, HIGH);    // Ensure fan is OFF initially
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

  // Configure MQTT server
  client.setServer(mqtt_server, mqtt_port);
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP8266Client", mqtt_user, mqtt_pass)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Read sensor data
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  int soilMoisture = analogRead(SOIL_MOISTURE_PIN);
  int rainStatus = digitalRead(RAIN_SENSOR_PIN);
  float pressure = bmp.readPressure() / 100.0F;  // Convert to hPa

  // Control motor based on soil moisture
  if (soilMoisture < 500) {  // Adjust threshold as needed
    digitalWrite(RELAY_MOTOR_PIN, LOW);  // Turn ON motor
    client.publish(motor_status_topic, "ON");
  } else {
    digitalWrite(RELAY_MOTOR_PIN, HIGH);  // Turn OFF motor
    client.publish(motor_status_topic, "OFF");
  }

  // Control fan based on temperature
  if (temperature > 30) {  // Adjust threshold as needed
    digitalWrite(RELAY_FAN_PIN, LOW);  // Turn ON fan
    client.publish(fan_status_topic, "ON");
  } else {
    digitalWrite(RELAY_FAN_PIN, HIGH);  // Turn OFF fan
    client.publish(fan_status_topic, "OFF");
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

  // Publish data to MQTT topics
  client.publish(soil_moisture_topic, String(soilMoisture).c_str());
  client.publish(humidity_topic, String(humidity).c_str());
  client.publish(temperature_topic, String(temperature).c_str());
  client.publish(pressure_topic, String(pressure).c_str());
  client.publish(rain_topic, String(rainStatus).c_str());

  delay(20000);  // Wait 20 seconds before next update
}
