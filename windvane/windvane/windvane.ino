//
//    FILE: windvane.ino
//  AUTHOR: Jan Kořínek
// PURPOSE: Run windvane/anemometer/DHT22/Adafruit sensors
//    DATE: 2023-04-19
// Sensors: AS5600 - Magnetic rotary encoder
//          DHT22 - Temperature/humidity sensor
//          HMC5883L - magnetic compas
//          

#include <Adafruit_HMC5883_U.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_Sensor.h>
#include <AS5600.h>
#include <DHT.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>   // Include the WebServer library
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h> 
#include <FS.h>
#include <MPU6050_light.h>
#include <SPI.h>
#include <WiFiClient.h>
#include <Wire.h>

// Asynchronous ISR and variables declaration
ICACHE_RAM_ATTR void isrPulseHandler();
volatile bool measure_window_start = false; // Whether first measurement in a time window starts
volatile unsigned long curr_time = 0;       // Current time stamp
volatile unsigned long start_time = 0;      // Timestamp of measurement window start
volatile bool concurrency_lock = false;     // Prevent two semi parallel runs on bounce
volatile float wind = 0;                    // Wind speed in m/s
volatile unsigned long rpm = 0;             // Counted pulses
int wind_probe = D6;                        // Anemometer pin

// Set static IP identity
IPAddress local_IP(192, 168, 192, 102);
// Set your Gateway IP address
IPAddress gateway(192, 168, 192, 254);

IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(192, 168, 192, 254);   //optional


// MultiWiFi setup
ESP8266WiFiMulti wifiMulti;     // Create an instance of the ESP8266WiFiMulti class, called 'wifiMulti'

// Start webserver
ESP8266WebServer server(80);    // Create a webserver object that listens for HTTP request on port 80

// Set up DHT digital pin
#define DHTPIN D4     // Digital pin connected to the DHT sensor
#define DHTTYPE    DHT22     // DHT 22 (AM2302)
DHT dht(DHTPIN, DHTTYPE);

// Barometric pressure
Adafruit_BMP280 bmp; // use I2C interface

// Set up windvane sensor
AS5600 as5600;   //  use default Wire

// Set up compass
Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);

// Set up accelerometer
MPU6050 mpu(Wire);

void handleRoot();              // function prototypes for HTTP handlers
void handleNotFound();

struct DhtData;   // DHT22 data prototype

struct BmpData;   // BMP280 data prototype

struct MpuData;   // MPU6050 data prototype

void setup(void){
  Serial.begin(115200);         // Start the Serial communication to send messages to the computer
  delay(10);
  Serial.println('\n');

  // Set up anemometer pin
  pinMode(wind_probe,INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(wind_probe), isrPulseHandler, FALLING);  //  function for creating external interrupts at wind_probe pin on falling (HIGH to LOW)
  start_time = micros();
  
  // Set up FS
  bool res = SPIFFS.begin();

  // Start DHT reader
  dht.begin();

  // Start compass reader
  mag.begin();

  // Start magnetic encoder
  as5600.begin(4);  //  set direction pin.
  as5600.setDirection(AS5600_CLOCK_WISE);  // default, just be explicit.

  // Set up default APs to try to connect to
  wifiMulti.addAP("koreni", "heslicko");   // add Wi-Fi networks you want to connect to
  wifiMulti.addAP("ossml", "MarineLogger");
  // wifiMulti.addAP("ssid_from_AP_3", "your_password_for_AP_3");

  Serial.println("Connecting ...");
  int i = 0;
  while (wifiMulti.run() != WL_CONNECTED) { // Wait for the Wi-Fi to connect: scan for Wi-Fi networks, and connect to the strongest of the networks above
    delay(250);
    Serial.print('.');
  }
  if (WiFi.SSID() == "ossml") {
    WiFi.config(local_IP, gateway, subnet, primaryDNS);
    Serial.println("Connected to an implicit SSID, setting static IP.");
  }
  Serial.println('\n');
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());              // Tell us what network we're connected to
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());           // Send the IP address of the ESP8266 to the computer

  if (MDNS.begin("meteo")) {              // Start the mDNS responder for esp8266.local
    Serial.println("mDNS responder started");
    MDNS.addService("http", "tcp", 80);
  } else {
    Serial.println("Error setting up MDNS responder!");
  }

  // Start barometer
  unsigned status;
  status = bmp.begin(0x76);
  if (!status) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring or "
                      "try a different address!"));
    Serial.print("SensorID was: 0x"); Serial.println(bmp.sensorID(),16);
    Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
    Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
    Serial.print("        ID of 0x60 represents a BME 280.\n");
    Serial.print("        ID of 0x61 represents a BME 680.\n");
    while (1) delay(10);
  } else {
    Serial.print("BMP280 init status: ");
    Serial.println(status);
  }
  bmp.setSampling(Adafruit_BMP280::MODE_FORCED,     /* Operating Mode. */
                Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */

  // Start MPU6050
  byte mpu_status = mpu.begin();
  Serial.print(F("MPU6050 status: "));
  Serial.println(mpu_status);
  // Calibrate
  mpu.calcOffsets(true,true);
  
  server.enableCORS(true);
  server.on("/data.json", handleJSON);      // Serve dynamic data as JSON
  server.on("/data.txt", handleTXT);        // Serve dynamic data as Prometheus plain text
  server.serveStatic("/", SPIFFS, "/index.html");
  server.serveStatic("/", SPIFFS, "/");
  server.onNotFound(handleNotFound);        // When a client requests an unknown URI (i.e. something other than "/"), call function "handleNotFound"

  server.begin();                           // Actually start the server
  Serial.println("HTTP server started");
}

void loop(void){
  server.handleClient();                    // Listen for HTTP requests from clients
}

struct DhtData
{
  String temperature;
  String humidity;
};

struct BmpData
{
  String temperature;
  String pressure;
  String altitude;
};

struct MpuData
{
  String temperature;
  String accX;
  String accY;
  String accZ;
  String accAngleX;
  String accAngleY;
  String angleX;
  String angleY;
  String angleZ;
  String gyroX;
  String gyroY;
  String gyroZ;
};

DhtData getDHT() {
  // DHT22 
  DhtData _dht;
  if(isnan(dht.readTemperature())) {
    _dht.temperature = String(-65535);
  } else {
    _dht.temperature = String(dht.readTemperature());
  }
  if(isnan(dht.readHumidity())) {
    _dht.humidity = String(-65535);
  }
    _dht.humidity = String(dht.readHumidity());
  return _dht;
}

BmpData getBMP() {
  // BMP init
  BmpData _bmp;
  if (bmp.takeForcedMeasurement()) {
    _bmp.temperature = String(bmp.readTemperature());
    _bmp.pressure = String(bmp.readPressure());
    _bmp.altitude = String(bmp.readAltitude());
  } else {
    _bmp.temperature = String("-0.0");
    _bmp.pressure = String("-0.0");
    _bmp.altitude = String("-0.0");
  }
  return _bmp;
}

MpuData getMPU() {
  // MPU init
  MpuData _mpu;
  mpu.update();
  _mpu.temperature = String(mpu.getTemp());
  _mpu.accX = String(mpu.getAccX());
  _mpu.accY = String(mpu.getAccY());
  _mpu.accZ = String(mpu.getAccZ());
  _mpu.accAngleX = String(mpu.getAccAngleX());
  _mpu.accAngleY = String(mpu.getAccAngleY());
  _mpu.angleX = String(mpu.getAngleX());
  _mpu.angleY = String(mpu.getAngleY());
  _mpu.angleZ = String(mpu.getAngleZ());
  _mpu.gyroX = String(mpu.getGyroX());
  _mpu.gyroY = String(mpu.getGyroY());
  _mpu.gyroZ = String(mpu.getGyroZ());
  return _mpu;
}

String getVoltage() {
  float analog = analogRead(A0);
  float voltage = 4.2 * (analog / 1024);  // Max ESP input voltage * Digital sensor load
  return String(voltage);
};

String getDirection() {
  // Compass data
  sensors_event_t magData; 
  mag.getEvent(&magData);
  float azimuth = atan2(magData.magnetic.y, magData.magnetic.x);
  if(azimuth < 0)
    azimuth += 2*PI;
  if(azimuth > 2*PI)
    azimuth -= 2*PI;  
  float azimuthDg = azimuth * 180/M_PI;
  return String(azimuthDg);  
}

void handleJSON() {
  // AS5600 wind direction
  float apparentWind = as5600.rawAngle() * AS5600_RAW_TO_DEGREES;
  // Acceleration
  // Wind speed
  // Timestamp
  // Calibrations
  // Battery voltage and percentage
  // HTTP server request count
  DhtData myDht = getDHT();
  BmpData myBmp = getBMP();
  MpuData myMpu = getMPU();
  
  String response = "{\"humidity_dht\": ";
  response += myDht.humidity;
  response += ", \"temperature_dht\": ";
  response += myDht.temperature;
  response += ", \"inner_temperature\": ";
  response += myBmp.temperature;
  response += ", \"air_pressure\": ";
  response += myBmp.pressure;
  response += ", \"altitude\": ";
  response += myBmp.altitude;
  response += ", \"direction\": ";
  response += getDirection();
  response += ", \"apparent_wind\": ";
  response += String(apparentWind);
  response += ", \"temperature_mpu\": ";
  response += myMpu.temperature;
  response += ", \"accX\": ";
  response += myMpu.accX;
  response += ", \"accY\": ";
  response += myMpu.accY;
  response += ", \"accZ\": ";
  response += myMpu.accZ;
  response += ", \"accAngleX\": ";
  response += myMpu.accAngleX;
  response += ", \"accAngleY\": ";
  response += myMpu.accAngleY;
  response += ", \"angleX\": ";
  response += myMpu.angleX;
  response += ", \"angleY\": ";
  response += myMpu.angleY;
  response += ", \"angleZ\": ";
  response += myMpu.angleZ;
  response += ", \"gyroX\": ";
  response += myMpu.gyroX;
  response += ", \"gyroY\": ";
  response += myMpu.gyroY;
  response += ", \"gyroZ\": ";
  response += myMpu.gyroZ;
  response += ", \"wind_speed\": ";
  response += String(wind);
  response += ", \"wind_pulses\": ";
  response += String(rpm);
  response += ", \"a0_data\": ";
  response += String(analogRead(A0));
  response += ", \"battery_voltage\": ";
  response += getVoltage();
  response += ", \"runtime\": ";
  response += String(millis());
  response += "}";
  server.send(200, "application/json", response);
}

void handleTXT() {
  DhtData myDht = getDHT();  // Read DHT
  BmpData myBmp = getBMP();
  MpuData myMpu = getMPU();
  // AS5600 wind direction
  float apparentWind = as5600.rawAngle() * AS5600_RAW_TO_DEGREES;
  // Acceleration
  // Wind speed
  // Timestamp
  // Calibrations 
  // Battery voltage and percentage
  // HTTP server request count
  String response = "humidity_dht " + myDht.humidity + "\n";
  response += "temperature_dht " + myDht.temperature + "\n";
  response += "inner_temperature " + myBmp.temperature + "\n";
  response += "air_pressure " + myBmp.pressure+ "\n";
  response += "altitude " + myBmp.altitude + "\n";
  response += "direction " + getDirection() + "\n";
  response += "apparent_wind " + String(apparentWind) + "\n";
  response += "temperature_mpu " + myMpu.temperature + "\n";
  response += "accX " + myMpu.accX + "\n";
  response += "accY " + myMpu.accY + "\n";
  response += "accZ " + myMpu.accZ + "\n";
  response += "accAngleX " + myMpu.accAngleX + "\n";
  response += "accAngleY " + myMpu.accAngleY + "\n";
  response += "angleX " + myMpu.angleX + "\n";
  response += "angleY " + myMpu.angleY + "\n";
  response += "angleZ " + myMpu.angleZ + "\n";
  response += "gyroX " + myMpu.gyroX + "\n";
  response += "gyroY " + myMpu.gyroY + "\n";
  response += "gyroZ " + myMpu.gyroZ + "\n";
  response += "wind_speed " + String(wind) + "\n";
  response += "wind_pulses " + String(rpm) + "\n";
  response += "a0_data " + String(analogRead(A0)) + "\n";
  response += "battery_voltage " + getVoltage() + "\n";
  response += "runtime " + String(millis()) + "\n";
  server.send(200, "text/plain", response);
}

void handleNotFound(){
  server.send(404, "text/plain", "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}

//ISR function catching anemometer pulse
ICACHE_RAM_ATTR void isrPulseHandler()
{
  if(!concurrency_lock) {
    concurrency_lock = true;
    // Parallel run debounce
    // Get now to use it for pulse length and debounce
    curr_time = micros();
    // Debounce measurement sooner, than 20 ms
    unsigned long pulse_duration = curr_time - start_time;
    if(pulse_duration > 750) {
      if(!measure_window_start) {
        float pulse_length = (float)(pulse_duration / 1000);  // Pulse length in seconds
        wind = (float)(2400000.0 / (pulse_duration * 3.6));    // Device constant 1 pulse per second = 2,4 km/h, calc in m/s
        rpm++;
      }
      // Measurement was considered valid, shift start time
      start_time = curr_time;
      // Next measurement will not be taken as initial
      measure_window_start = false;
    }
    concurrency_lock = false;
  }
}
