#include <Arduino.h>
#include <WiFi.h>
#include <Adafruit_Sensor.h>
#include "DHT.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include <FirebaseESP32.h>

#define DHT1_PIN 33
#define DHT2_PIN 32
#define DHT_TYPE DHT11
#define ONE_WIRE_BUS 21
#define FAN_PIN 23
#define FAN_PUMP_PIN 22 

#define DELAY_BETWEEN_MEASUREMENTS 15000

#define FIREBASE_HOST "water-harvester.firebaseio.com" //Without http:// or https:// schemes
#define FIREBASE_AUTH "2tMAiCMNGGDRgGUAHpaPCOxcmpzB2UKG8LREjzMj"
#define WIFI_SSID "iPhone van Mike"
#define WIFI_PASSWORD "Mike1234"
#define DATAPATH "water-harvester"

DHT dht1(DHT1_PIN, DHT_TYPE);
DHT dht2(DHT2_PIN, DHT_TYPE);
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

unsigned long sendDataPrevMillis;

float t1;
float h1;
float t2;
float h2;
float twp;

int fanOn;

FirebaseData firebaseData;

// Prototypes
void measureDHTValues();
void readFirebase();
void sendFloat(float, String);

void setup() {
  // Setup serial connection
  Serial.begin(9600);

  // Set values to 0;
  sendDataPrevMillis = 0;
  t1 = 0;
  h1 = 0;
  t2 = 0;
  h2 = 0;
  twp = 0;
  fanOn = 0;

  // Start both dht sensors
  dht1.begin();
  dht2.begin();

  // Set pin FAN_PIN to output and set it to LOW. This pin is active HIGH
  pinMode(FAN_PIN, OUTPUT);
  digitalWrite(FAN_PIN, fanOn);
  
  // Set pin FAN_PUMP_PIN to output and set it to LOW. This pin is active LOW
  pinMode(FAN_PUMP_PIN, OUTPUT);
  digitalWrite(FAN_PUMP_PIN, LOW);

  // Open Wifi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");

  // Wait till connected
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }

  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  // Open Firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

  if (!Firebase.beginStream(firebaseData, "/")) {
    Serial.println("------------------------------------");
    Serial.println("Can't begin stream connection...");
    Serial.println("REASON: " + firebaseData.errorReason());
    Serial.println("------------------------------------");
    Serial.println();
  }
}

void loop() {
  measureDHTValues();
  readFirebase();

  digitalWrite(FAN_PIN, fanOn);
}

void readFirebase() {
  if (!Firebase.readStream(firebaseData)) {
    Serial.println("------------------------------------");
    Serial.println("Can't read stream data...");
    Serial.println("REASON: " + firebaseData.errorReason());
    Serial.println("------------------------------------");
    Serial.println();
  }

  if (firebaseData.streamTimeout()) {
    Serial.println("Stream timeout, resume streaming...");
    Serial.println();
  }

  if (firebaseData.streamAvailable()) {
    if (firebaseData.dataType() == "int") {
      Serial.println(firebaseData.intData());
      Serial.println(firebaseData.dataPath());

      String path = firebaseData.dataPath() ;
      String equalPath = "/onOff";

      if (path.compareTo(equalPath) == 0) fanOn = firebaseData.intData() >= 1 ? 1 : 0;
    }

    else if (firebaseData.dataType() == "float") {
      Serial.println(firebaseData.floatData(), 5);
    }

    else if (firebaseData.dataType() == "double") { 
      printf("%.9lf\n", firebaseData.doubleData());
    }

    else if (firebaseData.dataType() == "boolean") {
      Serial.println(firebaseData.boolData() == 1 ? "true" : "false");
    }

    else if (firebaseData.dataType() == "string") {
      Serial.println(firebaseData.stringData());
    }

    else if (firebaseData.dataType() == "json") {
      FirebaseJson &json = firebaseData.jsonObject();
      String jsonStr;
      json.toString(jsonStr, true);
      size_t len = json.iteratorBegin();
      String key, value = "";
      String onOff = "onOff";
      String onValue = "1";
      int type = 0;
      
      for (size_t i = 0; i < len; i++) {
        json.iteratorGet(i, type, key, value);

        if (type == JSON_OBJECT) {
          if (key.compareTo(onOff) == 0) {
            Serial.print(", Value: ");
            Serial.println(value);

            if (value.compareTo(onValue) == 0) {
              fanOn = 1;
            }
            else {
              fanOn = 0;
            }
          }
        }
      }
      json.iteratorEnd();
    }
  }
}

void measureDHTValues() {
  if (millis() - sendDataPrevMillis < DELAY_BETWEEN_MEASUREMENTS) return;

  sendDataPrevMillis = millis(); 

  h1 = dht1.readHumidity();
  t1 = dht1.readTemperature();

  h2 = dht2.readHumidity();
  t2 = dht2.readTemperature();

  sensors.requestTemperatures(); 
  twp = sensors.getTempCByIndex(0);
  Serial.println((String) t1 + "|" + h1 + "|" + t2 + "|" + h2 + "|" + twp);
  
  sendFloat(h1, "ambientHum");
  sendFloat(h2, "insideHum");
  sendFloat(t1, "ambientTemp");
  sendFloat(t2, "insideTemp");
}

void sendFloat(float value, String path) {
  if (Firebase.setFloat(firebaseData, path, value)) {
      Serial.println("Data sent.");
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + firebaseData.errorReason());
      Serial.println("------------------------------------");
      Serial.println();
    }
}