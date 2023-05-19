#ifndef MyConfig_h
#define MyConfig_h

// Basic struct written to and being read from EEPROM
// EEPROM config struct
struct {
  char check[6] = "ERROR";
  char ssid[33] = "01234567890123456789012345678901";
  char pass[33] = "01234567890123456789012345678901";
  float cal_windvane = 0.0;
  float cal_compass = 0.0;
} static_config;

String x_config = "invalid"; // Valid config flag

void saveEepromData(String check="UNDEF", String ssid="none", String password="none", float windvaneCal=0.0, float compassCal=0.0) {
  EEPROM.put(0, check);
  EEPROM.put(0+sizeof(check), ssid);
  EEPROM.put(0+sizeof(check)+sizeof(ssid), password);
  EEPROM.put(0+sizeof(check)+sizeof(ssid)+sizeof(password), windvaneCal);
  EEPROM.put(0+sizeof(check)+sizeof(ssid)+sizeof(password)+sizeof(windvaneCal), compassCal);
  EEPROM.commit();
  EEPROM.end();
};

void readEepromData() {
  
};


#endif 
