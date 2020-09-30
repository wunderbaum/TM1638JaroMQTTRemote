# TM1638JaroMQTTRemote

Remotecontrol via MQTT for the Jarolift_MQTT-project: https://github.com/madmartin/Jarolift_MQTT
You need a WEMOS D1 R2 (or other ESP8266based board) and a 16-button QYF-TM1638-board and a few cables:

Module: https://www.aliexpress.com/item/4001104267700.html

Based on the famous TM1638plus-library by Gavin Lyons https://github.com/gavinlyonsrepo/TM1638plus

For each shutter, two buttons are used. Up and down. If the shutter is in motion, it stops when you press the button again.

Connect:
<pre>
GND <-> GND 
D1  <-> STB
D2  <-> CLK
D3  <-> DIO
3.3 <-> VCC
</pre>
This is a project in development, so feel free to participate!

TODO:
Housing, powersource etc etc.
