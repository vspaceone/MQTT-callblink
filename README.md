# Callblink

Makes an ESP8266-Output switch outputs when receiving a specific MQTT Message.

## Other Software for this project

- [Project site in vspace.one wiki (German)](https://wiki.vspace.one/doku.php?id=projekte:klingelsignal)
- [Callmonitor-Connector-MQTT, translates Fritzbox-API (via callmonitor) to MQTT messages](https://github.com/vspaceone/Callmonitor-Connector-MQTT)

## Changes made to dependecies

In [knolleary/pubsubclient](https://github.com/knolleary/pubsubclient)'s `PubSubClient.h`:

```diff
  // MQTT_MAX_PACKET_SIZE : Maximum packet size
  #ifndef MQTT_MAX_PACKET_SIZE
- #define MQTT_MAX_PACKET_SIZE 128
+ #define MQTT_MAX_PACKET_SIZE 512
  #endif
```

This is necessary for being able to receive JSON Payloads sent by [Callmonitor-Connector-MQTT](https://github.com/vspaceone/Callmonitor-Connector-MQTT) which
bigger than 128 Bytes.