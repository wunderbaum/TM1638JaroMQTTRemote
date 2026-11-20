# TM1638JaroMQTTRemote

Remote control for [Jarolift_MQTT](https://github.com/madmartin/Jarolift_MQTT) using a TM1638 module.

## Supported boards

- **ESP8266:** Wemos D1 mini (existing implementation)
- **ESP32-C3:** ESP32-C3 SuperMini (new implementation)

## ESP32-C3 SuperMini wiring

| TM1638 | ESP32-C3 SuperMini |
|---|---|
| GND | GND |
| VCC | 3.3V |
| STB | GPIO4 |
| CLK | GPIO5 |
| DIO | GPIO6 |

Use 3.3 V for the TM1638. The ESP32-C3 GPIO pins are 3.3 V only. If your module only operates from 5 V or exposes 5 V DIO levels, use an appropriate logic-level interface.

## Configuration

Copy `src/wifidata.h.example` to `src/wifidata.h` and enter your Wi-Fi and MQTT settings:

```cpp
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
const char* mqtt_server = "YOUR_MQTT_BROKER";
```

`src/wifidata.h` is ignored by Git.

## PlatformIO builds

```bash
pio run -e d1_mini
pio run -e esp32-c3-supermini
```

To upload:

```bash
pio run -e esp32-c3-supermini -t upload
pio device monitor -e esp32-c3-supermini
```

## MQTT

- Button presses: `TM1638/button`
- Display text: `TM1638/message`
- Brightness: `TM1638/brightness` (0–7)
- Status: `TM1638/pub`
- Shutter commands: `cmd/jarolift/shutter/<0-7>` with `UP`, `DOWN`, or `STOP`

Example:

```bash
mosquitto_pub -t TM1638/message -m "HELLO"
mosquitto_pub -t TM1638/brightness -m 3
```
