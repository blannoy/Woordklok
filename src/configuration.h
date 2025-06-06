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
  if (strcmp(value, "#xxxxxx") == 0)
  {
    configVal.complementary = true;
    return;
  } 
  hexToColorDef(value, &configVal);
}

void clockface2JSON(Configuration &conf, DynamicJsonDocument &doc)
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
    wordConfig[F("function")] = isActiveMethodStrings[conf.clockface[i].isActive];
  }

  JsonObject background = layout.createNestedObject();
  background[F("background")] = conf.clockface[conf.clockfaceLayout.totalWords].label;
  JsonArray ledLayout = background[F("leds")].to<JsonArray>();
  for (uint8_t j = 0; j < background[F("background")].as<String>().length(); j++)
  {
    ledLayout.add(conf.clockface[conf.clockfaceLayout.totalWords].leds[j]);
  }
  background[F("function")] = isActiveMethodStrings[conf.clockface[conf.clockfaceLayout.totalWords].isActive];
  JsonObject colors = doc[F("colors")].to<JsonObject>();
}

void config2JSON(Configuration &conf, DynamicJsonDocument &doc)
{
  doc.clear();
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
  brightness[F("backgroundDimFactor")] = conf.backgroundDimFactor;
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

  if (conf.sensorCount > 0)
  {
    // JsonArray sensors = doc[F("sensors")].to<JsonArray>();
    // for (uint8_t i = 0; i < conf.sensorCount; i++)
    // {
    //   sensors.add(conf.availableSensors[i]);
    //   // if (strcmp(conf.availableSensors[i], "touch") == 0)
    //   // {
    //   //   doc[F("touchThreshold")] = conf.touchThreshold;
    //   // }
    // }
    JsonArray sensors = doc.createNestedArray(F("sensors"));
    for (uint8_t i = 0; i < conf.sensorCount; i++)
    {
      JsonObject sensor = sensors.createNestedObject();
      sensor[F("name")] = conf.sensors[i].name;
      JsonArray attributes = sensor.createNestedArray(F("attributes"));
      for (uint8_t j = 0; j < conf.sensors[i].attributeCount; j++)
      {
        JsonObject attribute = attributes.createNestedObject();
        attribute[F("name")] = conf.sensors[i].attributeNames[j];
        attribute[F("value")] = conf.sensors[i].attributeValues[j];
      }
    }
  }
}

bool clockface2config(const JsonDocument &doc, Configuration &conf)
{

  bool validJSON = true;

  if (validJSON)
  {
    conf.clockfaceLayout.wordGridCols = json[F("clockface")][F("wordGridCols")].as<unsigned int>();
    conf.clockfaceLayout.wordGridRows = json[F("clockface")][F("wordGridRows")].as<unsigned int>();
    conf.clockfaceLayout.extraLEDs = json[F("clockface")][F("extraLEDs")].as<unsigned int>();
    conf.clockfaceLayout.totalLeds = conf.clockfaceLayout.wordGridCols * conf.clockfaceLayout.wordGridRows + conf.clockfaceLayout.extraLEDs;
    JsonArray layout = json[F("clockface")][F("layout")];
    conf.clockfaceLayout.totalWords = layout.size() - 1;
    conf.clockfaceLayout.hasTwenty = false;

    if (json[F("system")][F("ntp_server")])
    {
      copyString(json[F("system")][F("ntp_server")], conf.ntp_server);
    }


    if (json[F("system")][F("hostname")])
    {
      copyString(json[F("system")][F("hostname")], conf.hostname);
    }


    /// Load clockface
    conf.clockface = (ClockfaceWord *)malloc((conf.clockfaceLayout.totalWords + 1) * sizeof(ClockfaceWord));
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
        conf.clockface[iWord].leds = (uint8_t *)malloc(jsonLeds.size() * sizeof(uint8_t));
        for (uint8_t i = 0; i < jsonLeds.size(); i++)
        {
          conf.clockface[iWord].leds[i] = jsonLeds[i].as<int>();
        }
        if (strcmp(layout[iWord][F("function")], "twenty") == 0)
        {
          debug_println("Clock with twenty");
          conf.clockfaceLayout.hasTwenty = true;
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
      conf.clockface[backgroundKey].leds = (uint8_t *)malloc(jsonLeds.size() * sizeof(uint8_t));
      for (uint8_t i = 0; i < jsonLeds.size(); i++)
      {
        conf.clockface[backgroundKey].leds[i] = jsonLeds[i].as<int>();
      }
      conf.clockface[backgroundKey].isActive = methodStringToMethod(createString(layout[backgroundKey][F("function")]));
      conf.clockface[backgroundKey].colorCodeInTable = backgroundKey;
    }
  }

  clockfaceLEDSetup();
  return true;
}

bool JSON2config(const JsonDocument &doc, Configuration &conf)
{

  bool validJSON = true;
  // debug_println(json[F("system")][F("ntp_server")].as<String>());
  // for (uint8_t i = 0; i < NUMKEYS; i++)
  // {
  //   if (!doc.containsKey(configKeys[i]))
  //   {
  //     // debug_print(F("Missing key "));
  //     // debug_println(configKeys[i]);
  //     validJSON = false;
  //   }
  // }
  if (validJSON)
  {

    JsonObjectConst jsonColors = json[F("colors")].as<JsonObjectConst>();
    const char *ledMode = jsonColors[F("ledMode")].as<const char *>();
    if (strcmp(ledMode, "singleColor") == 0)
    {
      conf.ledMode = LedMode::singleColor;
    }
    else if (strcmp(ledMode, "hourlyColor") == 0)
    {
      conf.ledMode = LedMode::hourlyColor;
    }
    else if (strcmp(ledMode, "wordColor") == 0)
    {
      conf.ledMode = LedMode::wordColor;
    }
    else if (strcmp(ledMode, "rainbowColor") == 0)
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
    conf.backgroundDimFactor = json[F("brightness")][F("backgroundDimFactor")].as<uint8_t>();
    const char *brightnessMode = jsonBrightnessMode.as<const char *>();
    if (strcmp(brightnessMode, "fixedBrightness") == 0)
    {
      conf.brightnessMode = BrightnessMode::fixedBrightness;
    }
    else if (strcmp(brightnessMode, "ldrBrightness") == 0)
    {
      conf.brightnessMode = BrightnessMode::ldrBrightness;
    }
    else if (strcmp(brightnessMode, "timeBrightness") == 0)
    {
      conf.brightnessMode = BrightnessMode::timeBrightness;
    }

    JsonObjectConst jsonBrightnessSettings = json[F("brightness")][F("settings")];
    conf.fixedBrightness.brightness = (uint8_t)jsonBrightnessSettings[F("fixedBrightness")][F("brightness")].as<int>();
    conf.ldrBrightness.brightness.min = (uint8_t)jsonBrightnessSettings[F("ldrBrightness")][F("brightness")][F("min")].as<int>();
    conf.ldrBrightness.brightness.max = (uint8_t)jsonBrightnessSettings[F("ldrBrightness")][F("brightness")][F("max")].as<int>();
    conf.timeBrightness.brightness.min = (uint8_t)jsonBrightnessSettings[F("timeBrightness")][F("brightness")][F("min")].as<int>();
    conf.timeBrightness.brightness.max = (uint8_t)jsonBrightnessSettings[F("timeBrightness")][F("brightness")][F("max")].as<int>();
    conf.timeBrightness.timeSlot.startHour = (uint8_t)jsonBrightnessSettings[F("timeBrightness")][F("timeSlot")][F("startHour")].as<int>();
    conf.timeBrightness.timeSlot.endHour = (uint8_t)jsonBrightnessSettings[F("timeBrightness")][F("timeSlot")][F("endHour")].as<int>();

    if (json[F("sensors")])
    {
      JsonArrayConst sensors = json[F("sensors")].as<JsonArrayConst>();
      conf.sensorCount = sensors.size();
      conf.sensors = (Sensor *)malloc(conf.sensorCount * sizeof(Sensor));
      if (!conf.sensors)
      {
        Serial.println("ERROR malloc conf.sensors");
      }
      for (uint8_t i = 0; i < conf.sensorCount; i++)
      {
        JsonObjectConst sensor = sensors[i].as<JsonObjectConst>();
        conf.sensors[i].name = (char *)malloc(SENSORNAME_MAX * sizeof(char));
        strlcpy(conf.sensors[i].name, sensors[i][F("name")], SENSORNAME_MAX);

        conf.sensors[i].attributeCount = sensor[F("attributes")].size();
        conf.sensors[i].attributeNames = (char **)malloc(conf.sensors[i].attributeCount * sizeof(char *));
        if (!conf.sensors[i].attributeNames)
        {
          Serial.println("ERROR malloc conf.sensors[i].attributeNames");
        }
        conf.sensors[i].attributeValues = (uint32_t *)malloc(conf.sensors[i].attributeCount * sizeof(uint32_t));
        if (!conf.sensors[i].attributeValues)
        {
          Serial.println("ERROR malloc conf.sensors[i].attributeValues");
        }
        for (uint8_t j = 0; j < conf.sensors[i].attributeCount; j++)
        {
          JsonObjectConst attribute = sensor[F("attributes")][j].as<JsonObjectConst>();
          conf.sensors[i].attributeNames[j] = (char *)malloc(ATTRIBUTENAME_MAX * sizeof(char));
          if (!conf.sensors[i].attributeNames[j])
          {
            Serial.println("ERROR malloc conf.sensors[i].attributeNames[j]");
          }
          strlcpy(conf.sensors[i].attributeNames[j], attribute[F("name")], ATTRIBUTENAME_MAX);
          conf.sensors[i].attributeValues[j] = attribute[F("value")];
          if ((strcmp(conf.sensors[i].name, "touch") == 0)&&(strcmp(conf.sensors[i].attributeNames[j], "threshold") == 0))
          {
            conf.touchThreshold = conf.sensors[i].attributeValues[j];
          } 
          if ((strcmp(conf.sensors[i].name, "ldr") == 0)&&(strcmp(conf.sensors[i].attributeNames[j], "ldrDark") == 0))
          {
            conf.ldrBrightness.ldrRange.dark = conf.sensors[i].attributeValues[j];
          }
          if ((strcmp(conf.sensors[i].name, "ldr") == 0)&&(strcmp(conf.sensors[i].attributeNames[j], "ldrBright") == 0))
          {
            conf.ldrBrightness.ldrRange.bright = conf.sensors[i].attributeValues[j];
          }
        }
      }
    }
    else
    {
      /// if no sensors are defined, set default values
      conf.sensorCount = 0;
#ifdef HASTOUCHBUTTON
      conf.sensorCount++;
#endif
#ifdef HASLDR
      conf.sensorCount++;
#endif
      conf.availableSensors = (char **)malloc(conf.sensorCount * sizeof(char *));
      if (!conf.availableSensors)
      {
        debug_println("ERROR malloc conf.availableSensors");
      }
      uint8_t i = 0;
#ifdef HASTOUCHBUTTON
      conf.availableSensors[i] = (char *)malloc(10 * sizeof(char));
      if (!conf.availableSensors[i])
      {
        debug_println("ERROR malloc conf.availableSensors[i]");
      }
      strcpy(conf.availableSensors[i], "touch");
      conf.touchThreshold = TOUCH_THRESHOLD;
      i++;
#endif
#ifdef HASLDR
      conf.availableSensors[i] = (char *)malloc(10 * sizeof(char));
      if (!conf.availableSensors[i])
      {
        debug_println("ERROR malloc conf.availableSensors[i]");
      }
      strcpy(conf.availableSensors[i], "ldr");
#endif
      //////////// start sensor code
      conf.sensors = (Sensor *)malloc(conf.sensorCount * sizeof(Sensor));
      if (!conf.sensors)
      {
        debug_println("ERROR malloc conf.sensors");
      }

#ifdef HASTOUCHBUTTON
      conf.sensors[0].name = (char *)malloc(SENSORNAME_MAX * sizeof(char));
      if (!conf.sensors[0].name)
      {
        debug_println("ERROR malloc conf.sensors[0].name");
      }
      strlcpy(conf.sensors[0].name, "touch", SENSORNAME_MAX);
      conf.sensors[0].attributeCount = 1;
      conf.sensors[0].attributeNames = (char **)malloc(conf.sensors[0].attributeCount * sizeof(char *));
      if (!conf.sensors[0].attributeNames)
      {
        debug_println("ERROR malloc conf.sensors[0].attributeNames");
      }
      conf.sensors[0].attributeNames[0] = (char *)malloc(ATTRIBUTENAME_MAX * sizeof(char));
      if (!conf.sensors[0].attributeNames[0])
      {
        debug_println("ERROR malloc conf.sensors[0].attributeNames[0]");
      }
      strlcpy(conf.sensors[0].attributeNames[0], "threshold", ATTRIBUTENAME_MAX);
      conf.sensors[0].attributeValues = (uint32_t *)malloc(conf.sensors[0].attributeCount * sizeof(uint32_t));
      if (!conf.sensors[0].attributeValues)
      {
        debug_println("ERROR malloc conf.sensors[0].attributeValues");
      }
      conf.sensors[0].attributeValues[0] = TOUCH_THRESHOLD;
#endif
#ifdef HASLDR
      conf.sensors[1].name = (char *)malloc(SENSORNAME_MAX * sizeof(char));
      if (!conf.sensors[1].name)
      {
        debug_println("ERROR malloc conf.sensors[1].name");
      }
      strlcpy(conf.sensors[1].name, "ldr", SENSORNAME_MAX);
      conf.sensors[1].attributeCount = 2;
      conf.sensors[1].attributeNames = (char **)malloc(conf.sensors[1].attributeCount * sizeof(char *));
      conf.sensors[1].attributeValues = (uint32_t *)malloc(conf.sensors[1].attributeCount * sizeof(uint32_t));
      if (!conf.sensors[1].attributeNames)
      {
        debug_println("ERROR malloc conf.sensors[1].attributeNames");
      }
      conf.sensors[1].attributeNames[0] = (char *)malloc(ATTRIBUTENAME_MAX * sizeof(char));
      if (!conf.sensors[1].attributeNames[0])
      {
        debug_println("ERROR malloc conf.sensors[1].attributeNames[0]");
      }
      strlcpy(conf.sensors[1].attributeNames[0], "ldrDark", ATTRIBUTENAME_MAX);
      conf.sensors[1].attributeValues[0] = conf.ldrBrightness.ldrRange.dark;
      conf.sensors[1].attributeNames[1] = (char *)malloc(ATTRIBUTENAME_MAX * sizeof(char));
      if (!conf.sensors[1].attributeNames[1])
      {
        debug_println("ERROR malloc conf.sensors[1].attributeNames[1]");
      }
      strlcpy(conf.sensors[1].attributeNames[1], "ldrBright", ATTRIBUTENAME_MAX);
      conf.sensors[1].attributeValues[1] = conf.ldrBrightness.ldrRange.bright;
#endif
    }
    ///////// end sensor code
  }
  else
  {
    debug_println(F("Invalid JSON config"));
  }

  return true;
}

void printConfig(Configuration &conf)
{
  config2JSON(conf, json);
  serializeJson(json, Serial);
}

bool loadConfiguration(Configuration *conf)
{

  debug_println(F("Loading configuration from filesystem"));
  validJSON = false;
  if (!LittleFS.exists("/config.json"))
  {
    debug_println(F("Config file does not exist"));
    return false;
  }
  File configFile = LittleFS.open("/config.json", "r");
  if (configFile)
  {
    auto deserializeError = deserializeJson(json, configFile);
    configFile.close();
    if (!deserializeError)
    {
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
return validJSON;
  // copyConfig(config, oldConfig);
}
bool loadClockface(Configuration *conf)
{

  debug_println(F("Loading clockface from filesystem"));
  validJSON = false;
  if (!LittleFS.exists("/clockface.json"))
  {
    debug_println(F("Clockface file does not exist"));
    return false;
  }
  File configFile = LittleFS.open("/clockface.json", "r");
  if (configFile)
  {
    auto deserializeError = deserializeJson(json, configFile);
    configFile.close();
    if (!deserializeError)
    {
      validJSON = true;
      clockface2config(json, *conf);
    }
    else
    {
      debug_println(deserializeError.c_str());
      debug_println(F("failed to load json clockface"));
    }
  }
  else
  {
    debug_println(F("Cannot open clockface file"));
  }

  if (!validJSON)
  {
    debug_println(F("Invalid clockface JSON"));
    // loadDefaultConfiguration();
  }
return validJSON;
  // copyConfig(config, oldConfig);
}
bool configurationSetup()
{
  bool clockfaceOK=loadClockface(&config);
  bool configOK=loadConfiguration(&config);
  return clockfaceOK && configOK;
}

void saveConfiguration(Configuration &conf)
{

  debug_println(F("Need to save configuration to Filesystem"));
  File configFile = LittleFS.open("/config.json", "w");
  if (!configFile)
  {
    debug_println(F("failed to open config file for writing"));
  }
  config2JSON(conf, json);
  serializeJson(json, configFile);
  configFile.close();
  debug_println(F("Closed file"));
}

void copyConfig(Configuration &confA, Configuration &confB)
{
  memcpy(&confA, &confB, sizeof(Configuration));
}
