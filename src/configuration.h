#pragma once
#include "headers.h"
bool validJSON = true;

void verifyConfigFile()
{

  debug_println(F("Verifying config"));
  File configFile = LittleFS.open("/config.json", "r");
  if (configFile)
  {
    auto deserializeError = deserializeJson(json, configFile);

    if (!deserializeError)
    {
      serializeJson(json, Serial);
      // Configuration conf = JSON2config(json);
      // printConfig(conf);
    }
    else
    {
      debug_println(F("failed to load json config"));
    }
  }
}

void copyString(JsonVariantConst variant, char *configVal)
{
  if (variant.isNull())
  {
    configVal[0] = 0;
  }
  else
  {
    const char *value = variant.as<const char *>();
    strlcpy(configVal, value, strlen(value) + 1);
  }
}

const char *createString(JsonVariantConst variant)
{
  char *temp;

  if (variant.isNull())
  {
    temp = (char *)malloc(sizeof(char));
    if (!temp)
    {
      Serial.println("ERROR malloc createString");
    }
    temp[0] = 0;
    return (const char *)temp;
  }
  else
  {
    const char *value = variant.as<const char *>();
    temp = (char *)malloc((strlen(value) + 1) * sizeof(char));
    if (!temp)
    {
      Serial.println("ERROR malloc createString");
    }
    strcpy(temp, value);
    return (const char *)temp;
  }
}

void copyColor(JsonVariantConst variant, colorDef &configVal)
{
  const char *value = variant.as<const char *>();
  hexToColorDef(value, &configVal);
}

void config2JSON(Configuration &conf, DynamicJsonDocument &doc)
{
  doc.clear();
  doc.garbageCollect();

  JsonObject system = doc[F("system")].to<JsonObject>();
  system[F("ntp_server")] = conf.ntp_server;
  system[F("hostname")] = conf.hostname;

  JsonObject jsonClockface = doc[F("clockface")].to<JsonObject>();
  jsonClockface[F("wordGridCols")] = conf.clockfaceLayout.wordGridCols;
  jsonClockface[F("wordGridRows")] = conf.clockfaceLayout.wordGridRows;
  jsonClockface[F("extraLEDs")] = conf.clockfaceLayout.extraLEDs;

  JsonArray layout = jsonClockface[F("layout")].to<JsonArray>();

  for (uint8_t i = 0; i < conf.clockfaceLayout.totalWords; i++)
  {
    JsonObject wordConfig = layout.createNestedObject();
    wordConfig[F("word")] = conf.clockface[i].label;
    JsonArray ledLayout = wordConfig[F("leds")].to<JsonArray>();
    for (uint8_t j = 0; j < wordConfig[F("word")].as<String>().length(); j++)
    {
      ledLayout.add(conf.clockface[i].leds[j]);
    }
    wordConfig[F("function")]=isActiveMethodStrings[conf.clockface[i].isActive];
  }

  JsonObject background = layout.createNestedObject();
  background[F("background")]=conf.clockface[conf.clockfaceLayout.totalWords].label;
  JsonArray ledLayout = background[F("leds")].to<JsonArray>();
  for (uint8_t j = 0; j < background[F("background")].as<String>().length(); j++)
  {
    ledLayout.add(conf.clockface[conf.clockfaceLayout.totalWords].leds[j]);
  } 
  background[F("function")]=isActiveMethodStrings[conf.clockface[conf.clockfaceLayout.totalWords].isActive];
  JsonObject colors = doc[F("colors")].to<JsonObject>();

  switch (conf.ledMode)
  {
  case LedMode::singleColor:
    colors[F("ledMode")] = "singleColor";
    break;
  case LedMode::rainbowColor:
    colors[F("ledMode")] = "rainbowColor";
    break;
  case LedMode::hourlyColor:
    colors[F("ledMode")] = "hourlyColor";
    break;
  case LedMode::wordColor:
    colors[F("ledMode")] = "wordColor";
    break;
  default:
    break;
  }

  JsonObject colors_ledConfig = colors[F("ledConfig")].to<JsonObject>();

  JsonObject colors_ledConfig_singleColor = colors_ledConfig[F("singleColor")].to<JsonObject>();
  copyColorToJson(conf.singleColor.color, colors_ledConfig_singleColor, "color");
  copyColorToJson(conf.singleColor.backgroundColor, colors_ledConfig_singleColor, "backgroundColor");

  JsonObject colors_ledConfig_rainbowColor = colors_ledConfig[F("rainbowColor")].to<JsonObject>();
  colors_ledConfig_rainbowColor[F("cycleTime")] = conf.rainbowColor.cycleTime;
  copyColorToJson(conf.rainbowColor.backgroundColor, colors_ledConfig_rainbowColor, "backgroundColor");

  JsonObject colors_ledConfig_hourlyColor = colors_ledConfig[F("hourlyColor")].to<JsonObject>();
  JsonArray colors_ledConfig_hourlyColor_color = colors_ledConfig_hourlyColor[F("color")].to<JsonArray>();
  for (uint8_t i = 0; i < 24; i++)
  {
    copyColorToJson(conf.hourlyColor.color[i], colors_ledConfig_hourlyColor_color);
  }
  copyColorToJson(conf.hourlyColor.backgroundColor, colors_ledConfig_hourlyColor, "backgroundColor");

  JsonObject colors_ledConfig_wordColor = colors_ledConfig[F("wordColor")].to<JsonObject>();
  JsonArray colors_ledConfig_wordColor_color = colors_ledConfig_wordColor[F("color")].to<JsonArray>();
  for (uint8_t i = 0; i < conf.clockfaceLayout.totalWords; i++)
  {
    copyColorToJson(conf.wordColor.color[i], colors_ledConfig_wordColor_color);
  }
  copyColorToJson(conf.wordColor.backgroundColor, colors_ledConfig_wordColor, "backgroundColor");
  JsonObject brightness = doc[F("brightness")].to<JsonObject>();
  ;
  switch (conf.brightnessMode)
  {
  case BrightnessMode::fixedBrightness:
    brightness[F("brightnessMode")] = "fixedBrightness";
    break;
  case BrightnessMode::ldrBrightness:
    brightness[F("brightnessMode")] = "ldrBrightness";
    break;
  case BrightnessMode::timeBrightness:
    brightness[F("brightnessMode")] = "timeBrightness";
    break;
  default:
    break;
  }

  JsonObject brightness_settings = brightness[F("settings")].to<JsonObject>();
  brightness_settings[F("fixedBrightness")][F("brightness")] = conf.fixedBrightness.brightness;

  JsonObject brightness_settings_ldrBrightness = brightness_settings[F("ldrBrightness")].to<JsonObject>();
  JsonObject brightness_settings_ldrBrightness_ldrRange = brightness_settings_ldrBrightness[F("ldrRange")].to<JsonObject>();
  brightness_settings_ldrBrightness_ldrRange[F("dark")] = conf.ldrBrightness.ldrRange.dark;
  brightness_settings_ldrBrightness_ldrRange[F("bright")] = conf.ldrBrightness.ldrRange.bright;

  JsonObject brightness_settings_ldrBrightness_brightness = brightness_settings_ldrBrightness[F("brightness")].to<JsonObject>();
  brightness_settings_ldrBrightness_brightness[F("min")] = conf.ldrBrightness.brightness.min;
  brightness_settings_ldrBrightness_brightness[F("max")] = conf.ldrBrightness.brightness.max;

  JsonObject brightness_settings_timeBrightness = brightness_settings[F("timeBrightness")].to<JsonObject>();
  JsonObject brightness_settings_timeBrightness_hourSlot = brightness_settings_timeBrightness[F("timeSlot")].to<JsonObject>();
  brightness_settings_timeBrightness_hourSlot[F("startHour")] = conf.timeBrightness.timeSlot.startHour;
  brightness_settings_timeBrightness_hourSlot[F("endHour")] = conf.timeBrightness.timeSlot.endHour;

  JsonObject brightness_settings_timeBrightness_brightness = brightness_settings_timeBrightness[F("brightness")].to<JsonObject>();
  brightness_settings_timeBrightness_brightness[F("min")] = conf.timeBrightness.brightness.min;
  brightness_settings_timeBrightness_brightness[F("max")] = conf.timeBrightness.brightness.max;

  if (strlen(conf.wifiConfig.static_ip) != 0)
  {
    doc[F("wifi")][F("static_ip")] = conf.wifiConfig.static_ip;
    doc[F("wifi")][F("static_gw")] = conf.wifiConfig.static_gw;
    doc[F("wifi")][F("static_sn")] = conf.wifiConfig.static_sn;
    doc[F("wifi")][F("static_dns1")] = conf.wifiConfig.static_dns1;
    doc[F("wifi")][F("static_dns2")] = conf.wifiConfig.static_dns2;
  }

  // json[F("checksum")] = conf.checksum;
}

bool JSON2config(const JsonDocument &doc, Configuration &conf)
{
  reportmem("Start config");
  // debug_println(F("Got following JSON"));
  // serializeJson(doc,Serial);

  bool validJSON = true;
  // debug_println(json[F("system")][F("ntp_server")].as<String>());
  for (uint8_t i = 0; i < NUMKEYS; i++)
  {
    if (!doc.containsKey(configKeys[i]))
    {
      // debug_print(F("Missing key "));
      // debug_println(configKeys[i]);
      validJSON = false;
    }
  }
  if (validJSON)
  {

    conf.clockfaceLayout.wordGridCols = json[F("clockface")][F("wordGridCols")].as<unsigned int>();
    conf.clockfaceLayout.wordGridRows = json[F("clockface")][F("wordGridRows")].as<unsigned int>();
    conf.clockfaceLayout.extraLEDs = json[F("clockface")][F("extraLEDs")].as<unsigned int>();
    conf.clockfaceLayout.totalLeds = conf.clockfaceLayout.wordGridCols * conf.clockfaceLayout.wordGridRows + conf.clockfaceLayout.extraLEDs;
    JsonArray layout = json[F("clockface")][F("layout")];
    conf.clockfaceLayout.totalWords = layout.size() - 1;
    conf.clockfaceLayout.hasTwenty= false;
    // debug_printf("Total words: %d\n", totalWords);
    // debug_printf("Total leds: %d\n", totalLeds);
    reportmem("Conversion started");

    if (json[F("system")][F("ntp_server")])
    {
      copyString(json[F("system")][F("ntp_server")], conf.ntp_server);
    }
    reportmem("ntp");

    if (json[F("system")][F("hostname")])
    {
      copyString(json[F("system")][F("hostname")], conf.hostname);
    }
    reportmem("hostname");

    /// Load clockface
    conf.clockface = (ClockfaceWord *)malloc((conf.clockfaceLayout.totalWords + 1)*sizeof(ClockfaceWord));
    if (!conf.clockface)
    {
      Serial.println("ERROR malloc conf.clockface");
    }
    uint8_t backgroundKey = -1;
    for (uint8_t iWord = 0; iWord < conf.clockfaceLayout.totalWords + 1; iWord++)
    {
      JsonArrayConst jsonLeds = layout[iWord]["leds"].as<JsonArray>();
      if (layout[iWord].containsKey(F("word")))
      {
        conf.clockface[iWord].label = createString(layout[iWord][F("word")]);
        conf.clockface[iWord].leds = (uint8_t *)malloc(jsonLeds.size() * sizeof(uint8_t ));
        for (uint8_t i = 0; i < jsonLeds.size(); i++)
        {
          conf.clockface[iWord].leds[i] = jsonLeds[i].as<int>();
        }
        if (strcmp(layout[iWord][F("function")],"twenty")==0){
          debug_println("Clock with twenty");
          conf.clockfaceLayout.hasTwenty=true;
        }
        conf.clockface[iWord].isActive = methodStringToMethod(createString(layout[iWord][F("function")]));
        conf.clockface[iWord].colorCodeInTable = iWord;
      }
      else if (layout[iWord].containsKey(F("background")))
      {
        backgroundKey = iWord;
      }
    }
    if (backgroundKey > 0)
    {
      JsonArrayConst jsonLeds = layout[backgroundKey]["leds"].as<JsonArray>();
      conf.clockface[backgroundKey].label = createString(layout[backgroundKey][F("background")]);
      conf.clockface[backgroundKey].leds = (uint8_t *)malloc(jsonLeds.size() * sizeof(uint8_t ));
      for (uint8_t i = 0; i < jsonLeds.size(); i++)
      {
        conf.clockface[backgroundKey].leds[i] = jsonLeds[i].as<int>();
      }
      conf.clockface[backgroundKey].isActive = methodStringToMethod(createString(layout[backgroundKey][F("function")]));
      conf.clockface[backgroundKey].colorCodeInTable = backgroundKey;
    }

    ///

    JsonObjectConst jsonColors = json[F("colors")].as<JsonObjectConst>();
    const char* ledMode = jsonColors[F("ledMode")].as<const char*>();
    if (strcmp(ledMode,"singleColor")==0)
    {
      conf.ledMode = LedMode::singleColor;
    }
    else if (strcmp(ledMode,"hourlyColor")==0)
    {
      conf.ledMode = LedMode::hourlyColor;
    }
    else if (strcmp(ledMode,"wordColor")==0)
    {
      conf.ledMode = LedMode::wordColor;
    }
    else if (strcmp(ledMode,"rainbowColor")==0)
    {
      conf.ledMode = LedMode::rainbowColor;
    }
    copyColor(jsonColors[F("ledConfig")][F("singleColor")][F("color")], conf.singleColor.color);
    copyColor(jsonColors[F("ledConfig")][F("singleColor")][F("backgroundColor")], conf.singleColor.backgroundColor);
    JsonObjectConst hourlyColor = jsonColors[F("ledConfig")][F("hourlyColor")];
    for (uint8_t i = 0; i < 24; i++)
    {
      copyColor(hourlyColor[F("color")][i], conf.hourlyColor.color[i]);
    }
    copyColor(hourlyColor[F("backgroundColor")], conf.hourlyColor.backgroundColor);

    JsonObjectConst wordColor = jsonColors[F("ledConfig")][F("wordColor")];
    conf.wordColor.color = (colorDef *)malloc(conf.clockfaceLayout.totalWords * sizeof(colorDef));
    if (!conf.wordColor.color)
    {
      Serial.println("ERROR malloc conf.wordcolor.color");
    }
    for (uint8_t i = 0; i < conf.clockfaceLayout.totalWords; i++)
    {
      copyColor(wordColor[F("color")][i], conf.wordColor.color[i]);
    }
    copyColor(wordColor[F("backgroundColor")], conf.wordColor.backgroundColor);

    conf.rainbowColor.cycleTime = jsonColors[F("ledConfig")][F("rainbowColor")][F("cycleTime")].as<unsigned long>();
    copyColor(jsonColors[F("ledConfig")][F("rainbowColor")][F("backgroundColor")], conf.rainbowColor.backgroundColor);

    JsonVariantConst jsonBrightnessMode = json[F("brightness")][F("brightnessMode")];
    const char *brightnessMode = jsonBrightnessMode.as<const char *>();
    if (strcmp(brightnessMode, "fixedBrightness")==0)
    {
      conf.brightnessMode = BrightnessMode::fixedBrightness;
    }
    else if (strcmp(brightnessMode, "ldrBrightness")==0)
    {
      conf.brightnessMode = BrightnessMode::ldrBrightness;
    }
    else if (strcmp(brightnessMode, "timeBrightness")==0)
    {
      conf.brightnessMode = BrightnessMode::timeBrightness;
    }

    JsonObjectConst jsonBrightnessSettings = json[F("brightness")][F("settings")];
    conf.fixedBrightness.brightness = (uint8_t)jsonBrightnessSettings[F("fixedBrightness")][F("brightness")].as<int>();
    conf.ldrBrightness.brightness.min = (uint8_t)jsonBrightnessSettings[F("ldrBrightness")][F("brightness")][F("min")].as<int>();
    conf.ldrBrightness.brightness.max = (uint8_t)jsonBrightnessSettings[F("ldrBrightness")][F("brightness")][F("max")].as<int>();
    conf.ldrBrightness.ldrRange.dark = (uint8_t)jsonBrightnessSettings[F("ldrBrightness")][F("ldrRange")][F("dark")].as<int>();
    conf.ldrBrightness.ldrRange.bright = (uint8_t)jsonBrightnessSettings[F("ldrBrightness")][F("ldrRange")][F("bright")].as<int>();
    conf.timeBrightness.brightness.min = (uint8_t)jsonBrightnessSettings[F("timeBrightness")][F("brightness")][F("min")].as<int>();
    conf.timeBrightness.brightness.max = (uint8_t)jsonBrightnessSettings[F("timeBrightness")][F("brightness")][F("max")].as<int>();
    conf.timeBrightness.timeSlot.startHour = (uint8_t)jsonBrightnessSettings[F("timeBrightness")][F("timeSlot")][F("startHour")].as<int>();
    conf.timeBrightness.timeSlot.endHour = (uint8_t)jsonBrightnessSettings[F("timeBrightness")][F("timeSlot")][F("endHour")].as<int>();

    reportmem("brightness");

    if (json[F("wifi")][F("static_ip")])
    {
      copyString(json[F("wifi")][F("static_ip")], conf.wifiConfig.static_ip);
    }
    if (json[F("wifi")][F("static_gw")])
    {
      copyString(json[F("wifi")][F("static_gw")], conf.wifiConfig.static_gw);
    }
    if (json[F("wifi")][F("static_sn")])
    {
      copyString(json[F("wifi")][F("static_sn")], conf.wifiConfig.static_sn);
    }
    if (json[F("wifi")][F("static_dns1")])
    {
      copyString(json[F("wifi")][F("static_dns1")], conf.wifiConfig.static_dns1);
    }
    if (json[F("wifi")][F("static_dns2")])
    {
      copyString(json[F("wifi")][F("static_dns2")], conf.wifiConfig.static_dns2);
    }
  }
  else
  {
    debug_println(F("Invalid JSON config"));
  }

  return true;
}

void printConfig(Configuration &conf)
{
  config2JSON(conf,json);
  serializeJson(json, Serial);
}

void loadConfiguration(Configuration *conf)
{

  debug_println(F("Loading configuration from filesystem"));
  validJSON = false;
  File configFile = LittleFS.open("/config.json", "r");
  if (configFile)
  {
    Serial.println(F("opened config file"));
    /*size_t size = configFile.size();
    std::unique_ptr<char[]> buf(new char[size]);
    configFile.readBytes(buf.get(), size);*/
    reportmem(F("file voor deserialize"));
    auto deserializeError = deserializeJson(json, configFile);
    reportmem(F("file na  deserialize"));
    configFile.close();
    if (!deserializeError)
    {
      // serializeJson(json,Serial);
      // serializeJsonPretty(json, Serial);
      validJSON = true;
      JSON2config(json, *conf);
    }
    else
    {
      debug_println(deserializeError.c_str());
      debug_println(F("failed to load json config"));
    }
  }
  else
  {
    debug_println(F("Cannot open config file"));
  }

  if (!validJSON)
  {
    debug_println(F("Invalid JSON"));
    Serial.println(F("Invalid JSON"));
    // loadDefaultConfiguration();
  }

  // copyConfig(config, oldConfig);
}

void configurationSetup()
{
  reportmem("test");
  /* config = (Configuration *)malloc(sizeof(Configuration));
  if (!config)
  {
    Serial.println("ERROR malloc configurationSetup");
  } */

  reportmem("test");
  loadConfiguration(&config);
}

void saveConfiguration(Configuration &conf)
{

  debug_println(F("Need to save configuration to Filesystem"));
  File configFile = LittleFS.open("/config.json", "w");
  if (!configFile)
  {
    debug_println(F("failed to open config file for writing"));
  }
  config2JSON(conf,json);
  serializeJson(json, configFile);
  configFile.close();
  debug_println(F("Closed file"));
}

void copyConfig(Configuration &confA, Configuration &confB)
{
  memcpy(&confA, &confB, sizeof(Configuration));
}
