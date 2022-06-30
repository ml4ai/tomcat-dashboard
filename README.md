# tomcat-dashboard
Dashboard for ToMCAT and Closed Loop Communication experiments

## Installing Dependencies
cmake: Build software used for generating cross-platform Makefiles
Mosquitto: MQTT message bus broker
paho.mqtt.cpp: Client library for MQTT message bus integration
nlohmann-json:  Library for creating and parsing JSON messages
wxWidgets: Cross-platform application and GUI development library
Boost: Multipurpose library

### macOS
On macOS, all dependencies can be installed using the MacPorts package manager.
`port install cmake mosquitto paho.mqtt.cpp nlohmann-json wxWidgets-3.2 boost`

### Ubuntu
Similarly, from Ubuntu 21.10 onwards, all dependencies can be installed through the apt-get package manager.
```console
apt-get install cmake mosquitto 
apt-get install libpaho-mqtt-dev libpaho-mqttpp-dev libssl-dev 
apt-get install nlohmann-json3-dev libboost-all-dev
apt-get install libwxgtk3.0-gtk3-dev
```
Note: The paho.mqtt.cpp library  is not available through  apt-get before Ubuntu 21.10. If using a prior version of Ubuntu, the library will need to be built from source.

## Building
```console
mkdir build && cd build
cmake ..
make 
```
## Running
`./gui`
