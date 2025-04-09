#pragma once

#include <headers.h>

ESP8266WebServer server(80);
ESP8266HTTPUpdateServer flashUpdateServer;
ESP8266HTTPUpdateServer fsUpdateServer;
bool serverStarted = false;
char* JSONStringRep;

void sendResponse(int code, const char *content_type, const char *message)
{
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Methods", "GET, PUT, POST, OPTIONS");
  server.sendHeader("Access-Control-Allow-Headers", "X-Requested-With, Content-Type, Accept");
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

void dumpLittleFS()
{
  Dir dir = LittleFS.openDir("");
  while (dir.next())
  {
    debug_printf("%s, (%d)\n", dir.fileName().c_str(), dir.fileSize());
  }
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
  // server.send(status, "application/json", json);
  sendResponse(status, "application/json", json.c_str());
}

void apiSendJSON(int status, DynamicJsonDocument doc)
{
  JSONStringRep=(char *)malloc(doc.memoryUsage()+1);
  serializeJson(doc, JSONStringRep,doc.memoryUsage()+1);
  //debug_printf("Sending JSON %s\n", JSONStringRep);
  // server.send(status, "application/json", json);
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

void getConfig()
{
  config2JSON(config,json);
  apiSendJSON(200,json);
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
      copyColor(json[ledMode][F("color")],config.singleColor.color);
      copyColor(json[ledMode][F("backgroundColor")],config.singleColor.backgroundColor);
      config.ledMode = LedMode::singleColor;
    }
    else if (ledMode.equals("wordColor"))
    {
      debug_println("Setting word color");
      copyColor(json[ledMode][F("backgroundColor")],config.wordColor.backgroundColor);
      JsonArray array = json[ledMode][F("color")].as<JsonArray>();
      if (array.size() == config.clockfaceLayout.totalWords)
      {
        for (int i = 0; i < config.clockfaceLayout.totalWords; i++)
        {
          copyColor(array[i],config.wordColor.color[i]);
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
      copyColor(json[ledMode][F("backgroundColor")],config.hourlyColor.backgroundColor);
      JsonArray array = json[ledMode][F("color")].as<JsonArray>();
      if (array.size() == 24)
      {
        for (int i = 0; i < 24; i++)
        {
          copyColor(array[i],config.hourlyColor.color[i]);
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
      copyColor(json[ledMode][F("backgroundColor")],config.rainbowColor.backgroundColor);
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
    JsonObject::iterator it = json.as<JsonObject>().begin();
    String brightnessMode = it->key().c_str();
    // validate
    if (json[brightnessMode][F("brightness")].isNull())
    {
      apiSendError("Missing brightness parameter");
      return;
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
      config.ldrBrightness.brightness.min = json[brightnessMode][F("brightness")][F("min")].as<int>();
      config.ldrBrightness.brightness.min = json[brightnessMode][F("brightness")][F("max")].as<int>();
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
  getConfig();
}

void webServerSetup()
{
  debug_println("Webserver setup");
  if (!serverStarted)
  {
    // dumpLittleFS();
    server.on("/api/TestAllPixels", runTestAllPixels);
    server.on("/api/TestPixels", runTestPixels);
    server.on("/api/testClockFaceMinutes", runClockFaceTestMinutes);
    server.on("/api/testClockFaceHours", runClockFaceTestHours);
    server.on("/api/configWifi", configWifi);

    server.on("/api/status", getStatus);
    server.on("/api/config", getConfig);
    server.on("/api/color", setLedColor);
    server.on("/api/brightness", setBrightness);

    // server.on("/api/log", getLog);
    server.onNotFound(handleNotFound);

    flashUpdateServer.setup(&server, String("/api/update/flash"));
    fsUpdateServer.setup(&server, String("/api/update/file"));
    server.begin();
    debug_printf("HTTP server started\n");
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
    MDNS.update();
  }
}
