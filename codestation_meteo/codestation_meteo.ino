#include <ESP32Firebase.h>
#include <DHT.h>
#include <Wire.h>
#include <WiFi.h>
#include <Adafruit_BMP280.h>

#define DHT11_PIN 4
#define DHT22_PIN 5

#define _SSID "marwen"          // Your WiFi SSID
#define _PASSWORD "00000000"      // Your WiFi Password
#define REFERENCE_URL "https://stationmeteo-33707-default-rtdb.firebaseio.com/"  // Your Firebase project reference URL

DHT dht11(DHT11_PIN, DHT11);
DHT dht22(DHT22_PIN, DHT22);  // DHT22 sensor
Adafruit_BMP280 bmp;          // BMP280 sensor, Default I2C address is 0x76
Firebase firebase(REFERENCE_URL);

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(1000);

  // Connect to WiFi
  Serial.println();
  Serial.println();
  Serial.print("Connecting to: ");
  Serial.println(_SSID);
  WiFi.begin(_SSID, _PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("-");
  }

  Serial.println("");
  Serial.println("WiFi Connected");

  // Print the IP address
  Serial.print("IP Address: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");

  dht11.begin();
   dht22.begin();   // DHT22 initialization commented out

  // Initialize BMP280
   if (!bmp.begin(0x76,0x58)) {
     Serial.println("Could not find a valid BMP280 sensor, check wiring!");
     while (1);
  }
}

void loop() {
  delay(3000);

  float humidityDHT11 = dht11.readHumidity();
  float temperatureDHT11 = dht11.readTemperature();
   float humidityDHT22 = dht22.readHumidity();      // Read humidity from DHT22 sensor
   float temperatureDHT22 = dht22.readTemperature();  // Read temperature from DHT22 sensor

  // Read BMP280 data
    float pressure = bmp.readPressure() / 100.0F; // Convert pressure to hPa
   float temperatureBMP = bmp.readTemperature();   // Read temperature from BMP280 sensor
   float altitudee = bmp.readAltitude(1013.25);
  if (isnan(humidityDHT11) || isnan(temperatureDHT11)) {
    Serial.println("Failed to read DHT11 sensor.");
    return;
  }

  Serial.println("Reading DHT11 sensor:");
  Serial.print("DHT11 - Humidity: ");
  Serial.print(humidityDHT11);
  Serial.print("%, Temperature: ");
  Serial.print(temperatureDHT11);
  Serial.println("°C");

   Serial.println("Reading DHT22 sensor:");
   Serial.print("DHT22 - Humidity: ");
   Serial.print(humidityDHT22);
   Serial.print("%, Temperature: ");
   Serial.print(temperatureDHT22);
   Serial.println("°C");


  Serial.println("Reading BMP280 sensor:");
  Serial.print("BMP280 - Temperature: ");
   Serial.print(temperatureBMP);
   Serial.print("°C, Pressure: ");
   Serial.print(pressure);
   Serial.println("hPa");

  // Convert the float values to strings
  String tempStrDHT11 = String(temperatureDHT11);
  String humStrDHT11 = String(humidityDHT11);
   String tempStrDHT22 = String(temperatureDHT22);
   String humStrDHT22 = String(humidityDHT22);
   String tempStrBMP = String(temperatureBMP);
   String pressureStr = String(pressure);
   String altitu = String(altitudee);

  // Send data to Firebase
  firebase.setString("dht11/temp", tempStrDHT11);
  firebase.setString("dht11/hum", humStrDHT11);
   firebase.setString("dht22/temp", tempStrDHT22);
   firebase.setString("dht22/hum", humStrDHT22);
   firebase.setString("bmp280/temp", tempStrBMP);
   firebase.setString("bmp280/pressure", pressureStr);
   firebase.setString("bmp280/altitude", altitu);

   
}