#include <SPI.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <iostream>
#include <string>
#include <map>
// #include <FirebaseESP32.h>
// #include <WiFi.h>

#define FIREBASE_HOST "water-harvester.firebaseio.com" //Without http:// or https:// schemes
#define FIREBASE_AUTH "2tMAiCMNGGDRgGUAHpaPCOxcmpzB2UKG8LREjzMj"
#define WIFI_SSID "iPhone van Mike"
#define WIFI_PASSWORD "Mike1234"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for SSD1306 display connected using software SPI (default case):
#define OLED_MOSI   25
#define OLED_CLK   26
#define OLED_DC    14
#define OLED_CS    27
#define OLED_RESET 13
#define DHTPIN1 33
#define DHTPIN2 32
#define ONE_WIRE_BUS 21
#define DHTTYPE DHT11
#define FANPIN 23
#define FANANDPUMPPIN 22 
#define DATAPATH "water-harvester"

//FirebaseData firebaseData;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);
DHT dht1(DHTPIN1, DHTTYPE);
DHT dht2(DHTPIN2, DHTTYPE);
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

#define NUMFLAKES     10 // Number of snowflakes in the animation example
#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16
#define XPOS   0 
#define YPOS   1
#define DELTAY 2
int fanloop = 0;
int bottleloop = 0;
int screenloop = 2;
float t1;
float h1;
float t2;
float h2;
float twp;
String dht1_status;
String dht2_status;
String dht3_status;
String twp_status;
float MAXmovedair = 1.415;
//template<int, char> maxhumidwithtemp;

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

// int waterrendement(){
//   std::map<std::int16_t, std::int16_t> m;
//   m[-20] = 0.9;
//   m[-15] = 1.41;
//   m[-10] = 2.15;
//   m[-5] = 3.26;
//   m[0] = 4.84;
//   m[5] = 6.83;
//   m[10] = 9.4;
//   m[15] = 12.85;
//   m[20] = 17.28;
//   m[25] = 23.05;
//   m[30] = 30.34;
//   m[35] = 39.56;

//   if(!isnan(t1)){
//         //return m[((t1 + 2)/5) * 5];
//         Serial.println(m[-20]);
//         return 0;
//   }
// }


void TurnOnFan(){
  pinMode(FANPIN, OUTPUT);
  digitalWrite(FANPIN, HIGH);
}

void TurnOnFanAndPump(){
  pinMode(FANANDPUMPPIN, OUTPUT);
  digitalWrite(FANANDPUMPPIN, LOW);
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
    display.setCursor(65, 32);
    display.setTextColor(WHITE,BLACK);
    //display.println(waterrendement());
    //Serial.println(waterrendement());
    display.display();
}

// void printResult(FirebaseData &data)
// {

//   if (data.dataType() == "int")
//     Serial.println(data.intData());
//   else if (data.dataType() == "float")
//     Serial.println(data.floatData(), 5);
//   else if (data.dataType() == "double")
//     printf("%.9lf\n", data.doubleData());
//   else if (data.dataType() == "boolean")
//     Serial.println(data.boolData() == 1 ? "true" : "false");
//   else if (data.dataType() == "string")
//     Serial.println(data.stringData());
//   else if (data.dataType() == "json")
//   {
//     Serial.println();
//     FirebaseJson &json = data.jsonObject();
//     //Print all object data
//     Serial.println("Pretty printed JSON data:");
//     String jsonStr;
//     json.toString(jsonStr, true);
//     Serial.println(jsonStr);
//     Serial.println();
//     Serial.println("Iterate JSON data:");
//     Serial.println();
//     size_t len = json.iteratorBegin();
//     String key, value = "";
//     int type = 0;
//     for (size_t i = 0; i < len; i++)
//     {
//       json.iteratorGet(i, type, key, value);
//       Serial.print(i);
//       Serial.print(", ");
//       Serial.print("Type: ");
//       Serial.print(type == JSON_OBJECT ? "object" : "array");
//       if (type == JSON_OBJECT)
//       {
//         Serial.print(", Key: ");
//         Serial.print(key);
//       }
//       Serial.print(", Value: ");
//       Serial.println(value);
//     }
//     json.iteratorEnd();
//   }
//   else if (data.dataType() == "array")
//   {
//     Serial.println();
//     //get array data from FirebaseData using FirebaseJsonArray object
//     FirebaseJsonArray &arr = data.jsonArray();
//     //Print all array values
//     Serial.println("Pretty printed Array:");
//     String arrStr;
//     arr.toString(arrStr, true);
//     Serial.println(arrStr);
//     Serial.println();
//     Serial.println("Iterate array values:");
//     Serial.println();
//     for (size_t i = 0; i < arr.size(); i++)
//     {
//       Serial.print(i);
//       Serial.print(", Value: ");

//       FirebaseJsonData &jsonData = data.jsonData();
//       //Get the result data from FirebaseJsonArray object
//       arr.get(jsonData, i);
//       if (jsonData.typeNum == JSON_BOOL)
//         Serial.println(jsonData.boolValue ? "true" : "false");
//       else if (jsonData.typeNum == JSON_INT)
//         Serial.println(jsonData.intValue);
//       else if (jsonData.typeNum == JSON_DOUBLE)
//         printf("%.9lf\n", jsonData.doubleValue);
//       else if (jsonData.typeNum == JSON_STRING ||
//                jsonData.typeNum == JSON_NULL ||
//                jsonData.typeNum == JSON_OBJECT ||
//                jsonData.typeNum == JSON_ARRAY)
//         Serial.println(jsonData.stringValue);
//     }
//   }
// }


// void HandleDataStream(){
//   if (!Firebase.readStream(firebaseData))
//   {
//     Serial.println("------------------------------------");
//     Serial.println("Can't read stream data...");
//     Serial.println("REASON: " + firebaseData.errorReason());
//     Serial.println("------------------------------------");
//     Serial.println();
//   }

//   if (firebaseData.streamTimeout())
//   {
//     Serial.println("Stream timeout, resume streaming...");
//     Serial.println();
//   }

//   if (firebaseData.streamAvailable())
//   {
//     Serial.println("------------------------------------");
//     Serial.println("Stream Data available...");
//     Serial.println("STREAM PATH: " + firebaseData.streamPath());
//     Serial.println("EVENT PATH: " + firebaseData.dataPath());
//     Serial.println("DATA TYPE: " + firebaseData.dataType());
//     Serial.println("EVENT TYPE: " + firebaseData.eventType());
//     Serial.print("VALUE: ");
//     printResult(firebaseData);
//     Serial.println("------------------------------------");
//     Serial.println();
//   }
// }

void readTemp(){
  delay(500);

  h1 = dht1.readHumidity();
  t1 = dht1.readTemperature();

  h2 = dht2.readHumidity();
  t2 = dht2.readTemperature();

  sensors.requestTemperatures(); 
  twp = sensors.getTempCByIndex(0);
  Serial.println((String) t1 + "|" + h1 + "|" + t2 + "|" + h2 + "|" + "|" + twp);
}


void setup() {
  Serial.begin(9600);

  TurnOnFan();
  TurnOnFanAndPump();

  // WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  // Serial.print("Connecting to Wi-Fi");


  // while (WiFi.status() != WL_CONNECTED)
  //   {
  //     Serial.print(".");
  //     delay(300);
  //   }
  //   Serial.println();
  //   Serial.print("Connected with IP: ");
  //   Serial.println(WiFi.localIP());
  //   Serial.println();

  //   Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  //   Firebase.reconnectWiFi(true);

  //   if (!Firebase.beginStream(firebaseData, DATAPATH))
  //   {
  //     Serial.println("------------------------------------");
  //     Serial.println("Can't begin stream connection...");
  //     Serial.println("REASON: " + firebaseData.errorReason());
  //     Serial.println("------------------------------------");
  //     Serial.println();
  //   }

  // if(!display.begin(SSD1306_SWITCHCAPVCC)) {
  //   Serial.println(F("SSD1306 allocation failed"));
  //   for(;;); 
  // }
  dht1.begin();
  dht2.begin();
  sensors.begin();


  display.clearDisplay();
  display.display();
  delay(2000);
  

}

void loop() {
  display.clearDisplay();
  readTemp();

  if (screenloop == 0){
    TemperatureScreen();
  }
  else if (screenloop == 1){
    HumidityScreen();
  }
  else if (screenloop == 2){
    PredictedWaterRendement();
  }
  else
  {
    SensorCheck();
    SensorScreen();
  }
}
