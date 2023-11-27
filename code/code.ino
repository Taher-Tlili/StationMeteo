#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <WiFi.h>
#include <FirebaseESP32.h>
#include <DHT.h>

#define WIFI_SSID "TT_1D50"
#define WIFI_PASSWORD "1qqq8fsa7w"
#define FIREBASE_HOST "https://stationmeteo-33707-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "AIzaSyCHklwL_3OJccFoFtEzg093DlH9PhOPiYU"

#define DHTPIN 16
#define DHTTYPE DHT11
#define DHT22PIN 13 // 
DHT dht(DHTPIN, DHTTYPE);
DHT dht22(DHT22PIN, DHT22);  // Initialisez un nouveau capteur DHT22
Adafruit_BMP280 bmp;  // L'adresse I2C par défaut est 0x76

FirebaseData fbdo;

void setup() {
  Serial.begin(115200);

  // Connexion au réseau WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connexion au WiFi en cours...");
  }
  Serial.println("Connecté au WiFi");

  // Initialisez la connexion à Firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

  dht.begin();
  dht22.begin();

  // Initialisez le capteur BMP280 avec l'adresse I2C 0x76
  if (!bmp.begin(0x76)) {
    Serial.println("Impossible de trouver le capteur BMP280, vérifiez les connexions !");
    while (1);
  }
}

void loop() {
  float humidityDHT11 = dht.readHumidity();
  float temperatureDHT11 = dht.readTemperature();
  float humidityDHT22 = dht22.readHumidity();  // Lire l'humidité du DHT22
  float temperatureDHT22 = dht22.readTemperature();  // Lire la température du DHT22
  float pressure = bmp.readPressure() / 100.0F; // Convertir la pression en hPa
  float temperatureBMP = bmp.readTemperature();   // Lire la température du BMP280

  if (isnan(humidityDHT11) || isnan(temperatureDHT11) || isnan(humidityDHT22) || isnan(temperatureDHT22)) {
    Serial.println("Échec de la lecture des capteurs DHT11 ou DHT22.");
    return;
  }

  // Ajoute "C" après la température
  String temperatureWithUnitDHT11 = String(temperatureDHT11) + " C";
  String temperatureWithUnitDHT22 = String(temperatureDHT22) + " C";
  // Ajoute " C" après la température du BMP
  String temperatureWithUnitBMP = String(temperatureBMP) + " C";

  // Ajoute "%" après l'humidité
  String humidityWithUnitDHT11 = String(humidityDHT11) + "%";
  String humidityWithUnitDHT22 = String(humidityDHT22) + "%";

  // Ajoute "hPa" après la pression
  String pressureWithUnit = String(pressure) + " hPa";

  // Enregistrez les données dans Firebase avec les unités
  Firebase.setString(fbdo, "/dht11/temperature", temperatureWithUnitDHT11);
  Firebase.setString(fbdo, "/dht11/humidity", humidityWithUnitDHT11);
  Firebase.setString(fbdo, "/dht22/temperature", temperatureWithUnitDHT22);
  Firebase.setString(fbdo, "/dht22/humidity", humidityWithUnitDHT22);
  Firebase.setFloat(fbdo, "/bmp280/temperature", temperatureBMP);
  Firebase.setString(fbdo, "/bmp280/temperature", temperatureWithUnitBMP);
  Firebase.setString(fbdo, "/bmp280/pressure", pressureWithUnit);

  if (Firebase.setString(fbdo, "/dht11/temperature", temperatureWithUnitDHT11) &&
      Firebase.setString(fbdo, "/dht11/humidity", humidityWithUnitDHT11) &&
      Firebase.setString(fbdo, "/dht22/temperature", temperatureWithUnitDHT22) &&
      Firebase.setString(fbdo, "/dht22/humidity", humidityWithUnitDHT22) &&
      Firebase.setFloat(fbdo, "/bmp280/temperature", temperatureBMP) &&
      Firebase.setString(fbdo, "/bmp280/temperature", temperatureWithUnitBMP) &&
      Firebase.setString(fbdo, "/bmp280/pressure", pressureWithUnit)) {
    Serial.println("Données mises à jour avec succès dans Firebase !");
  } else {
    Serial.println("Échec de la mise à jour des données dans Firebase.");
  }

  delay(3000); // Envoyer les données toutes les 3 secondes
}