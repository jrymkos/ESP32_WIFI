# ESP32_WIFI

Make sure to download the following libraries and add them to arduino IDE

1. https://github.com/mikalhart/TinyGPSPlus/archive/master.zip
2. https://github.com/plerup/espsoftwareserial/

Also make sure to change the SSID and password depending on the network to connect too.
The GPS does not work indoors so there is an example there, make sure to set baud rate of monitor to 9600.
The GPS will need to take time to find a satellite when used outside before giving accurate coordinates.
