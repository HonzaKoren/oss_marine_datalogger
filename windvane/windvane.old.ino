//
//    FILE: windvane.ino
//  AUTHOR: Jan Kořínek
// PURPOSE: Run windvane/anemometer/DHT22 sensors
//    DATE: 2023-02-18
// Sensors: AS5600 - Magnetic rotationary encoder
//          DHT22 - Temperature/humidity sensor
//          HMC5883L - magnetic compas
//          

#include <Adafruit_HMC5883_U.h>
#include <Adafruit_Sensor.h>
#include <AS5600.h>
#include <DHT.h>
#include <EEPROM.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <ESP8266WebServer-impl.h>
#include <ESP8266WebServerSecure.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h> 
#include <Parsing-impl.h>
#include <RH_ASK.h>
#include <SPI.h> // Not actualy used but needed to compile
#include <Uri.h>
#include <WiFiClient.h>
#include <Wire.h>

#include "my_config.h"

// Default WiFi AP config
#define WIFI_SSID "meteoprobe"
#define WIFI_PASS "set_me_up"

// Set up windvane sensor
AS5600 as5600;   //  use default Wire

// Set up DHT digital pin
#define DHTPIN D4     // Digital pin connected to the DHT sensor
#define DHTTYPE    DHT22     // DHT 22 (AM2302)
DHT dht(DHTPIN, DHTTYPE);

// Set up compass
Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);

// Set up WiFi HTTP server
// WiFiServer server(80);
ESP8266WebServer server(80);

// void handleRoot();              // function prototypes for HTTP handlers
// void handleNotFound();

void setup()
{
  /* First try to read config array from EEPROM
     Config params:
     - WiFi SSID
     - WiFi password
     - compas clibration
     - windvane calibration
  */
  EEPROM.begin(512); // control string, SSID and password up to 32 byte and two 64-bit numbers for calibrations
  Serial.begin(115200);

  Serial.println("");
  Serial.println("Run Forrest, run!");

  // WiFi connection or AP start
  EEPROM.get(0, static_config);

  if (static_config.check == "XchkX") {
    x_config = "valid";
    Serial.println("Found valid local config.");
  } else {
    Serial.println("Valid local config not found, starting in AP mode.");
  };

  Serial.println("SSID " + String(static_config.ssid));
  Serial.println("Password " + String(static_config.pass));
  Serial.println("Windvane offset " + String(static_config.cal_windvane));
  Serial.println("Compass offset " + String(static_config.cal_compass));
  WiFi.begin("koreni", "heslicko");             // Connect to the network

  int i = 0;
  while (WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
    Serial.println('Waiting for WiFi to connect');
    delay(1000);
  }

  server.begin();
  dht.begin();
  mag.begin();

  as5600.begin(4);  //  set direction pin.
  as5600.setDirection(AS5600_CLOCK_WISE);  // default, just be explicit.
  int b = as5600.isConnected();
}

void http_default(WiFiClient xcl)
{
    xcl.println("HTTP/1.1 200 OK");
    xcl.println("Content-Type: text/html");
    xcl.println(""); //  do not forget this one
    xcl.println("<!DOCTYPE html>");
    xcl.println("<html><head><title>OSS Marine Logger Wind Sensor</title><head/><body>");
    xcl.println("<a href=\"/metrics/\">Metrics</a><br>");
    xcl.println("<a href=\"/health/\">Health</a><br>");
    xcl.println("<a href=\"/config/\">Configuration</a><br>");
    xcl.println("<a href=\"/reset/\">Reset to factory defaults</a><br>");
    xcl.println("<p>Configuration stored on board:<br>");
    xcl.println("<ul>");
    xcl.println("<li>WiFi SSID " + String(static_config.ssid) + "</li>");
    xcl.println("<li>WiFi password " + String(static_config.pass) + "</li>");
    xcl.println("<li>Windvane calibration offset " + String(static_config.cal_windvane) + "</li>");
    xcl.println("<li>Compas calibration offset " + String(static_config.cal_compass) + "</li>");
    xcl.println("</ul>");
    xcl.println("Configuration " + String(x_config) + "!</p>");
    xcl.println("<body/></html>");
}

void http_reset(WiFiClient xcl)
{
    Serial.println("Writing default config to EEPROM");
    saveEepromData();
    xcl.println("HTTP/1.1 200 OK");
    xcl.println("Content-Type: text/html");
    xcl.println(""); //  do not forget this one
    xcl.println("<!DOCTYPE html>");
    xcl.println("<html><head><meta http-equiv=\"refresh\" content=\"5; url=/\"><title>OSS Marine Logger Wind Sensor - Factory reset successfull</title><head/><body>");
    xcl.println("Reset to factory defaults successfull!");
    xcl.println("<body/></html>");
}

void loop()
{
  WiFiClient client = server.available();
  if (!client) {
    // No request, no response
    return;
  }
  sensors_event_t magData; 
  mag.getEvent(&magData);
  float azimuth = atan2(magData.magnetic.y, magData.magnetic.x);
  if(azimuth < 0)
    azimuth += 2*PI;
  if(azimuth > 2*PI)
    azimuth -= 2*PI;
  
  // Azimuth calculation in degrees
  float azimuthDg = azimuth * 180/M_PI; 

  // Wind direction
  float apparentWind = as5600.rawAngle() * AS5600_RAW_TO_DEGREES;
  
  // DHT22 temperature
  float temperature = dht.readTemperature();

  // Read Humidity
  float humidity = dht.readHumidity();

  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  if (request.indexOf("/metrics/") != -1){
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/plain");
      client.println(""); //  do not forget this one
      client.print("humidity ");
      client.println(humidity);
      client.print("temperature ");
      client.println(temperature);
      client.print("direction ");
      client.println(azimuthDg);
      client.print("apparent_wind ");
      client.println(apparentWind);
  } else if (request.indexOf("/reset/") != -1) {
    http_reset(client);
  } else {
    http_default(client);
  }
}
