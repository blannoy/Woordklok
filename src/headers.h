#pragma once 

#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include <FS.h>
#include <LittleFS.h>
#include <ESP8266mDNS.h>
#include <EEPROM.h>
#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>

#define DEBUG true // flag to turn on/off debugging
#define debug_begin(...) do { if (DEBUG) { Serial.begin(__VA_ARGS__); while(!Serial); }} while (0)
#define debug_print(...) do { if (DEBUG) Serial.print(__VA_ARGS__); } while (0)
#define debug_println(...) do { if (DEBUG) Serial.println(__VA_ARGS__); } while (0)
#define debug_printf(...) do { if (DEBUG) printf(__VA_ARGS__); } while (0)

#define numReadings 20

#define HOSTNAME_MAX 256
#define NUM_ROWS 10
#define NUM_COLS 11
#define OFFSET 4  // 4 minute dots
#define MATRIX_LEDS NUM_ROWS* NUM_COLS
#define NUM_LEDS NUM_ROWS* NUM_COLS + OFFSET
#define NUMWORDS 27
#define BOOTANIMTIME 5000
String version="0.5";

extern int PixelCount = NUMWORDS;

enum LedMode {
  single,
  words,
  hourly,
  rainbow
};

enum BrightnessMode {
  fixedBrightness,
  ldrBrightness,
  timeBrightness
};

struct ClockfaceWord {
  std::vector<int> leds;
  int colorCodeInTable;
  String label;
  bool (*isActive)(int, int);
};

struct Configuration {
  char ntp_server[256];
  char hostname[HOSTNAME_MAX];
  ClockfaceWord clockface[NUMWORDS];
  LedMode ledMode;
  int singleColorHue;
  int hourlyColors[24];
  int wordColors[NUMWORDS];
  BrightnessMode brightnessMode;
  uint8_t ldrDark;
  uint8_t ldrBright;
  uint8_t maxBrightness;
  uint8_t minBrightness;
  uint8_t brightnessStartHour;
  uint8_t brightnessEndHour;
  uint8_t checksum;
};

typedef enum { BOOT,
               STARTUP,
               TOUCHED,
               SPLASH,
               CLOCK,
               TESTPIXEL,
               TESTFULL,
               TESTHOURS,
               TESTMINUTES } states;
static char* statesStrings[] = { "BOOT", "STARTUP", "TOUCHED", "SPLASH", "CLOCK", "TESTPIXEL", "TESTFULL", "TESTHOURS", "TESTMINUTES" };

extern states state;

enum statusLedList {
  WIFI,
  Time,
  AP
};

extern Configuration config;
extern ClockfaceWord clockface[];
extern bool serverStarted;
extern ESP8266WebServer server;
extern ESP8266HTTPUpdateServer flashUpdateServer;
extern ESP8266HTTPUpdateServer fsUpdateServer;

extern uint8_t dateHours;
extern uint8_t dateMinutes;
extern uint8_t dateSeconds;
extern uint8_t dateDay;
extern uint8_t dateMonth;
extern uint8_t dateYear;

extern long LDRvalue;
extern bool touched;
extern bool touchtrigger;

#define MAXDROPS 12
#define colorSaturation 64
extern RgbColor red;
extern RgbColor green;
extern RgbColor blue;
extern RgbColor white;
extern RgbColor black;
extern int splashScreen[];
extern long lastLedUpdate;

extern int testState;
extern int currentPixel;
extern long timeStamp;
struct RCCoord {
  int row = 0;
  int col = 0;
};

void APModeCallback (WiFiManager*);
void saveConfigCallback();
void wifiSetup();
void wifiLoop();
void configWifi();

void handleRoot();
void handleNotFound();
void dumpLittleFS();
void setBrightness();
void setLedMode();
void apiSendError(String message);
void apiSendJSON(int status, JsonObject object);
void setPixelCount();
void runTestPixels();
void runTestAllPixels();
void runClockFaceTestMinutes();
void runClockFaceTestHours();
void webServerSetup();
void stopServer();
void getLog();
void webServerLoop();
bool webserverServeFileFromFS(String);

void configurationSetup();
void saveConfiguration();
void loadConfiguration();
void loadDefaultConfiguration();
uint8_t calculateConfigChecksum();
void calibrateSensor();
void setupSensors();
void sensorLoop();

void showTime();
void time_is_set_scheduled();
void timeSetup();
void timeLoop();

void changeState(states);
void logLoop();
void flushLog();
void putchar_(char);

void ledSetup();
void clearLEDS();
void FadeAll(uint8_t);
void showSplash();
void FadeAnimUpdate(const AnimationParam&);
void MoveAnimUpdate(const AnimationParam&);
void stopRain();
void dropLetter(int);
void setupRain();
void rainLoop();
void ledShowClockface();
void clockLoop();
void statusLed(statusLedList, NeoBufferMethod<NeoGrbFeature>::ColorObject);
void setColor(int, NeoBufferMethod<NeoGrbFeature>::ColorObject);
void clearLEDTo(NeoBuffer<NeoBufferMethod<NeoGrbFeature>>&, NeoBufferMethod<NeoGrbFeature>::ColorObject);
void SetupAnimationSet();
void showFace(bool);
void setStatusLeds();
struct RCCoord calcCoord(int);
byte calcBrightness();
RgbColor getColor(NeoBuffer<NeoBufferMethod<NeoGrbFeature>>&, int);
bool doAnimation();
HslColor valueToColor(int);