#pragma once

#include "headers.h"

#include <ElegantOTA.h>

void handleFileUpload();
void handleDefaultRoot();
bool serverStarted = false;
char *JSONStringRep;

void rebootClock()
{
  static const char index_html[] PROGMEM = R"rawliteral(
  <html>
  <head>
     <script type="text/javascript">      setTimeout(function(){        window.location.href = '/';      }, 5000);    </script> 
  </head>
  <body>
    <p>Rebooting and going back to home ...</p>
    <p>If nothing happens after 5 seconds, go <a href"/">here</a></p>
  </body>
</html>)rawliteral";
  server.send_P(200, "text/html", index_html);
  ESP.restart();
}
void sendResponse(int code, const char *content_type, const char *message)

{
// CORS headers for cross-origin requests
#if defined(ESP8266)
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Methods", "GET, PUT, POST, OPTIONS");
  server.sendHeader("Access-Control-Allow-Headers", "X-Requested-With, Content-Type, Accept");
#endif
  server.send(code, content_type, message);
}

void handleRoot()
{
  server.send(200, "text/plain", "hello from esp8266!\r\n");
}

void handleNotFound()
{
  if (!webserverServeFileFromFS(server.uri()))
  {
    String message = "File Not Found\n\n";
    message += "URI: ";
    message += server.uri();
    message += "\nMethod: ";
    message += (server.method() == HTTP_GET) ? "GET" : "POST";
    message += "\nArguments: ";
    message += server.args();
    message += "\n";
    for (uint8_t i = 0; i < server.args(); i++)
    {
      message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
    }
    server.send(404, "text/plain", message);
  }
}

String webserverGetContentType(String filename)
{
  if (filename.endsWith(".htm"))
    return "text/html";
  else if (filename.endsWith(".html"))
    return "text/html";
  else if (filename.endsWith(".css"))
    return "text/css";
  else if (filename.endsWith(".js"))
    return "application/javascript";
  else if (filename.endsWith(".svg"))
    return "image/svg+xml";
  else if (filename.endsWith(".png"))
    return "image/png";
  else if (filename.endsWith(".gif"))
    return "image/gif";
  else if (filename.endsWith(".jpg"))
    return "image/jpeg";
  else if (filename.endsWith(".ico"))
    return "image/x-icon";
  else if (filename.endsWith(".xml"))
    return "text/xml";
  else if (filename.endsWith(".pdf"))
    return "application/x-pdf";
  else if (filename.endsWith(".zip"))
    return "application/x-zip";
  else if (filename.endsWith(".gz"))
    return "application/x-gzip";
  return "text/html";
}

bool webserverServeFileFromFS(String path)
{
  debug_printf("handleFileRead: %s (%d)\n", path.c_str(), path.length());

  if (path.length() == 0)
    path = "/index.html";
  if (path.endsWith("/"))
    path += "index.html";

  String content_type = webserverGetContentType(path);
  String compressed_path = path + ".gz";
  if (LittleFS.exists(compressed_path) || LittleFS.exists(path))
  {
    if (LittleFS.exists(compressed_path))
      path += ".gz";
    File file = LittleFS.open(path, "r");
    #if defined(ESP8266)
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Methods", "GET, PUT, POST, OPTIONS");
  server.sendHeader("Access-Control-Allow-Headers", "X-Requested-With, Content-Type, Accept");
#endif
    size_t sent = server.streamFile(file, content_type);
    file.close();
    debug_printf("sent file: %s \n", path.c_str());
    debug_print("\tSent file:");
    debug_println(path);
    return true;
  }
  debug_printf("\tFile Not Found: %s\n", path.c_str());
  return false;
}

void apiSendError(String message)
{
  DynamicJsonDocument json(2048);

  JsonObject root = json.createNestedObject();
  root[F("error")] = message;
  debug_printf("API Error: %s \n", message.c_str());
  // server.send(400, "text/plain", message);
  apiSendJSON(400, root);
  json.clear();
}

void apiSendJSON(int status, JsonObject object)
{
  String json;
  serializeJson(object, json);
  sendResponse(status, "application/json", json.c_str());
}

void apiSendJSON(int status, DynamicJsonDocument doc)
{
  JSONStringRep = (char *)malloc(doc.memoryUsage() + 1);
  serializeJson(doc, JSONStringRep, doc.memoryUsage() + 1);
  sendResponse(status, "application/json", JSONStringRep);
  doc.clear();
}

void runTestPixels()
{
  state = TESTPIXEL;
  server.send(200, "text/plain", "Running pixel test");
}

void runTestAllPixels()
{
  state = TESTFULL;
  server.send(200, "text/plain", "Running all pixel test");
}

void runClockFaceTestMinutes()
{
  state = TESTMINUTES;
  server.send(200, "text/plain", "Running clockface minute test");
}

void runClockFaceTestHours()
{
  state = TESTHOURS;
  server.send(200, "text/plain", "Running clockface hour test");
}

void getStatus()
{
  DynamicJsonDocument jsonDoc(256);
  JsonObject root = jsonDoc.createNestedObject();
  root[F("version")] = version;
  char timeString[6];
  sprintf(timeString, "%02d:%02d", dateHours, dateMinutes);
  root[F("currentTime")] = timeString;

  apiSendJSON(200, root);
  jsonDoc.clear();
}


// Due to memory issues serializing/deserializing, file returned from FS
void getConfig()
{
  webserverServeFileFromFS("/config.json");
//  config2JSON(config, json);
//  apiSendJSON(200, json);
}

// Due to memory issues serializing/deserializing, file returned from FS
void getClockface()
{
  webserverServeFileFromFS("/clockface.json");
//  clockface2JSON(config, json);
//  apiSendJSON(200, json);
}

void setLedColor()
{
  bool testConfig = (server.arg("test") == "true" ? true : false);
  String body = server.arg(F("plain"));

  if (testConfig && (state != states::TESTCOLORS))
  {
    debug_println("Test cycle: copy config");
    copyConfig(workingConfig, config);
  }
  if (body.length() > 0)
  {
    // get JSON body out of request & get first key
    DynamicJsonDocument json(768);
    deserializeJson(json, body);
    serializeJsonPretty(json, Serial);
    JsonObject::iterator it = json.as<JsonObject>().begin();
    String ledMode = it->key().c_str();
    // validate
    if ((json[ledMode][F("color")].isNull() && json[ledMode][F("cycleTime")].isNull()) || json[ledMode][F("backgroundColor")].isNull())
    {
      apiSendError("Missing color or backgroundColor");
      return;
    }

    if (ledMode.equals("singleColor"))
    {
      debug_println("Setting single color");
      copyColor(json[ledMode][F("color")], config.singleColor.color);
      copyColor(json[ledMode][F("backgroundColor")], config.singleColor.backgroundColor);
      config.ledMode = LedMode::singleColor;
    }
    else if (ledMode.equals("wordColor"))
    {
      debug_println("Setting word color");
      copyColor(json[ledMode][F("backgroundColor")], config.wordColor.backgroundColor);
      JsonArray array = json[ledMode][F("color")].as<JsonArray>();
      if (array.size() == config.clockfaceLayout.totalWords)
      {
        for (int i = 0; i < config.clockfaceLayout.totalWords; i++)
        {
          copyColor(array[i], config.wordColor.color[i]);
        }
        config.ledMode = LedMode::wordColor;
      }
      else
      {
        debug_printf("Expected %d words, got %d\n", config.clockfaceLayout.totalWords, array.size());
      }
    }
    else if (ledMode.equals("hourlyColor"))
    {
      debug_println("Setting hourly color");
      copyColor(json[ledMode][F("backgroundColor")], config.hourlyColor.backgroundColor);
      JsonArray array = json[ledMode][F("color")].as<JsonArray>();
      if (array.size() == 24)
      {
        for (int i = 0; i < 24; i++)
        {
          copyColor(array[i], config.hourlyColor.color[i]);
        }
        config.ledMode = LedMode::hourlyColor;
      }
      else
      {
        debug_printf("Expected %d colors, got %d\n", 24, array.size());
      }
    }
    else if (ledMode.equals("rainbowColor"))
    {
      debug_println("Setting rainbowcolor");
      copyColor(json[ledMode][F("backgroundColor")], config.rainbowColor.backgroundColor);
      config.rainbowColor.cycleTime = json[ledMode][F("cycleTime")];
      config.ledMode = LedMode::rainbowColor;
    }
    else
    {
      apiSendError("Unknown mode, valid options: 'singleColor', 'hourlyColor', 'rainbowColor', 'wordColor'");
      return;
    }
    if (testConfig)
    {
      debug_println("Test cycle: change state to TESTCOLORS");
      changeState(states::TESTCOLORS);
    }
    else
    {

      saveConfiguration(config);
    }
  }
  getConfig();
}

void setBrightness()
{
  bool testConfig = (server.arg("test") == "true" ? true : false);
  String body = server.arg(F("plain"));

  if (testConfig)
  {
    debug_println("Testing config");
  }
  if (body.length() > 0)
  {
    // get JSON body out of request & get first key
    DynamicJsonDocument json(768);
    deserializeJson(json, body);
    serializeJsonPretty(json, Serial);
    String brightnessMode = "";

    for (JsonPair kv : json.as<JsonObject>())
    {
      if (strcmp(kv.key().c_str(), "fixedBrightness") == 0 ||
          strcmp(kv.key().c_str(), "ldrBrightness") == 0 ||
          strcmp(kv.key().c_str(), "timeBrightness") == 0)
      {
        brightnessMode = kv.key().c_str();
        break;
      }
    }

    // validate
    if (json[brightnessMode][F("brightness")].isNull())
    {
      String errorMessage = "Missing brightness parameter in mode: ";
      errorMessage += brightnessMode;
      apiSendError(errorMessage);
      return;
    }
    if (!json[F("backgroundDimFactor")].isNull())
    {
      config.backgroundDimFactor = json[F("backgroundDimFactor")].as<uint8_t>();
    }
    if (brightnessMode.equals("fixedBrightness"))
    {
      debug_println("Setting fixed brightness");
      config.fixedBrightness.brightness = json[brightnessMode][F("brightness")].as<int>();
      config.brightnessMode = BrightnessMode::fixedBrightness;
    }
    else if (brightnessMode.equals("ldrBrightness"))
    {
      debug_println("Setting ldr brightness");
      config.ldrBrightness.ldrRange.dark = json[brightnessMode][F("ldrRange")][F("dark")].as<int>();
      config.ldrBrightness.ldrRange.bright = json[brightnessMode][F("ldrRange")][F("bright")].as<int>();
      for (uint8_t i = 0; i < config.sensorCount; i++)
      {
        if (strcmp(config.sensors[i].name, "ldr") == 0)
        {
          for (uint8_t j = 0; j < config.sensors[i].attributeCount; j++)
          {
            if (strcmp(config.sensors[i].attributeNames[j], "ldrDark") == 0)
            {
              config.sensors[i].attributeValues[j] = config.ldrBrightness.ldrRange.dark;
            }
            if (strcmp(config.sensors[i].attributeNames[j], "ldrBright") == 0)
            {
              config.sensors[i].attributeValues[j] = config.ldrBrightness.ldrRange.bright;
            }
          }
          break;
        }
      }

      config.ldrBrightness.brightness.min = json[brightnessMode][F("brightness")][F("min")].as<int>();
      config.ldrBrightness.brightness.max = json[brightnessMode][F("brightness")][F("max")].as<int>();
      config.brightnessMode = BrightnessMode::ldrBrightness;
    }
    else if (brightnessMode.equals("timeBrightness"))
    {
      debug_println("Setting time brightness");
      config.timeBrightness.brightness.min = json[brightnessMode][F("brightness")][F("min")].as<int>();
      config.timeBrightness.brightness.max = json[brightnessMode][F("brightness")][F("max")].as<int>();
      config.timeBrightness.timeSlot.startHour = json[brightnessMode][F("timeSlot")][F("startHour")].as<int>();
      config.timeBrightness.timeSlot.endHour = json[brightnessMode][F("timeSlot")][F("endHour")].as<int>();
      config.brightnessMode = BrightnessMode::timeBrightness;
    }
    else
    {
      apiSendError("Unknown mode, valid options: 'fixedBrightness', 'ldrBrightness', 'timeBrightness'");
      return;
    }
  }
  saveConfiguration(config);
  getConfig();
}

void setSensors()
{
  String body = server.arg(F("plain"));
  if (body.length() > 0)
  {
    DynamicJsonDocument json(768);
    deserializeJson(json, body);
    serializeJsonPretty(json, Serial);
    if (json["sensors"].isNull())
    {
      apiSendError("Missing sensors parameter");
      return;
    }
    JsonArray array = json["sensors"].as<JsonArray>();
    if (array.size() == config.sensorCount)
    {
      JsonArrayConst sensors = json[F("sensors")].as<JsonArrayConst>();
      config.sensors = (Sensor *)malloc(config.sensorCount * sizeof(Sensor));
      if (!config.sensors)
      {
        Serial.println("ERROR malloc conf.sensors");
      }
      for (uint8_t i = 0; i < config.sensorCount; i++)
      {
        JsonObjectConst sensor = sensors[i].as<JsonObjectConst>();
        config.sensors[i].name = (char *)malloc(SENSORNAME_MAX * sizeof(char));
        strlcpy(config.sensors[i].name, sensors[i][F("name")], SENSORNAME_MAX);

        config.sensors[i].attributeCount = sensor[F("attributes")].size();
        config.sensors[i].attributeNames = (char **)malloc(config.sensors[i].attributeCount * sizeof(char *));
        if (!config.sensors[i].attributeNames)
        {
          Serial.println("ERROR malloc conf.sensors[i].attributeNames");
        }
        config.sensors[i].attributeValues = (uint32_t *)malloc(config.sensors[i].attributeCount * sizeof(uint32_t));
        if (!config.sensors[i].attributeValues)
        {
          Serial.println("ERROR malloc conf.sensors[i].attributeValues");
        }
        for (uint8_t j = 0; j < config.sensors[i].attributeCount; j++)
        {
          JsonObjectConst attribute = sensor[F("attributes")][j].as<JsonObjectConst>();
          config.sensors[i].attributeNames[j] = (char *)malloc(ATTRIBUTENAME_MAX * sizeof(char));
          if (!config.sensors[i].attributeNames[j])
          {
            Serial.println("ERROR malloc conf.sensors[i].attributeNames[j]");
          }
          strlcpy(config.sensors[i].attributeNames[j], attribute[F("name")], ATTRIBUTENAME_MAX);
          config.sensors[i].attributeValues[j] = attribute[F("value")];
          if ((strcmp(config.sensors[i].name, "touch") == 0) && (strcmp(config.sensors[i].attributeNames[j], "threshold") == 0))
          {
            config.touchThreshold = config.sensors[i].attributeValues[j];
          }
          if ((strcmp(config.sensors[i].name, "ldr") == 0) && (strcmp(config.sensors[i].attributeNames[j], "ldrDark") == 0))
          {
            config.ldrBrightness.ldrRange.dark = config.sensors[i].attributeValues[j];
          }
          if ((strcmp(config.sensors[i].name, "ldr") == 0) && (strcmp(config.sensors[i].attributeNames[j], "ldrBright") == 0))
          {
            config.ldrBrightness.ldrRange.bright = config.sensors[i].attributeValues[j];
          }
        }
      }
    }
    else
    {
      char errorMessage[64];
      snprintf(errorMessage, sizeof(errorMessage), "Expected %d sensors, got %d\n", config.sensorCount, array.size());
      apiSendError(errorMessage);
    }
  }
  saveConfiguration(config);
  getConfig();
  //   server.send(200, "text/plain", "OK");
}

void getSensorValue()
{
  DynamicJsonDocument json(256);
  JsonObject root = json.createNestedObject();
  if (!server.hasArg("sensor"))
  {
    apiSendError("Missing 'sensor' argument");
    return;
  }
  char sensorArg[10];
  server.arg("sensor").toCharArray(sensorArg, sizeof(sensorArg));
  bool sensorFound = false;
  for (int i = 0; i < config.sensorCount; i++)
  {
    if (strcmp(sensorArg, config.sensors[i].name) == 0)
    {
      sensorFound = true;
      break;
    }
  }
  if (!sensorFound)
  {
    apiSendError("Sensor not found in configuration");
    return;
  }
  uint32_t sensorValue = readSensor(sensorArg);
  // debug_printf("Sensor %s value: %d\n", sensorArg, sensorValue);
  root["sensorValue"] = sensorValue;
  apiSendJSON(200, root);
  json.clear();
}
void webServerSetup()
{
  debug_println("Webserver setup");
  if (!serverStarted)
  {
#if defined(ESP32)
    server.enableCORS();
#endif
    server.on("/api/TestAllPixels", runTestAllPixels);
    server.on("/api/TestPixels", runTestPixels);
    server.on("/api/testClockFaceMinutes", runClockFaceTestMinutes);
    server.on("/api/testClockFaceHours", runClockFaceTestHours);
    server.on("/api/configWifi", configWifi);

    server.on("/api/status", getStatus);
    server.on("/api/config", getConfig);
    server.on("/api/clockface", getClockface);
    server.on("/api/color", setLedColor);
    server.on("/api/brightness", setBrightness);
    server.on("/api/getSensorValue", getSensorValue);
    server.on("/api/sensors", setSensors);
    server.on("/api/reboot", rebootClock);
    server.on("/api/uploadConfig", HTTP_POST, // if the client posts to the upload page
              []() {},
              handleFileUpload // Receive and save the file
    );
    if (!LittleFS.exists("/index.html"))
    {
      server.on("/", handleDefaultRoot); // if the root page is requested, send the index.html file
    }

    // server.on("/api/log", getLog);
    server.onNotFound(handleNotFound);

    ElegantOTA.onEnd([](bool success)
                     {
                      if (success) {
                        debug_println("Update succesful");
                        delay(3000);
                        rebootClock();
                      } else {
                        debug_println("Error updating");
                      } 
                    });
    ElegantOTA.begin(&server);
    server.begin();
    debug_printf("HTTP server started on %s\n", WiFi.localIP().toString().c_str());
    // Serial.println("HTTP server started\n");
    serverStarted = true;
  }
}

void stopServer()
{
  server.stop();
}
/*
void getLog(){
  flushLog();
  String message(logBufferString);
  server.send(200,"text/plain",message);
}*/

void webServerLoop()
{
  if (serverStarted)
  {
    server.handleClient();
#if defined(ESP8266)
    MDNS.update();
#endif
  }
}

void handleFileUpload()
{ // upload a new file to the SPIFFS
  HTTPUpload &upload = server.upload();
  static File fsUploadFile; // Static file object to hold the file being uploaded
  if (upload.status == UPLOAD_FILE_START)
  {
    String filename = upload.filename;
    if (!(filename.equals("config.json") || filename.equals("clockface.json")))
    {
      Serial.println("Invalid file name for upload, only config.json and clockface.json are allowed");
      server.send(400, "text/plain", "400: invalid file name");
      return;
    }
    if (!filename.startsWith("/"))
      filename = "/" + filename;

    Serial.print("handleFileUpload Name: ");
    Serial.println(filename);
    fsUploadFile = LittleFS.open(filename, "w+"); // Open the file for writing (overwrite if it exists)
  }
  else if (upload.status == UPLOAD_FILE_WRITE)
  {
    if (fsUploadFile)
      fsUploadFile.write(upload.buf, upload.currentSize); // Write the received bytes to the file
  }
  else if (upload.status == UPLOAD_FILE_END)
  {
    if (fsUploadFile)
    {                       // If the file was successfully created
      fsUploadFile.close(); // Close the file again
      Serial.print("handleFileUpload Size: ");
      Serial.println(upload.totalSize);
      //server.send(200, "text/plain", "File loaded"); // Redirect the client to the success page
      rebootClock();
    }
    else
    {
      Serial.println("File upload failed");
      server.send(500, "text/plain", "500: couldn't create file");
    }
  }
}

void handleDefaultRoot()
{
  static const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <title>Wordclock initial config</title>
</head>
<body>
    <h1>Config files to upload</h1>
    <p>Check which json files are present and upload a new one. Reboot after finishing the upload. You can also upload a full filesystem image.</p>
    <h2><a href="/clockface.json" >Clockface</a></h2>
    <form method="POST" action="/api/uploadConfig" enctype="multipart/form-data">
        <input type="file" name="clockface.json" accept=".json">
        <input class="button" type="submit" value="Upload Clockface">
    </form>
    <h2><a href="/config.json" >Configuration</a></h2>
        <form method="POST" action="/api/uploadConfig" enctype="multipart/form-data">
        <input type="file" name="config.json" accept=".json">
        <input class="button" type="submit" value="Upload config">
    </form>
            <form method="POST" action="/api/reboot" enctype="multipart/form-data">
        <input class="button" type="submit" value="Reboot">
</form>
    <h1>Firmware/Image upload</h1>
    <iframe src="/update" style="width: 100%; height: 500px;"></iframe>
</body>
</html>
)rawliteral";
  server.send_P(200, "text/html", index_html);
}