# What is this?
This is the script for the arduino component of a three-parts project where an [API](https://github.com/sqrrrrl/pc-power-api), an arduino and a [mobile app](https://github.com/sqrrrrl/pc-power-app) communicate together in order to enable users to remotely control the power state of their desktop computer(s).

# Prerequisites
## Important
Make sure the computer's motherboard supports standby USB power so the arduino stay on even when the computer is off.

## Required material
| Item description                        | Amazon link                                                                                                                           |
| --------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------- |
| Arduino board with wifi capabilities    | [Arduino Uno R4 WiFi](https://www.amazon.com/Arduino-UNO-WiFi-ABX00087-Bluetooth/dp/B0C8V88Z9D/)                                      |
| Split female to male jumper wires       | [Single female to dual male jumper wires](https://www.amazon.com/GELRHONR-Motherboard-Light-Emitting-Computer-Buttons/dp/B09P52PLK4/) |
| Male to male jumper wire                | [40 PCS 20 CM jumper wires](https://www.amazon.com/California-JOS-Breadboard-Optional-Multicolored/dp/B0BRTJQZRD/)                    |
| Motherboard USB header to USB A adapter | [Female 9 pin USB 2.0 to female USB A adapter](https://www.amazon.com/SinLoon-Female-Motherboard-Header-Adapter-Dual/dp/B0878S6BD5/)  |
| USB-C to USB-A cable                    | [Amazon Basics 3 feet USB-C to USB-A cable](https://www.amazon.com/Amazon-Basics-Charger-480Mbps-Certified/dp/B01GGKYKQM/)            |

## Installation
1. Download and install the [arduino ide](https://www.arduino.cc/en/software)
2. [Update the arduino firmware](https://support.arduino.cc/hc/en-us/articles/360013896579-Use-the-Firmware-Updater-in-Arduino-IDE)
3. Clone the repository:
```
git clone https://github.com/sqrrrrl/pc-power-arduino.git
```

# Usage
## Environment variables
Open the project in the arduino IDE and create a file named environment.h with the following variables:
```cpp
#define SECRET_SSID "example-wifi" //The wifi network name
#define SECRET_PASS "password" //The wifi network password
#define API_ADDRESS "pcpower.example.com" //The ip address or domain name for the API
#define API_PORT 443 //The port the API is listening on
#define DEVICE_ID "abcdef" //The 6 characters long device id assigned to the device by the API
#define DEVICE_SECRET "abcdefghijqlmnop" //The 16 characters long device secret assigned to the device by the API
```

## Board configuration steps
1. Read the computer's motherboard manual to locate the required pins and use the split jumper wires to connect a motherboard pin to both the arduino and the case switches/LED
2. Connect the positive power switch pin ```POWER SW+``` to the ```digital``` pin ```#3```
3. Connect the positive reset switch pin ```RESET SW+``` to the ```digital``` pin ```#5```
4. Connect the positive power switch LED pin ```POWER+``` to the ```analog``` pin ```#4```
5. Connect the reset pin of the arduino to the ```digital``` pin ```#11```
6. Use one of the USB ports on the computer as the power source for the arduino

## Running the script
The script can be compiled and flashed on the board with the arduino IDE.

# License
Distributed under the [GPL-3.0 license](https://github.com/sqrrrrl/pc-power-arduino#GPL-3.0-1-ov-file)

