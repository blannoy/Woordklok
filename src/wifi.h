#pragma once 

#include <headers.h>

WiFiManager wifiManager;
void APModeCallback (WiFiManager* myWiFiManager);

//default custom static IP

/*char static_ip[16] = "192.168.0.55";
char static_gw[16] = "192.168.0.1";
char static_sn[16] = "255.255.255.0";
char static_dns1[16] = "8.8.8.8";
char static_dns2[16] = "8.8.4.4";*/
char static_ip[16] = "";
char static_gw[16] = "";
char static_sn[16] = "";
char static_dns1[16] = "";
char static_dns2[16] = "";
bool wifiConnected=false;
//flag for saving data
bool shouldSaveConfig = false;

//callback notifying us of the need to save config
void saveConfigCallback () {
  debug_print("Should save config");
  shouldSaveConfig = true;
}

void APModeCallback (WiFiManager* myWiFiManager) {
  Serial.println("Entered config mode");
  wifiConnected=false;
  //setStatusLeds();
  stopServer();
  serverStarted=false;
}

void wifiSetup() {
  debug_println("wifiSetup");
  // Read configfile
  if (LittleFS.exists("/config.json")) {
    //file exists, reading and loading
    debug_print("reading config file");
    File configFile = LittleFS.open("/config.json", "r");
    if (configFile) {
      debug_print("opened config file");
      size_t size = configFile.size();
      // Allocate a buffer to store contents of the file.
      std::unique_ptr<char[]> buf(new char[size]);

      configFile.readBytes(buf.get(), size);
#if defined(ARDUINOJSON_VERSION_MAJOR) && ARDUINOJSON_VERSION_MAJOR >= 6
      DynamicJsonDocument json(1024);
      auto deserializeError = deserializeJson(json, buf.get());
      serializeJson(json, Serial);
      if ( ! deserializeError ) {
#else
      DynamicJsonBuffer jsonBuffer;
      JsonObject& json = jsonBuffer.parseObject(buf.get());
      json.printTo(Serial);
      if (json.success()) {
#endif
        debug_print("\nparsed json");
        if (json["ip"]) {
          debug_print("setting custom ip from config");
          strcpy(static_ip, json["ip"]);
          strcpy(static_gw, json["gateway"]);
          strcpy(static_sn, json["subnet"]);
          strcpy(static_dns1, json["dns1"]);
          strcpy(static_dns2, json["dns2"]);
          debug_print(static_ip);
        } else {
          debug_print("no custom ip in config");
        }
      } else {
        debug_print("failed to load json config");
      }
    }
  }
  wifiManager.setAPCallback(APModeCallback);
 // wifiManager.setConfigPortalBlocking(false);
  wifiManager.setSaveConfigCallback(saveConfigCallback);
 wifiManager.setConnectTimeout(180);
if (static_ip != ""){
  IPAddress _ip, _gw, _sn, _dns;
  _ip.fromString(static_ip);
  _gw.fromString(static_gw);
  _sn.fromString(static_sn);
  _dns.fromString(static_dns1);
   wifiManager.setSTAStaticIPConfig(_ip, _gw, _sn, _dns);

}
  //set static ip
  WiFi.mode(WIFI_STA);
  WiFi.setPhyMode(WIFI_PHY_MODE_11G); 
  //delay(5000);
  wifiManager.setShowStaticFields(true);
  wifiManager.setShowDnsFields(true); 
  //wifiManager.resetSettings();

  bool res;
  res = wifiManager.autoConnect("Woordklok-AP");
  if (!res) {
    debug_print("Failed to connect");
    // ESP.restart();
  }
  else {
    //if you get here you have connected to the WiFi
    debug_println("connected...yeey :)");
    //statusLed(WIFI,green);
    //wifiConnected=true;
    if (MDNS.begin("woordklok2")) {
      debug_println("MDNS responder started");
    }
  }
  //save the custom parameters to FS
  if (shouldSaveConfig) {
    debug_print("saving config");
 #if defined(ARDUINOJSON_VERSION_MAJOR) && ARDUINOJSON_VERSION_MAJOR >= 6
    DynamicJsonDocument json(1024);
#else
    DynamicJsonBuffer jsonBuffer;
    JsonObject& json = jsonBuffer.createObject();
#endif
    json["ip"] = WiFi.localIP().toString();
    json["gateway"] = WiFi.gatewayIP().toString();
    json["subnet"] = WiFi.subnetMask().toString();
    json["dns1"] = WiFi.dnsIP().toString();
    json["dns2"] = WiFi.dnsIP(1).toString();
    
    File configFile = LittleFS.open("/config.json", "w");
    if (!configFile) {
      debug_print("failed to open config file for writing");
    }

 #if defined(ARDUINOJSON_VERSION_MAJOR) && ARDUINOJSON_VERSION_MAJOR >= 6
    serializeJson(json, Serial);
    serializeJson(json, configFile);
#else
    json.printTo(Serial);
    json.printTo(configFile);
#endif
    configFile.close();
    //end save
  }
}
void wifiLoop(){
  //wifiManager.process();
  
  if (WiFi.status() == WL_CONNECTED && (state==BOOT || state==STARTUP) && !wifiConnected){
    debug_println("WIFI OK");
    //TODO setStatusLeds();
    wifiConnected=true;
    webServerSetup();
  }
}
void configWifi(){
        server.send(200, "text/plain", "Starting wifi config portal");
        wifiManager.resetSettings();
        if (LittleFS.exists("/config.json")) {
          LittleFS.remove("/config.json");
         }
      ESP.restart();
      delay(5000);
}
