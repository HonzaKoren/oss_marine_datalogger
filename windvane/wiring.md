# Arduino Nano module wiring

## Components

- Controller: Arduino Nano https://www.laskakit.cz/arduino-nano-r3--atmega328p-klon--nepripajene-piny/
- Local storage **SDC**: SD Card reader (SPI, 5V) https://dratek.cz/arduino/993-ctecka-microsd-karet.html
- Location **LOC**: Ublox Neo-7M (SoftwareSerial) https://dratek.cz/arduino/1733-gps-satelitni-urceni-polohy-neo-7m-modul.html
- Temperature&Humidity **DHT**: DHT22 (DigitalRead) https://dratek.cz/arduino/1188-dht22-teplomer-a-vlhkomer-digitalni.html
- Anemometer **ANE**: Hall sensor KY-003 (DigitalRead) https://dratek.cz/arduino/7717-halluv-senzor-modul-ky-003.html
- Wireless Serial **WSER**: HC-12 (SoftwareSerial + DigitalWrite) https://dratek.cz/arduino/1394-433mhz-hc-12-si4463-wireless-serial-port-modul-1000m.html + https://dratek.cz/arduino/2119-nicerf-433mhz-antena-spiralova-sw433-th32dn-poniklovana.html
- Movement detection **ACC**: MPU6050 Gyroscope (I2C) https://www.laskakit.cz/arduino-gyroskop-a-akcelerometr-gy-521--mpu6050/
- Barometric pressure **BAR**: BMP280 (I2C) https://www.laskakit.cz/arduino-senzor-barometrickeho-tlaku-a-teploty-bmp280/
- Compass **CMP**: HMC5883L (I2C) https://www.laskakit.cz/3-osy-magnetometr-a-kompas-hmc5883l/
- Wind direction **DIR**: AS5600 (I2C) https://www.laskakit.cz/magneticky-rotacni-enkoder-as5600--i2c--pwm/

## Nano pins

- A0 - Battery voltage
- D2 - D12 - Digital pins
  - D2: Ublox Neo RX SoftwareSerial
  - D3: Ublox Neo TX SoftwareSerial
  - D4: AS5600 DigitalRead
  - D5: 
  - D6: Hall sensor DigitialRead
  - D7
  - D8
  - D9
  - D10
  - D11
  - D12

| Use | PIN | ::: | PIN  | Use |
|:---:|:---:|:---:|:----:|:---:|
| ___ | TX  |     | VIN  | ___ |
| ___ | RX  |     | GND  | ___ |
| ___ | RST |     | RST  | ___ |
| ___ | GND |     | +5V  | ___ |
| ___ | D2  |     | A7   | ___ |
| ___ | D3  |     | A6   | ___ |
| ___ | D4  |     | A5   | SCL |
| ___ | D5  |     | A4   | SDA |
| ___ | D6  |     | A3   | ___ |
| ___ | D7  |     | A2   | ___ |
| ___ | D8  |     | A1   | ___ |
| ___ | D9  |     | A0   | ___ |
| ___ | D10 |     | AREF | ___ |
| ___ | D11 |     | 3V3  | ___ |
| ___ | D12 |     | D13  | ___ |
|     |     | USB |      |     |
