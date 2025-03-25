# Wake-On-Wemos #
Wemos D1 mini v4 software for controlling PC power status

# Main functionalities #
- Turning PC ON with power button press simulation
- Turning PC OFF with power button hold simulation
- Restarting PC with reset button press simulation
- Checking wheter PC is ON or OFF, by measuring power LED voltage

## Needed tools ##
- ### [Arduino IDE](https://support.arduino.cc/hc/en-us/articles/360019833020-Download-and-install-Arduino-IDE) ###
For wemos D1 mini v4 it's also required to add **Additional Boards Manager** and install **esp8266 platform** as explained [here](https://github.com/esp8266/Arduino)
   - **Additional Boards Manager**
     >Enter https://arduino.esp8266.com/stable/package_esp8266com_index.json into the File>Preferences>Additional Boards Manager URLs field of the Arduino IDE. You can add multiple URLs, separating them with commas.
   - **esp8266 platform**
     >Open Boards Manager from Tools > Board menu and install esp8266 platform (and don't forget to select your ESP8266 board from Tools > Board menu after installation).

## Needed hardware ##
- [Wemos D1 mini v4](https://www.wemos.cc/en/latest/d1/d1_mini.html)
```cpp
static const uint8_t D0   = 16;
static const uint8_t D1   = 5;
static const uint8_t D2   = 4;
static const uint8_t D3   = 0;
static const uint8_t D4   = 2;
static const uint8_t D5   = 14;
static const uint8_t D6   = 12;
static const uint8_t D7   = 13;
static const uint8_t D8   = 15;
static const uint8_t D9   = 3;
static const uint8_t D10  = 1;
```
