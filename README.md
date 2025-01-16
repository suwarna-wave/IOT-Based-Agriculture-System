# IoT-Based Smart Agriculture System

## Overview
This project is an **IoT-based Smart Agriculture System** designed to monitor and control environmental conditions for optimal crop growth. It integrates multiple sensors and actuators to measure soil moisture, temperature, humidity, atmospheric pressure, and rain status. The system uses a **NodeMCU (ESP8266)** for WiFi connectivity, an **OLED display** for local data visualization, and **ThingSpeak** for cloud-based data logging and monitoring.

---

## Components
1. **NodeMCU (ESP8266)**: Microcontroller with built-in WiFi for IoT connectivity.
2. **Soil Moisture Sensor**: Measures the moisture level in the soil.
3. **Motor/Water Pump**: Regulates soil moisture by pumping water when needed.
4. **DHT11 Sensor**: Measures temperature and humidity.
5. **BMP280 Sensor**: Measures atmospheric pressure.
6. **Rain Sensor**: Detects rainfall.
7. **Fan**: Regulates temperature by cooling the environment.
8. **Relay Module**: Controls the motor and fan.
9. **OLED Display (SSD1306)**: Displays real-time sensor data locally.
10. **Breadboard and Jumper Wires**: For connecting components.
11. **Power Supply**: 5V/3.3V power supply for all components.

---

## Working of Sensors

### 1. **Soil Moisture Sensor**
- **Function**: Measures the volumetric water content in the soil.
- **Working**: The sensor outputs an analog voltage proportional to the soil moisture level. Dry soil has higher resistance, while wet soil has lower resistance.
- **Usage**: The motor is turned ON/OFF based on the soil moisture level.

### 2. **DHT11 Sensor**
- **Function**: Measures temperature and humidity.
- **Working**: The sensor uses a capacitive humidity sensor and a thermistor to measure the surrounding air.
- **Usage**: The fan is controlled based on the temperature reading.

### 3. **BMP280 Sensor**
- **Function**: Measures atmospheric pressure.
- **Working**: The sensor uses a piezoresistive pressure sensor to measure pressure.
- **Usage**: Pressure data is logged for environmental monitoring.

### 4. **Rain Sensor**
- **Function**: Detects rainfall.
- **Working**: The sensor outputs a digital signal (HIGH/LOW) based on the presence of rain.
- **Usage**: Rain status is logged for weather monitoring.

### 5. **OLED Display (SSD1306)**
- **Function**: Displays real-time sensor data.
- **Working**: The display uses I2C communication to show text and graphics.
- **Usage**: Provides local visualization of sensor data.

---

## System Flow
1. **Sensor Data Collection**:
   - The NodeMCU reads data from all sensors (soil moisture, DHT11, BMP280, rain sensor).
2. **Data Processing**:
   - The microcontroller processes the sensor data and makes decisions (e.g., turning ON/OFF the motor and fan).
3. **Local Display**:
   - Real-time data is displayed on the OLED display.
4. **Cloud Logging**:
   - Sensor data is sent to ThingSpeak for remote monitoring and visualization.
5. **Actuator Control**:
   - The motor and fan are controlled based on soil moisture and temperature readings.

---

## Setup Instructions

### Hardware Setup
1. **Connect the Soil Moisture Sensor**:
   - Connect the VCC pin to 3.3V, GND to GND, and the analog output to pin A0.
2. **Connect the DHT11 Sensor**:
   - Connect the VCC pin to 3.3V, GND to GND, and the data pin to D4.
3. **Connect the BMP280 Sensor**:
   - Connect the VCC pin to 3.3V, GND to GND, SDA to D2, and SCL to D1.
4. **Connect the Rain Sensor**:
   - Connect the VCC pin to 3.3V, GND to GND, and the digital output to D5.
5. **Connect the Relay Module**:
   - Connect the VCC pin to 5V, GND to GND, and the control pins to D6 (motor) and D7 (fan).
6. **Connect the OLED Display**:
   - Connect the VCC pin to 3.3V, GND to GND, SDA to D2, and SCL to D1.
7. **Power Supply**:
   - Use a 5V/2A power supply to power the NodeMCU and all components.

### Software Setup
1. **Install Arduino IDE**:
   - Download and install the Arduino IDE from [arduino.cc](https://www.arduino.cc/).
2. **Install Required Libraries**:
   - Install the following libraries via the Arduino Library Manager:
     - `DHT` (for DHT11 sensor)
     - `Adafruit BMP280` (for BMP280 sensor)
     - `Adafruit SSD1306` (for OLED display)
     - `Adafruit GFX` (for OLED display graphics)
3. **Upload the Code**:
   - Open the provided code in the Arduino IDE.
   - Replace `YOUR_WIFI_SSID`, `YOUR_WIFI_PASSWORD`, and `YOUR_THINGSPEAK_API_KEY` with your credentials.
   - Select the NodeMCU board and upload the code.

### ThingSpeak Setup
1. **Create a ThingSpeak Account**:
   - Sign up at [ThingSpeak](https://thingspeak.com/).
2. **Create a New Channel**:
   - Add fields for soil moisture, humidity, temperature, pressure, and rain status.
3. **Note the API Key**:
   - Copy the Write API Key and paste it into the code.

---

## Power Management
1. **Power Supply**:
   - Use a **5V/2A power supply** to ensure sufficient power for all components.
2. **Voltage Regulation**:
   - Use a **voltage regulator (e.g., LM7805)** if your power supply voltage is higher than 5V.
3. **Current Consumption**:
   - Ensure the total current consumption of all components does not exceed the power supply's capacity.

---

## Code Explanation
The code integrates all sensors and actuators, reads sensor data, controls the motor and fan, displays data on the OLED, and sends data to ThingSpeak. Key functions include:
- `setup()`: Initializes sensors, actuators, and WiFi.
- `loop()`: Reads sensor data, controls actuators, updates the OLED display, and sends data to ThingSpeak.

---

## Data Visualization
1. **Local Display**:
   - The OLED display shows real-time sensor data.
2. **ThingSpeak**:
   - Sensor data is logged and visualized on ThingSpeak for remote monitoring.

---

## License
This project is open-source and available under the MIT License.

---

## Troubleshooting
1. **WiFi Connection Issues**:
   - Ensure the correct SSID and password are entered.
   - Check the WiFi signal strength.
2. **Sensor Reading Errors**:
   - Verify the sensor connections.
   - Ensure the sensors are powered correctly.
3. **ThingSpeak Data Logging Issues**:
   - Check the API key and internet connection.

---

## Future Enhancements
1. **Add Soil pH Sensor**: Monitor soil pH for better crop management.
2. **Implement MQTT**: Use MQTT for more efficient IoT communication.
3. **Add Solar Power**: Use a solar panel for sustainable power supply.

---

## References
- [Arduino IDE](https://www.arduino.cc/)
- [ThingSpeak](https://thingspeak.com/)
- [Adafruit Libraries](https://github.com/adafruit)

---

## Author
Suwarna Pyakurel 
suwarna.079bei@ioepc.edu.np
