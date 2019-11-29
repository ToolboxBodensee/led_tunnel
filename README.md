# LED tunnel

**Entwicklung nur auf [GitLab](https://gitlab.com/ToolboxBodensee/die-erleuchteten/led_tunnel). Auf GitHub befindet sich lediglich ein Mirror**

[![Build Status](https://travis-ci.com/ToolboxBodensee/led_tunnel.svg?branch=master)](https://travis-ci.com/ToolboxBodensee/led_tunnel)
## About
This is for the hexagonal 2 meter tunnel for drones which has a 5 meter neopixel led stripe in a spiral attached to the inner surface. The code is running on a ESP8266 based board.
This project uses platform io ![Logo](https://platformio.org/images/platformio-logo-xs.fd6e881d.png) to build it which makes it super simple to use.

## Installation & Compilation
### Install platform io
You can use and install platform io in a lot of different ways.
There is a [Atom](https://atom.io/) :atom: plugin if you want to install and use it with atom.
You could install the platform io IDE if you want or just install the core tool with the line below.
```bash
pip install -U platformio
```
If you just install the platform io core you may want to add some links for easy access like this:
```bash
#Note: your path may be different
sudo ln -s ~/.platformio/penv/bin/platformio /usr/local/bin/platformio
sudo ln -s ~/.platformio/penv/bin/pio /usr/local/bin/pio
sudo ln -s ~/.platformio/penv/bin/piodebuggdb /usr/local/bin/piodebuggdb
```
### Compile
Compilation is easy:
```bash
pio run
```
## Get the code onto the ESP
### Automated OTA update of the ESP8266
At the moment you copy the build bin file from `.pioenvs/*/*.bin` to `bin/httpUpdate.bin` and commit it.
You also need to increment the version number manually at the moment _(in the code and the `bin/httpUpdate.txt` file in `bin`)_.
The ESP will flash itself on startup automatically if something newer is found.
In the future this process should get even more simplified with travis.
All the settings like SSID and password are stored in EEPROM and will survive a flash.
### Manual flashing
_This method is should only be needed for the first upload._
In case you use a frash ESP you need to flash it manually with the following command _(you may need to configure a device)_
```bash
pio run -t upload
```
You also need to write some usefull defaults to the EEPROM.
This is done by changeing the code at the moment and will get improved in the future with a configuration page hosted by the ESP when no config is set.
