
#pragma once

#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include <ESP8266mDNS.h>
#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>
#include <FS.h>
#include <LittleFS.h>

#define numReadings 20
#define BACKGROUNDDIMFACTOR 100

#define TESTTIMER 30000
long testStart=0;
#define filesystem (LittleFS)

#define HOSTNAME_MAX 256
#define CONFIGSIZE 8192
#define NUMKEYS 5
String configKeys[NUMKEYS] = {"system", "clockface", "colors", "brightness", "checksum"};

typedef enum { BOOT,
               STARTUP,
               TOUCHED,
               SPLASH,
               CLOCK,
               TESTPIXEL,
               TESTFULL,
               TESTHOURS,
               TESTMINUTES,
               TESTCOLORS } states;
static char* statesStrings[] = { "BOOT", "STARTUP", "TOUCHED", "SPLASH", "CLOCK", "TESTPIXEL", "TESTFULL", "TESTHOURS", "TESTMINUTES", "TESTCOLORS" };

extern states state;
typedef enum
{
  alwaysOn,
  isJust,
  fiveMinute,
  tenMinute,
  quarter,
  past,
  before,
  wholeHour,
  halfHour,
  tenMinute_20,
  twenty,
  past_20,
  before_20,
  halfHour_20,
  firstMinute,
  secondMinute,
  thirdMinute,
  fourthMinute,
  isOneActive,
  isTwoActive,
  isThreeActive,
  isFourActive,
  isFiveActive,
  isSixActive,
  isSevenActive,
  isEightActive,
  isNineActive,
  isTenActive,
  isElevenActive,
  isTwelveActive
} isActiveMethod;
const char *isActiveMethodStrings[] = {"alwaysOn", "isJust", "fiveMinute", "tenMinute", "quarter", "past", "before", "wholeHour", "halfHour", "tenMinute_20", "twenty", "past_20", "before_20", "halfHour_20","firstMinute", "secondMinute", "thirdMinute", "fourthMinute", "isOneActive", "isTwoActive", "isThreeActive", "isFourActive", "isFiveActive", "isSixActive", "isSevenActive", "isEightActive", "isNineActive", "isTenActive", "isElevenActive", "isTwelveActive"};

struct ClockfaceWord
{
  uint8_t *leds;
  uint8_t colorCodeInTable;
  const char *label;
  isActiveMethod isActive;
};

struct ClockfaceLayout
{
  uint8_t wordGridCols;
  uint8_t wordGridRows;
  uint8_t extraLEDs;
  uint16_t totalLeds;
  uint8_t totalWords;
  bool hasTwenty;
};
#define BOOTANIMTIME 5000
String version = "0.5";

extern uint8_t PixelCount;

enum statusLedList {
  WIFI,
  Time,
  AP
};


enum LedMode
{
  singleColor,
  wordColor,
  hourlyColor,
  rainbowColor
};

enum BrightnessMode
{
  fixedBrightness,
  ldrBrightness,
  timeBrightness
};

struct colorDef
{
  uint8_t r;
  uint8_t g;
  uint8_t b;
  bool complementary=false;
};

struct BrightnessDef
{
  uint8_t min;
  uint8_t max;
};

struct LdrRangeDef
{
  uint8_t dark;
  uint8_t bright;
};

struct WifiConfig
{
  char static_ip[16] = "";
  char static_gw[16] = "";
  char static_sn[16] = "";
  char static_dns1[16] = "";
  char static_dns2[16] = "";
};
struct SingleColorConfig
{
  colorDef color;
  colorDef backgroundColor;
};

struct RainbowColorConfig
{
  unsigned long cycleTime;
  colorDef backgroundColor;
};

struct WordColorConfig
{
  colorDef *color;
  colorDef backgroundColor;
};

struct HourlyColorConfig
{
  colorDef color[24];
  colorDef backgroundColor;
};

struct FixedBrightnessConfig
{
  uint8_t brightness;
};

struct LDRBrightnessConfig
{
  LdrRangeDef ldrRange;
  BrightnessDef brightness;
};

struct TimeSlotDef
{
  uint8_t startHour;
  uint8_t endHour;
};

struct TimeBrightnessConfig
{
  TimeSlotDef timeSlot;
  BrightnessDef brightness;
};

struct Configuration
{
  char ntp_server[64];
  char hostname[64];
  ClockfaceLayout clockfaceLayout;
  ClockfaceWord *clockface;
  LedMode ledMode;
  WifiConfig wifiConfig;
  SingleColorConfig singleColor;
  RainbowColorConfig rainbowColor;
  WordColorConfig wordColor;
  HourlyColorConfig hourlyColor;
  BrightnessMode brightnessMode;
  FixedBrightnessConfig fixedBrightness;
  LDRBrightnessConfig ldrBrightness;
  TimeBrightnessConfig timeBrightness;
};

Configuration config;
Configuration workingConfig;
DynamicJsonDocument json(CONFIGSIZE);

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
extern uint8_t splashScreen[];
extern long lastLedUpdate;

extern uint8_t testState;
extern uint8_t currentPixel;
extern long timeStamp;
struct RCCoord {
  uint8_t row = 0;
  uint8_t col = 0;
};
void ledSetup();
void clearLEDS();
void FadeAll(uint8_t);
void showSplash();
void FadeAnimUpdate(const AnimationParam&);
void MoveAnimUpdate(const AnimationParam&);
void stopRain();
void dropLetter(uint8_t);
void setupRain();
void rainLoop();
void ledShowClockface();
void clockLoop();
void statusLed(statusLedList, NeoBufferMethod<NeoGrbFeature>::ColorObject);
void SetupAnimationSet();
void setStatusLeds();
struct RCCoord calcCoord(uint8_t);
byte calcBrightness();
RgbColor getColor(NeoBuffer<NeoBufferMethod<NeoGrbFeature>>&, uint8_t);
bool doAnimation();

void setColor(uint8_t, NeoBufferMethod<NeoGrbFeature>::ColorObject);
RgbColor getColor(NeoBuffer<NeoBufferMethod<NeoGrbFeature>> &, uint8_t);
void clearLEDTo(NeoBuffer<NeoBufferMethod<NeoGrbFeature>> &, NeoBufferMethod<NeoGrbFeature>::ColorObject);
void showFace(bool);
RgbColor HueToRgbColor(uint8_t);
isActiveMethod methodStringToMethod(String);
isActiveMethod methodStringToMethod(const char *);

void flushLog();
void webServerSetup();
void stopServer();
bool webserverServeFileFromFS(String);
void handleRoot();
void handleNotFound();
void dumpLittleFS();
void setBrightness();
void apiSendError(String message);
void apiSendJSON(int status, JsonObject object);
void runTestPixels();
void runTestAllPixels();
void runClockFaceTestMinutes();
void runClockFaceTestHours();