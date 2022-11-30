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

  uint8_t expected_checksum = calculateConfigChecksum();
  if (config.checksum != expected_checksum) {
    debug_printf("EEPROM checksum not valid, got %d, expected %d\n", config.checksum, expected_checksum);
    loadDefaultConfiguration();
  }
}

void loadDefaultConfiguration() {
  debug_printf("Loading default configuration\n");

  char ntpServer[] = "pool.ntp.org";
  memcpy(config.ntp_server, ntpServer, sizeof(ntpServer));

  char hostname[] = "woordklok";
  memcpy(config.hostname, hostname, sizeof(hostname));

  memcpy(config.clockface, clockface, sizeof(clockface));

  config.ledMode = single;
  config.singleColorHue = -1;
  for (int i = 0; i < sizeof(config.hourlyColors)/sizeof(config.hourlyColors[0]); i++) { config.hourlyColors[i] = random(255); }
  for (int i = 0; i < sizeof(config.wordColors)/sizeof(config.wordColors[0]); i++) { config.wordColors[i] = random(255); }

  config.brightnessMode = ldrBrightness;
  config.ldrDark = 10;
  config.ldrBright = 100;
  config.maxBrightness = 150;
  config.minBrightness = 16;
  config.brightnessStartHour = 8;
  config.brightnessEndHour = 22;
}

uint8_t calculateConfigChecksum() {
  uint8_t previousChecksum = config.checksum;
  config.checksum = 0;

  unsigned char *data = (unsigned char *)&config;
  uint8_t checksum = sizeof(Configuration);
  for (int i = 0; i < sizeof(Configuration); i++) {
    checksum ^= data[i];
  }

  config.checksum = previousChecksum;
  return checksum;
}