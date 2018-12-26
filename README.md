# Callblink

Makes an ESP8266-Output switch when receiving a specific MQTT Message.

## Other Software for this project

- [Project site in vspace.one wiki (German)](https://wiki.vspace.one/doku.php?id=projekte:klingelsignal)
- [MQTT-CallblinkConnector, translates Fritzbox-API (via callmonitor) to MQTT messages]()

## Changes made to dependecies

In [knolleary/pubsubclient](https://github.com/knolleary/pubsubclient)'s `PubSubClient.h`:

```diff
  // MQTT_MAX_PACKET_SIZE : Maximum packet size
  #ifndef MQTT_MAX_PACKET_SIZE
- #define MQTT_MAX_PACKET_SIZE 128
+ #define MQTT_MAX_PACKET_SIZE 512
  #endif
```

This is necessary for being able to receive JSON Payloads sent by MQTT-CallblinkConnector which
bigger than 128 Bytes.