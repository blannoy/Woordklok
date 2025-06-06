#pragma once 

#include "headers.h"

#include <WiFiClient.h>
#if defined(ESP8266)
#include <ESP8266mDNS.h>
#endif

#if defined(ESP32)
#include <ESPmDNS.h>
#include <WiFi.h>
#endif


#include <WiFiManager.h>

WiFiManager wifiManager;
void APModeCallback (WiFiManager* myWiFiManager);

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
  statusLed(WIFI, blue);
  //setStatusLeds();
  stopServer();
  serverStarted=false;
}

void wifiSetup() {
  debug_println("wifiSetup");
  
  wifiManager.setAPCallback(APModeCallback);
  wifiManager.setSaveConfigCallback(saveConfigCallback);
 wifiManager.setConnectTimeout(180);
if (!strcmp(static_ip,"")){
  IPAddress _ip, _gw, _sn, _dns;
  _ip.fromString(static_ip);
  _gw.fromString(static_gw);
  _sn.fromString(static_sn);
  _dns.fromString(static_dns1);
   wifiManager.setSTAStaticIPConfig(_ip, _gw, _sn, _dns);

}
  //set static ip
  WiFi.mode(WIFI_STA);

  // in order to avoid issues with Assu router, we need to set the phy mode to 11g
  #if defined(ESP8266)
  WiFi.setPhyMode(WIFI_PHY_MODE_11G); 
  #endif
  #if defined(ESP32)
  esp_wifi_set_protocol(WIFI_IF_STA, WIFI_PROTOCOL_11G);
  #endif
  
  wifiManager.setShowStaticFields(true);
  wifiManager.setShowDnsFields(true); 

  bool res;
  res = wifiManager.autoConnect("Woordklok-AP");
  if (!res) {
    debug_print("Failed to connect");
  }
  else {
    //if you get here you have connected to the WiFi
    debug_println("connected...yeey :)");
    statusLed(WIFI,green);
    //wifiConnected=true;
    if (MDNS.begin(config.hostname)) {
      debug_println("MDNS responder started");
    }
  }
  
}
void wifiLoop(){
  
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
      ESP.restart();
      delay(5000);
}
