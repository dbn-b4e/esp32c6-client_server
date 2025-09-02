#include "ZigbeeAnalogServer.h"

void onRemoteAnalogOutputChanged(uint64_t device_addr, float value) {
    Serial.print("Remote Analog Output changed for device: ");
    Serial.print(device_addr, HEX);
    Serial.print(" Value: ");
    Serial.println(value);
}

ZigbeeAnalogServer analogServer(ANALOG_ENDPOINT_NUMBER);

void setup() {
    Serial.begin(115200);
    analogServer.onDeviceAnalogOutputChange(onRemoteAnalogOutputChanged);
    Serial.println("ZigbeeAnalogServer coordinator ready.");
}

void loop() {
    delay(5000);
}