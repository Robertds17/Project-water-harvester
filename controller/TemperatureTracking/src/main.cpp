#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <Adafruit_Sensor.h>
#include "DHT.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include <FirebaseESP32.h>


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_MOSI   25
#define OLED_CLK   26
#define OLED_DC    14
#define OLED_CS    27
#define OLED_RESET 13
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
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
  OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);
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

int fanloop = 0;
int bottleloop = 0;
int screenloop = 0;
float predictedWater = 0;

String dht1_status;
String dht2_status;
String dht3_status;
String twp_status;

int fanOn;

FirebaseData firebaseData;
const unsigned char bitmap_9ywlzf[] PROGMEM = {
  B00000000,B00000000,B01111100,B00000000,
  B00000000,B00000000,B11111111,B00000000,
  B00000000,B00000001,B11111111,B10000000,
  B00000000,B00000011,B11111111,B11000000,
  B00000000,B00000011,B11111111,B11000000,
  B00000000,B00000011,B11111111,B11000000,
  B00011110,B00000011,B11111111,B11000000,
  B00111111,B00000011,B11111111,B10000000,
  B01111111,B00000011,B11111100,B00000000,
  B01111111,B00000011,B11110000,B00000000,
  B11111111,B00000011,B11100000,B00000000,
  B11111111,B10000001,B11100000,B00000000,
  B11111111,B11000001,B11000000,B00000000,
  B11111111,B11110011,B11000000,B00000000,
  B11111111,B11111111,B11100111,B11111000,
  B01111111,B11111110,B01111111,B11111100,
  B00111111,B11111110,B01111111,B11111110,
  B00011111,B11100111,B11111111,B11111111,
  B00000000,B00000011,B11001111,B11111111,
  B00000000,B00000011,B10000011,B11111111,
  B00000000,B00000111,B10000001,B11111111,
  B00000000,B00000111,B11000000,B11111111,
  B00000000,B00001111,B11000000,B11111110,
  B00000000,B00111111,B11000000,B11111110,
  B00000001,B11111111,B11000000,B11111100,
  B00000011,B11111111,B11000000,B01111000,
  B00000011,B11111111,B11000000,B00000000,
  B00000011,B11111111,B11000000,B00000000,
  B00000011,B11111111,B11000000,B00000000,
  B00000001,B11111111,B10000000,B00000000,
  B00000000,B11111111,B00000000,B00000000,
  B00000000,B00111110,B00000000,B00000000
};



const unsigned char bitmap_2qds4l[] PROGMEM = {
  B00000000,B00111110,B00000000,B00000000,
  B00000000,B11111111,B00000000,B00000000,
  B00000001,B11111111,B00000000,B00000000,
  B00000011,B11111111,B10000000,B00000000,
  B00000011,B11111111,B00000000,B00000000,
  B00000011,B11111110,B00000000,B00000000,
  B00000111,B11111110,B00000011,B11111000,
  B00000011,B11111100,B00000111,B11111100,
  B00000011,B11111100,B00001111,B11111110,
  B00000011,B11111100,B00001111,B11111110,
  B00000001,B11111100,B00011111,B11111111,
  B00000000,B11111110,B00111111,B11111111,
  B00000000,B01111110,B00111111,B11111111,
  B00000000,B00011111,B11111111,B11111111,
  B00000000,B00000111,B11111100,B01111111,
  B00010000,B00000110,B01100000,B00011110,
  B01111000,B00000110,B01100000,B00001000,
  B11111110,B00011111,B11100000,B00000000,
  B11111111,B11111111,B11111000,B00000000,
  B11111111,B11111100,B01111110,B00000000,
  B11111111,B11111100,B01111111,B00000000,
  B11111111,B11111000,B00111111,B10000000,
  B01111111,B11111000,B00111111,B11000000,
  B01111111,B11110000,B00111111,B11000000,
  B00111111,B11100000,B00111111,B11000000,
  B00011111,B11000000,B01111111,B11100000,
  B00000000,B00000000,B01111111,B11000000,
  B00000000,B00000000,B11111111,B11000000,
  B00000000,B00000001,B11111111,B11000000,
  B00000000,B00000000,B11111111,B10000000,
  B00000000,B00000000,B11111111,B00000000,
  B00000000,B00000000,B01111100,B00000000
};


const unsigned char bitmap_8kyah[] PROGMEM = {
  B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000111,B11100000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000111,B11100000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000111,B11100000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00001111,B11110000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00001111,B11110000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000111,B11100000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000111,B11110000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00001111,B11110000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00011111,B11111000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00111111,B11111100,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B01111111,B11111110,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B11111111,B11111111,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11111111,B11111111,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11000000,B00000011,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11000000,B00000011,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11000000,B00000011,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11000000,B00000011,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11000000,B00000011,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11000000,B00000011,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B11000000,B00000011,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B11000000,B00000011,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11000000,B00000011,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11000000,B00000011,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11000000,B00000011,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11000000,B00000011,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B11000000,B00000011,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11000000,B00000011,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11000000,B00000011,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11000000,B00000011,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11000000,B00000011,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B11000000,B00000011,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11000000,B00000011,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11000000,B00000011,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11000000,B00000011,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11000000,B00000011,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11000000,B00000011,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B11000000,B00000011,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11000000,B00000011,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11000000,B00000011,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11000000,B00000011,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11000000,B00000011,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B11000000,B00000011,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11000000,B00000011,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11000000,B00000011,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11000000,B00000011,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11000000,B00000011,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B11000000,B00000011,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B11000000,B00000011,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11000000,B00000011,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11000000,B00000011,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11000000,B00000011,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11000000,B00000011,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B11000000,B00000011,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11000000,B00000011,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11111111,B11111111,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11111111,B11111111,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000
};

const unsigned char bitmap_owcao[] PROGMEM = {
  B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000111,B11100000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000111,B11100000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000111,B11100000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000111,B11100000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00001111,B11110000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000111,B11100000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000111,B11110000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00001111,B11110000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00011111,B11111000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00111111,B11111100,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B01111111,B11111110,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B11111111,B11111111,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11111111,B11111111,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11000000,B00000011,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11000000,B00000011,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11000000,B00000011,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11000000,B00000011,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11000000,B00000011,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11000000,B00000011,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B11000000,B00000011,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B11000000,B00000011,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11000000,B00000011,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11000000,B00000011,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11000000,B00000011,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11000000,B00000011,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B11000000,B00000011,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11000000,B00000011,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11000000,B00000011,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11000000,B00000011,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11000000,B00000011,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B11000000,B00000011,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11000000,B00000011,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11000000,B00000011,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11000000,B00000011,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11000000,B00000011,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11000000,B00000011,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B11000000,B00000011,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11000000,B00000011,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11000000,B00000011,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11000000,B00000011,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11111111,B11111111,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B11111111,B11111111,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11111111,B11111111,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11111111,B11111111,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11111111,B11111111,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11111111,B11111111,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B11111111,B11111111,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B11111111,B11111111,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11111111,B11111111,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11111111,B11111111,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11111111,B11111111,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11111111,B11111111,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B11111111,B11111111,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11111111,B11111111,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11111111,B11111111,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11111111,B11111111,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000
};

const unsigned char bitmap_4rww8e[] PROGMEM = {
  B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000111,B11100000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000111,B11100000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000111,B11100000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000111,B11100000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00001111,B11110000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000111,B11100000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000111,B11110000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00001111,B11110000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00011111,B11111000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00111111,B11111100,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B01111111,B11111110,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B11111111,B11111111,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11111111,B11111111,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11000000,B00000011,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11000000,B00000011,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11000000,B00000011,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11000000,B00000011,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11000000,B00000011,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11000000,B00000011,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B11000000,B00000011,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B11000000,B00000011,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11000000,B00000011,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11000000,B00000011,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11000000,B00000011,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11000000,B00000011,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B11000000,B00000011,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11100000,B00000111,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11111111,B11111111,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11111111,B11111111,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11111111,B11111111,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B11111111,B11111111,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11111111,B11111111,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11111111,B11111111,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11111111,B11111111,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11111111,B11111111,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11111111,B11111111,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B11111111,B11111111,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11111111,B11111111,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11111111,B11111111,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11111111,B11111111,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11111111,B11111111,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B11111111,B11111111,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11111111,B11111111,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11111111,B11111111,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11111111,B11111111,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11111111,B11111111,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B11111111,B11111111,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B11111111,B11111111,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11111111,B11111111,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11111111,B11111111,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11111111,B11111111,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11111111,B11111111,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B11111111,B11111111,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11111111,B11111111,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11111111,B11111111,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11111111,B11111111,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000
};


const unsigned char bitmap_0e3yha[] PROGMEM = {
  B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000111,B11100000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000111,B11100000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000111,B11100000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000111,B11100000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00001111,B11110000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000111,B11100000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000111,B11110000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00001111,B11110000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00011111,B11111000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00111111,B11111100,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B01111111,B11111110,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B11111111,B11111111,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11111111,B11111111,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11111111,B11111111,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11111111,B11111111,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11111111,B11111111,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11111111,B11111111,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11111111,B11111111,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11111111,B11111111,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B11111111,B11111111,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B11111111,B11111111,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11111111,B11111111,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11111111,B11111111,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11111111,B11111111,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11111111,B11111111,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B11111111,B11111111,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11111111,B11111111,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11111111,B11111111,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11111111,B11111111,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11111111,B11111111,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B11111111,B11111111,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11111111,B11111111,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11111111,B11111111,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11111111,B11111111,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11111111,B11111111,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11111111,B11111111,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B11111111,B11111111,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11111111,B11111111,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11111111,B11111111,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11111111,B11111111,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11111111,B11111111,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B11111111,B11111111,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11111111,B11111111,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11111111,B11111111,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11111111,B11111111,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11111111,B11111111,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B11111111,B11111111,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B11111111,B11111111,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11111111,B11111111,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11111111,B11111111,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11111111,B11111111,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11111111,B11111111,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B11111111,B11111111,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11111111,B11111111,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11111111,B11111111,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000001,B11111111,B11111111,B10000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000
};

// Prototypes
void measureDHTValues();
void readFirebase();
void sendFloat(float, String);
void SensorCheck();
void PredictedWaterRendement();
void TemperatureScreen();
void HumidityScreen();
void SensorScreen();


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

  if(!display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.display();
  delay(2000);

    // Clear the buffer
  display.clearDisplay();

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

  display.clearDisplay();

  if (screenloop <= 100){
    TemperatureScreen();
    screenloop++;
  }
  else if (screenloop <= 200){
    HumidityScreen();
    screenloop++;
  }
  else if (screenloop <= 300){
    PredictedWaterRendement();
    screenloop++;
  }
  else if (screenloop <= 400)
  {
    SensorCheck();
    SensorScreen();
    screenloop++;
  }
  else{
    screenloop = 0;
  }
  display.display();
  Serial.println(screenloop);
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
void TemperatureScreen() {
    if (fanloop == 0){
    display.drawBitmap(90, 10, bitmap_2qds4l, 32,32, SSD1306_WHITE);
    fanloop = 1;
  }
  else
  {
    display.drawBitmap(90, 10, bitmap_9ywlzf, 32,32, SSD1306_WHITE);
    fanloop = 0;
  }
  display.fillRect(50,20,24, 42, SSD1306_WHITE);
  display.fillRect(40, 42, 16, 20, SSD1306_WHITE);
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0,54);             // Start at top-left corner
  display.println((String)t1 + "C");
  display.setCursor(45, 0);
  display.println((String)t2 + "C");
  display.setCursor(85,54);
  display.println((String)twp + "C");
  display.display();

  delay(200);
}

void HumidityScreen() {
    if (fanloop == 0){
    display.drawBitmap(90, 10, bitmap_2qds4l, 32,32, SSD1306_WHITE);
    fanloop = 1;
  }
  else
  {
    display.drawBitmap(90, 10, bitmap_9ywlzf, 32,32, SSD1306_WHITE);
    fanloop = 0;
  }
  display.fillRect(50,20,24, 42, SSD1306_WHITE);
  display.fillRect(40, 42, 16, 20, SSD1306_WHITE);
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0,54);             // Start at top-left corner
  display.println((String)h1 + "%");
  display.setCursor(45, 0);
  display.println((String)h2 + "%");
  display.setCursor(85,54);
  display.println((String)twp + "%");
  display.display();

  delay(200);
}

void SensorScreen() {
  display.setCursor(0,0);
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);
  display.println("DHT11 - 1:       " + dht1_status);
  display.println("DHT11 - 2:       " + dht2_status);
  display.println("DHT11 - 3:       " + dht3_status);
  display.println("DS18P20:         " + twp_status);
  display.display();
}

void SensorCheck(){

  if (isnan(t1) || isnan(h1)) {
    dht1_status = "off";
  }
  else{
    dht1_status = "on";
  }
  if (isnan(t2) || isnan(h2)) {
    dht2_status = "off";
  }
    else{
    dht2_status = "on";
  }

    if (twp == -127){
    twp_status = "off";
  }
    else{
    twp_status = "on";
  }
}

void PredictedWaterRendement(){
    
    std::vector<float> randomint = {0.10,0.11,0.13,0.15,0.2,0.10,0.11,0.13,0.15,0.2};



      display.clearDisplay();
      if (bottleloop == 1){
      display.drawBitmap(0,0,bitmap_owcao, 64,64,SSD1306_WHITE);
      bottleloop++;
      }
      else if (bottleloop == 2)
      {
        display.drawBitmap(0,0,bitmap_4rww8e, 64,64,SSD1306_WHITE);
        bottleloop++;
      }
      else if (bottleloop == 3)
      {
        display.drawBitmap(0,0,bitmap_0e3yha, 64,64,SSD1306_WHITE);
        bottleloop = 0;
      }
      else
      {
        display.drawBitmap(0,0,bitmap_8kyah, 64,64,SSD1306_WHITE);
        bottleloop++;
      }
        int index = rand() % randomint.size();
        float value = randomint[index];
        display.setCursor(65, 32);
        display.setTextColor(WHITE,BLACK);
        display.println((String)value + " l/h");
        display.display();
        delay(300);
}


