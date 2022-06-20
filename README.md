# OSS Marine Datalogger
OSS datalogger for any sailing fan including support of set of gadgets to get real data to your sailing history.

# Aim
Let's create a set of hardware and software, so you can log your sailing trips under the open data and with as much detail as suits you.
For look under the hood look at [architecture](architecture.md).

# Kick it up&running
A thorough [guide to make your Rasberry your personal tracker](qsg.md)

# External guides and inspirations
[See more](external_links.md)

# Expected hardware
- [Wind speed and direction sensor](wind_measurement.md)
- [Microcontroller Zero W](microcontroller.md)
- Any power bank with USB port to power up whole solution
- GPS/Galileo/GLONASS receiver
- Accelerometer (for anchor alarm and yacht waving compensations)
- Barometer (for weather alarms)

[Bill of material](bom.md)

# Standard wiring
[For no configuration customization use standard wiring schema.](wiring.md)

# Features
- Web interface to be used for visualisations
- Possibility to use WiFi internet connection to share data to any configured server supporting documented REST API
  - Select the data sets to be shared
  - Select the sharing frequency per dataset
  - Global share on/off
- Possibility to set asynchronous commands
  - Managed "cron" jobs
  - Alarms
  - Timed events to start/stop sharing
  - Any command possible to run at given time and frequency in general
- Possibility to set up a reaction to event (now primary intended for anchor alarm)
  - Event sources:
    - Accelerometer (broach logging, dangerous tilt)
    - GPS
      - position offset (anchor alarm)
      - point on map (turning or target point)
      - point connection (i.e. for start or finish during races)
    - Barometer (air pressure drop)
    - Wind speed (reefing alarm)
- Possibility to store Windy data for current location along with tracking data

Want more? Look, if we [plan it](backlog.md).

# Open Points
- Check out the possibility to broadcast data via BTLE to mobile apps
- Source of navy maps
