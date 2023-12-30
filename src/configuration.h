#pragma once 
#include "headers.h"

Configuration config;

void configurationSetup() {
  debug_printf("Configuration consists of %d bytes\n", sizeof(Configuration));
  EEPROM.begin(sizeof(Configuration));
  loadConfiguration();
}

void saveConfiguration() {
  debug_printf("Need to save configuration to EEPROM\n");
  config.checksum = calculateConfigChecksum();
  EEPROM.put(0, config);
  EEPROM.commit();
}

void loadConfiguration() {
  debug_printf("Loading configuration from EEPROM\n");
  EEPROM.get(0, config);

  uint16_t expected_checksum = calculateConfigChecksum(config);
  if (config.checksum != expected_checksum) {
    debug_printf("EEPROM checksum not valid, got %d, expected %d\n", config.checksum, expected_checksum);
    loadDefaultConfiguration();
    saveConfiguration();
  }
}

uint16_t calculateConfigChecksum(Configuration configToCheck) {
  uint16_t previousChecksum = configToCheck.checksum;
  configToCheck.checksum = 0;

  unsigned char *data = (unsigned char *)&configToCheck;
  uint16_t checksum = sizeof(Configuration);
  for (uint16_t i = 0; i < sizeof(Configuration); i++) {
    checksum ^= data[i];
  }

  configToCheck.checksum = previousChecksum;
  return checksum;
}

uint16_t calculateConfigChecksum() {
  return calculateConfigChecksum(config);
}

void loadDefaultConfiguration()
{
  debug_printf("Loading default configuration\n");

  char ntpServer[] = "pool.ntp.org";
  memcpy(config.ntp_server, ntpServer, sizeof(ntpServer));

  char hostname[] = "woordklok";
  memcpy(config.hostname, hostname, sizeof(hostname));

  memcpy(config.clockface, clockface, sizeof(clockface));

  config.ledMode = LedMode::singleColor;

  randomColor(config.singleColor.color);
  randomColor(config.singleColor.backgroundColor);

  for (int i = 0; i < 24; i++)
  {
    randomColor(config.hourlyColor.color[i]);
  }
  randomColor(config.hourlyColor.backgroundColor);
  for (int i = 0; i < NUMWORDS; i++)
  {
    randomColor(config.wordColor.color[i]);
  }
  randomColor(config.wordColor.backgroundColor);

  config.rainbowColor.cycleTime = 25;
  randomColor(config.rainbowColor.backgroundColor);
  config.brightnessMode = BrightnessMode::ldrBrightness;
  config.fixedBrightness.brightness = 100;

  config.ldrBrightness.brightness[0] = 10;
  config.ldrBrightness.brightness[1] = 120;
  config.ldrBrightness.ldrRange[0] = 10;
  config.ldrBrightness.ldrRange[1] = 100;

  config.timeBrightness.brightness[0] = 10;
  config.timeBrightness.brightness[1] = 120;
  config.timeBrightness.timeSlot[0] = 6;
  config.timeBrightness.timeSlot[1] = 22;
  config.checksum = 1245;
  config.checksum = calculateConfigChecksum();
}

void copyConfig(Configuration &confA, Configuration &confB)
{
      reportmem(__func__);
  debug_println(F("copy config"));
  memcpy(&confB, &confA, sizeof(confA));
          reportmem(__func__);
}

DynamicJsonDocument config2JSON(Configuration &conf)
{
  reportmem(__func__);
  DynamicJsonDocument doc(CONFIGSIZE);

  JsonObject system = doc.createNestedObject("system");
  system["ntp_server"] = conf.ntp_server;
  system["hostname"] = conf.hostname;

  JsonObject jsonClockface = doc.createNestedObject(F("clockface"));
  jsonClockface[F("wordGridHorizontal")] = NUM_COLS;
  jsonClockface[F("wordGridVertical")] = NUM_ROWS;
  jsonClockface[F("extraLEDs")] = OFFSET;

  JsonArray layout = jsonClockface.createNestedArray(F("layout"));

  for (int i = 0; i < NUMWORDS; i++)
  {
    JsonObject wordConfig = layout.createNestedObject();
    wordConfig[F("word")] = clockface[i].label;
    JsonArray ledLayout = wordConfig.createNestedArray(F("leds"));
    for (int j = 0; j < clockface[i].label.length(); j++)
    {
      ledLayout.add(clockface[i].leds[j]);
    }
  }

  JsonObject background = layout.createNestedObject();
  background[F("background")] = clockface[NUMWORDS].label;
  JsonArray ledLayout = background.createNestedArray(F("leds"));
  for (int j = 0; j < clockface[NUMWORDS].label.length(); j++)
  {
    ledLayout.add(clockface[NUMWORDS].leds[j]);
  }
  JsonObject colors = doc.createNestedObject("colors");
  colors["ledMode"] = "singleColor";

  JsonObject colors_ledConfig = colors.createNestedObject("ledConfig");

  JsonObject colors_ledConfig_singleColor = colors_ledConfig.createNestedObject("singleColor");
  colors_ledConfig_singleColor["color"] = config.singleColor.color;
  colors_ledConfig_singleColor["backgroundColor"] = config.singleColor.backgroundColor;

  JsonObject colors_ledConfig_rainbowColor = colors_ledConfig.createNestedObject("rainbowColor");
  colors_ledConfig_rainbowColor["cycleTime"] = config.rainbowColor.cycleTime;
  colors_ledConfig_rainbowColor["backgroundColor"] = config.rainbowColor.backgroundColor;

  JsonObject colors_ledConfig_hourlyColor = colors_ledConfig.createNestedObject("hourlyColor");
  JsonArray colors_ledConfig_hourlyColor_color = colors_ledConfig_hourlyColor.createNestedArray("color");
  for (int i = 0; i < 24; i++)
  {
    colors_ledConfig_hourlyColor_color.add(config.hourlyColor.color[i]);
  }
  colors_ledConfig_hourlyColor["backgroundColor"] = config.hourlyColor.backgroundColor;

  JsonObject colors_ledConfig_wordColor = colors_ledConfig.createNestedObject("wordColor");
  JsonArray colors_ledConfig_wordColor_color = colors_ledConfig_wordColor.createNestedArray("color");
  for (int i = 0; i < NUMWORDS; i++)
  {
    colors_ledConfig_wordColor_color.add(config.wordColor.color[i]);
  }
  colors_ledConfig_wordColor["backgroundColor"] = config.wordColor.backgroundColor;

  JsonObject brightness = doc.createNestedObject("brightness");
  brightness["brightnessMode"] = "ldrBrightness";

  JsonObject brightness_settings = brightness.createNestedObject("settings");
  brightness_settings["fixedBrightness"]["brightness"] = config.fixedBrightness.brightness;

  JsonObject brightness_settings_ldrBrightness = brightness_settings.createNestedObject("ldrBrightness");
  JsonObject brightness_settings_ldrBrightness_ldrRange = brightness_settings_ldrBrightness.createNestedObject("ldrRange");
  brightness_settings_ldrBrightness_ldrRange["dark"] = config.ldrBrightness.ldrRange[0];
  brightness_settings_ldrBrightness_ldrRange["bright"] = config.ldrBrightness.ldrRange[1];

  JsonObject brightness_settings_ldrBrightness_brightness = brightness_settings_ldrBrightness.createNestedObject("brightness");
  brightness_settings_ldrBrightness_brightness["min"] = config.ldrBrightness.brightness[0];
  brightness_settings_ldrBrightness_brightness["max"] = config.ldrBrightness.brightness[1];

  JsonObject brightness_settings_timeBrightness = brightness_settings.createNestedObject("timeBrightness");
  JsonObject brightness_settings_timeBrightness_hourSlot = brightness_settings_timeBrightness.createNestedObject("timeSlot");
  brightness_settings_timeBrightness_hourSlot["startHour"] = config.timeBrightness.timeSlot[0];
  brightness_settings_timeBrightness_hourSlot["endHour"] = config.timeBrightness.timeSlot[1];

  JsonObject brightness_settings_timeBrightness_brightness = brightness_settings_timeBrightness.createNestedObject("brightness");
  brightness_settings_timeBrightness_brightness["max"] = config.timeBrightness.brightness[0];
  brightness_settings_timeBrightness_brightness["min"] = config.timeBrightness.brightness[1];

  if (strlen(conf.wifiConfig.static_ip) != 0)
  {
    doc[F("wifi")][F("static_ip")] = conf.wifiConfig.static_ip;
    doc[F("wifi")][F("static_gw")] = conf.wifiConfig.static_gw;
    doc[F("wifi")][F("static_sn")] = conf.wifiConfig.static_sn;
    doc[F("wifi")][F("static_dns1")] = conf.wifiConfig.static_dns1;
    doc[F("wifi")][F("static_dns2")] = conf.wifiConfig.static_dns2;
  }

  doc[F("checksum")] = conf.checksum;
  reportmem(__func__);
  return doc;
}

Configuration JSON2config(DynamicJsonDocument doc)
{
      reportmem(__func__);
  Configuration conf;
  JSON2config(doc, conf);
  return conf;
}


bool JSON2config(DynamicJsonDocument doc, Configuration& conf)
{
      reportmem(__func__);
  // debug_println(F("Got following JSON"));
  // serializeJson(doc,Serial);
  bool validJSON=true;
  for (int i = 0; i < NUMKEYS; i++)
  {
    if (!doc.containsKey(configKeys[i]))
    {
      debug_print(F("Missing key "));
      debug_println(configKeys[i]);
      validJSON = false;
    }
  }
  if (validJSON)
  {
    if (doc[F("system")][F("ntp_server")]){
      const char* ntp_server=doc[F("system")][F("ntp_server")];
      memcpy(conf.ntp_server,ntp_server,sizeof(ntp_server));
    }
    if (doc[F("system")]["hostname"]){
      const char* hostname=doc[F("system")]["hostname"];
      memcpy(conf.hostname, hostname, sizeof(hostname));
    }

    JsonObject jsonColors = doc[F("colors")];
    String ledMode=jsonColors[F("ledMode")];
    if (ledMode=="singleColor"){
      conf.ledMode = LedMode::singleColor;
    } else if (ledMode=="hourlyColor"){
      conf.ledMode = LedMode::hourlyColor;
    } else if (ledMode=="wordColor"){
      conf.ledMode = LedMode::wordColor;
    } else if (ledMode=="rainbowColor"){
      conf.ledMode = LedMode::rainbowColor;
    }

    strncpy(conf.singleColor.color, jsonColors["ledConfig"]["singleColor"]["color"], 8);
    strncpy(conf.singleColor.backgroundColor, jsonColors["ledConfig"]["singleColor"]["backgroundColor"], 8);
  JsonObject hourlyColor = jsonColors["ledConfig"]["hourlyColor"];
  for (int i = 0; i < 24; i++)
  {
    strncpy(conf.hourlyColor.color[i],hourlyColor["color"][i],8);
  }
    strncpy(conf.hourlyColor.backgroundColor,hourlyColor["backgroundColor"],8);

  JsonObject wordColor = jsonColors["ledConfig"]["wordColor"];
  for (int i = 0; i < NUMWORDS; i++)
  {
    strncpy(conf.wordColor.color[i],wordColor["color"][i],8);
  }
    strncpy(conf.wordColor.backgroundColor,wordColor["backgroundColor"],8);

  conf.rainbowColor.cycleTime = jsonColors["ledConfig"]["rainbowColor"]["cycleTime"];
    strncpy(conf.rainbowColor.backgroundColor,jsonColors["ledConfig"]["rainbowColor"]["backgroundColor"],8);

  JsonObject jsonBrightness = doc[F("brightness")];
    String brightnessMode=jsonBrightness["brightnessMode"];
    if (brightnessMode=="fixedBrightness"){
  conf.brightnessMode = BrightnessMode::fixedBrightness;
    } else if (brightnessMode=="ldrBrightness"){
  conf.brightnessMode = BrightnessMode::ldrBrightness;
    } else if (brightnessMode=="timeBrightness"){
  conf.brightnessMode = BrightnessMode::timeBrightness;
    } 

  
  conf.fixedBrightness.brightness = jsonBrightness["fixedBrightness"]["brightness"];
  conf.ldrBrightness.brightness[0] = jsonBrightness["ldrBrightness"]["brightness"]["min"];
  conf.ldrBrightness.brightness[1] = jsonBrightness["ldrBrightness"]["brightness"]["max"];
  conf.ldrBrightness.ldrRange[0] = jsonBrightness["ldrBrightness"]["ldrRange"]["dark"];
  conf.ldrBrightness.ldrRange[1] = jsonBrightness["ldrBrightness"]["ldrRange"]["bright"];

  conf.timeBrightness.brightness[0] = jsonBrightness["timeBrightness"]["brightness"]["min"];
  conf.timeBrightness.brightness[1] = jsonBrightness["timeBrightness"]["brightness"]["min"];
  conf.timeBrightness.timeSlot[0] = jsonBrightness["timeBrightness"]["timeSlot"]["startHour"];
  conf.timeBrightness.timeSlot[1] = jsonBrightness["timeBrightness"]["timeSlot"]["endHour"];;
  conf.checksum = 1245;
  conf.checksum=calculateConfigChecksum(conf);

    if (doc[F("wifi")][F("static_ip")])
    {
      strcpy(conf.wifiConfig.static_ip, doc[F("wifi")][F("static_ip")]);
    }
    if (doc[F("wifi")][F("static_gw")])
    {
      strcpy(conf.wifiConfig.static_gw, doc[F("wifi")][F("static_gw")]);
    }
    if (doc[F("wifi")][F("static_sn")])
    {
      strcpy(conf.wifiConfig.static_sn, doc[F("wifi")][F("static_sn")]);
    }
    if (doc[F("wifi")][F("static_dns1")])
    {
      strcpy(conf.wifiConfig.static_dns1, doc[F("wifi")][F("static_dns1")]);
    }
    if (doc[F("wifi")][F("static_dns2")])
    {
      strcpy(conf.wifiConfig.static_dns2, doc[F("wifi")][F("static_dns2")]);
    }
  }
  else
  {
    // conf.checksum = 0;
    debug_println(F("Invalid JSON config"));
  }

  return true;
}

void printConfig(Configuration conf)
{
      reportmem(__func__);
  String output;
  serializeJsonPretty(config2JSON(conf), output);
  debug_println(output);
  Serial.println(output);
}

void verifyConfigFile()
{
      reportmem(__func__);
  debug_println(F("Verifying config"));
  File configFile = LittleFS.open("/config.json", "r");
  if (configFile)
  {
    StaticJsonDocument<CONFIGSIZE> json;
    auto deserializeError = deserializeJson(json, configFile);

    if (!deserializeError)
    {
      serializeJson(json, Serial);
      Configuration conf = JSON2config(json);
      // printConfig(conf);
    }
    else
    {
      debug_println(F("failed to load json config"));
    }
  }
}