# Arduino Nano module wiring

## Components

- Controller: Arduino Nano
  - https://www.laskakit.cz/arduino-nano-r3--atmega328p-klon--nepripajene-piny/
- Local storage **SDC**: SD Card reader (SPI, **3V3**/+5V)
  - https://dratek.cz/arduino/993-ctecka-microsd-karet.html
  - Connected to Nano SPI top header
- Location **LOC**: Ublox Neo-7M (SoftwareSerial, **3V3**/+5V)
  - https://dratek.cz/arduino/1733-gps-satelitni-urceni-polohy-neo-7m-modul.html
  - https://dratek.cz/docs/produkty/0/474/1502177743.pdf
- Temperature&Humidity **DHT**: DHT22 (DigitalRead, **3V3**/+5V)
  - https://dratek.cz/arduino/1188-dht22-teplomer-a-vlhkomer-digitalni.html
  - https://dratek.cz/docs/produkty/0/56/1455993178.pdf
- Anemometer **ANE**: Hall sensor KY-003 (DigitalRead, **3V3**/+5V)
  - https://dratek.cz/arduino/7717-halluv-senzor-modul-ky-003.html
  - S pin for digital input
  - - for GND
  - Middle for VIN
- Wireless Serial **WSER**: HC-12 (SoftwareSerial + DigitalWrite, **3V3**/+5V)
  - https://dratek.cz/arduino/1394-433mhz-hc-12-si4463-wireless-serial-port-modul-1000m.html
  - https://dratek.cz/docs/produkty/1/1301/1473675078.pdf
- Movement detection **ACC**: MPU6050 Gyroscope (I2C, 3V3)
  - https://www.laskakit.cz/arduino-gyroskop-a-akcelerometr-gy-521--mpu6050/
  - https://www.laskakit.cz/user/related_files/mpu-6050_datasheet_v3_4.pdf
- Barometric pressure **BAR**: BMP280 (I2C, 3V3)
  - https://www.laskakit.cz/arduino-senzor-barometrickeho-tlaku-a-teploty-bmp280/
- Compass **CMP**: HMC5883L (I2C, 3V3)
  - https://www.laskakit.cz/3-osy-magnetometr-a-kompas-hmc5883l/
  - https://github.com/adafruit/Adafruit_HMC5883_Unified
  - https://www.laskakit.cz/user/related_files/hmc5883l.pdf
- Wind direction **DIR**: AS5600 (I2C, 3V3)
  - https://www.laskakit.cz/magneticky-rotacni-enkoder-as5600--i2c--pwm/
  - https://www.laskakit.cz/user/related_files/bst-bmp280-ds001-11.pdf

## Nano pins

- A0 - Battery voltage
- D2 - D12 - Digital pins
  - D2: Ublox Neo TX SoftwareSerial
  - D3: Ublox Neo RX SoftwareSerial
  - D4: HC-12 TX SoftwareSerial
  - D5: HC-12 RX SoftwareSerial
  - D6: DHT22 DigitalRead
  - D7: Hall sensor DigitialRead
  - D8
  - D9
  - D10
  - D11
  - D12

| Use     | PIN | :::<br>SD Card SPI | PIN  | Use          |
|:---:    |:---:|:---:               |:----:|:---:         |
| ___     | TX  |                    | VIN  | ___          |
| ___     | RX  |                    | GND  | All chip GND |
| ___     | RST |                    | RST  | ___          |
| ___     | GND |                    | +5V  | ___          |
| LOC TX  | D2  |                    | A7   | ___          |
| LOC RX  | D3  |                    | A6   | ___          |
| WSER TX | D4  |                    | A5   | All I2C SCL  |
| WSER RX | D5  |                    | A4   | All I2C SDA  |
| DHT     | D6  |                    | A3   | ___          |
| ANE     | D7  |                    | A2   | ___          |
| ___     | D8  |                    | A1   | ___          |
| ___     | D9  |                    | A0   | ___          |
| ___     | D10 |                    | AREF | ___          |
| ___     | D11 |                    | 3V3  | All chip VIN |
| ___     | D12 |                    | D13  | ___          |
|         |     | USB                |      |              |
