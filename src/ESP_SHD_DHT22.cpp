#include "ESP_SHD_DHT22.h"

#define DEBUG 0

ShdDht22Sensor::ShdDht22Sensor(uint8_t _pin, uint32_t _intervalS, uint8_t _dhtType) : counter5ms(0), pin(_pin), interval5msCount(_intervalS * 200) {
  dht = new DHT(_pin, _dhtType);

  snprintf(pubTopicHumidity, TOPIC_BUFFER_LENGTH, "%s/Humidity", name);
  snprintf(pubTopicTemperature, TOPIC_BUFFER_LENGTH, "%s/Temperature", name);

  dht->begin();

  Serial.print("DHT: New DHT sensor. It publishes \"");
  Serial.print(pubTopicHumidity);
  Serial.print("\" and \"");
  Serial.print(pubTopicHumidity);
  Serial.println("\"");
}

void ShdDht22Sensor::timer5msHandler() {
  counter5ms++;

  if (counter5ms >= interval5msCount) {
     counter5ms = 0;
     republish();
  }

}

void ShdDht22Sensor::republish() {
  clearMessage();
  #if DEBUG >= 1
  Serial.print("DHT: Humidity = ");
  Serial.println(dht->readHumidity());
  #endif
  snprintf(message, MESSAGE_BUFFER_LENGTH, "%.1f", dht->readHumidity());
  mqttPublish(pubTopicHumidity, message);

  clearMessage();
  #if DEBUG >= 1
  Serial.print("DHT: Temperature = ");
  Serial.println(dht->readTemperature());
  #endif
  snprintf(message, MESSAGE_BUFFER_LENGTH, "%.1f", dht->readTemperature());
  mqttPublish(pubTopicTemperature, message);
}

void ShdDht22Sensor::clearMessage() {
  for (uint8_t i = 0; i < MESSAGE_BUFFER_LENGTH; i++) {
    message[i] = 0;
  }
}

bool ShdDht22Sensor::handleMqttRequest(char* _topic, unsigned char* _payload, uint16_t _length) {
  return false;
}
