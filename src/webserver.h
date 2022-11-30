#pragma once 

#include <headers.h>

ESP8266WebServer server(80);
ESP8266HTTPUpdateServer flashUpdateServer;
ESP8266HTTPUpdateServer fsUpdateServer;
bool serverStarted=false;
StaticJsonDocument<10000> jsonBuffer;

void handleRoot() {
  server.send(200, "text/plain", "hello from esp8266!\r\n");
}

void handleNotFound() {
  if (!webserverServeFileFromFS(server.uri())) {
    String message = "File Not Found\n\n";
    message += "URI: ";
    message += server.uri();
    message += "\nMethod: ";
    message += (server.method() == HTTP_GET) ? "GET" : "POST";
    message += "\nArguments: ";
    message += server.args();
    message += "\n";
    for (uint8_t i = 0; i < server.args(); i++) {
      message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
    }
    server.send(404, "text/plain", message);
  }

}

String webserverGetContentType(String filename) {
  if (filename.endsWith(".htm")) return "text/html";
  else if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".svg")) return "image/svg+xml";
  else if (filename.endsWith(".png")) return "image/png";
  else if (filename.endsWith(".gif")) return "image/gif";
  else if (filename.endsWith(".jpg")) return "image/jpeg";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  else if (filename.endsWith(".xml")) return "text/xml";
  else if (filename.endsWith(".pdf")) return "application/x-pdf";
  else if (filename.endsWith(".zip")) return "application/x-zip";
  else if (filename.endsWith(".gz")) return "application/x-gzip";
  return "text/html";
}

bool webserverServeFileFromFS(String path) {
  debug_printf("handleFileRead: %s (%d)\n",path,path.length());

  if (path.length() == 0) path = "/index.html";
  if (path.endsWith("/")) path += "index.html";

  String content_type = webserverGetContentType(path);
  String compressed_path = path + ".gz";
  if (LittleFS.exists(compressed_path) || LittleFS.exists(path)) {
    if (LittleFS.exists(compressed_path)) path+=".gz";
    File file = LittleFS.open(path, "r");
    size_t sent = server.streamFile(file, content_type);
    file.close();
    debug_printf("sent file: %s \n",path);
    debug_print("\tSent file:");
    debug_println(path);
    return true;
  }
  debug_printf("\tFile Not Found: %s\n", path);
  return false;
}

void dumpLittleFS() {
  Dir dir = LittleFS.openDir("");
  while (dir.next ()) {
    debug_printf("%s, (%d)\n",dir.fileName(),dir.fileSize());
  }
}

void setBrightness() {
  int brightness = server.arg("brightness").toInt()%256;
  String message="Setting brightness to ";
  message+=brightness;
  message+="\n";
  debug_printf("%s",message);
  server.send(200, "text/plain", message);
  config.maxBrightness=brightness;
}

void setLedMode() {
  String arg = server.arg("ledmode");
  String message="Setting ledMode to ";
  message+=arg;
  message+="\n";
  debug_printf("%s",message);
  server.send(200, "text/plain", message);
  if(arg == "single") {
    config.ledMode = single;
  } else if (arg == "words") {
    config.ledMode = words;
  } else if (arg == "hourly") {
    config.ledMode = hourly;
  } else if (arg == "rainbow") {
    config.ledMode = rainbow;
  } else {
    apiSendError("Unknown mode, valid options: 'single', 'hourly', 'rainbow', 'words'");
    return;
  }
}

void apiSendError(String message) {
  JsonObject root = jsonBuffer.createNestedObject();
  root["error"] = message;
  //server.send(400, "text/plain", message);
  apiSendJSON(400, root);
}

void apiSendJSON(int status, JsonObject object) {
  String json;
 serializeJson(object, json);
  server.send(status, "application/json", json); 
  jsonBuffer.clear();
}

/*void apiSendJSONArray(int status, JsonArray object) {
  String json;
  serializeJson(object, json);
  server.send(status, "application/json", json); 
  jsonBuffer.clear();
}*/

void setPixelCount() {
  //PixelCount = max((int)server.arg("PixelCount").toInt(), 4);
  String message="Setting number of pixels to ";
  message+=PixelCount;
  message+="\n";
  debug_printf("%s",message);
  server.send(200, "text/plain", message);
  runTestPixels();
}

void runTestPixels() {
  state=TESTPIXEL;
  server.send(200, "text/plain", "Running pixel test");
}

void runTestAllPixels() {
state=TESTFULL;
server.send(200, "text/plain", "Running all pixel test");
}

void runClockFaceTestMinutes() {
  state=TESTMINUTES;
  server.send(200, "text/plain", "Running clockface minute test");
}

void runClockFaceTestHours() {
  state=TESTHOURS;
  server.send(200, "text/plain", "Running clockface hour test");
}

void webServerSetup() {
  debug_println("Webserver setup");
  if (!serverStarted){
  dumpLittleFS();
  server.on("/api/PixelCount", setPixelCount);
  server.on("/api/TestAllPixels", runTestAllPixels);
  server.on("/api/TestPixels", runTestPixels);
  server.on("/api/configWifi", configWifi);
  server.on("/api/setBrightness", setBrightness);
  server.on("/api/setLedMode",setLedMode);
  server.on("/api/testClockFaceMinutes", runClockFaceTestMinutes);
  server.on("/api/testClockFaceHours", runClockFaceTestHours);
  //server.on("/api/log", getLog);
  server.onNotFound(handleNotFound);

  flashUpdateServer.setup(&server, String("/api/update/flash"));
  fsUpdateServer.setup(&server, String("/api/update/file"));
  server.begin();
  debug_printf("HTTP server started\n");
  //Serial.println("HTTP server started\n");
  serverStarted=true;
  }

}

void stopServer(){
  server.stop();
}
/*
void getLog(){
  flushLog();
  String message(logBufferString);
  server.send(200,"text/plain",message);
}*/

void webServerLoop() {
  if (serverStarted){
    server.handleClient();
    MDNS.update();
  }
}
