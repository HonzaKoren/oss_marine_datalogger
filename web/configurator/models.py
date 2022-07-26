from django.db import models

# Create your models here.

class Configuration(models.Model):
    """
    Configuration items

    Items name schema of dotted notation:
    type.bus.device.[optional parameter].[optional parameter].<source|active|value>
    Types:
    - sensor - source of data
    - output - digital data target
    - pwm - pwm driven data target

    Buses:
    - analog - GrovePi analog ports
    - digital - GrovePi digital ports
    - serial - GrovePi/RPi serial ports
    - i2c - I2C bus
    - network - network device
    - usb - USB device
    
    Devices:
    - button - digital or analog input
    - display - any visual output device
    - buzzer - digital acoustic signaller
    - led - LED for visual status
    - accelerometer - position sensor
    - barometer - environmental conditions sensor
    - dht11 - environmental conditions sensor
    - nmea_location - Air530 or other NMEA comaptible location sensor
    """
    item = models.CharField(max_length=100, primary_key=True)
    value = models.CharField(max_length=100)
    comment = models.TextField(null=True, blank=True)

    def __str__(self):
        """
        Human readable representation
        """
        return f"{self.item}"
    
    class Meta:
        """
        Model customization
        """
        ordering = ('item', )