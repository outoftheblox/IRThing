# IRThing
A wemos D1 mini IR receiver and sender that works with mqtt.

# Hardware
 - Wemos Lolin D1 mini
 - Wemos IR Controller Shield V1.0.0 voor LOLIN D1 mini


# MQTT
Listen to an IRThing
```
mosquitto_sub -t 'esp/{client_id}/ir/+' -m 
```

Publish to an IRThing
```
mosquitto_pub -t 'esp/{client_id}/ir/send' -m "4 5ea1b24c 32 2"
```