#include "ZigbeeAnalog.h"

void onAnalogInputChange(float value) {
    Serial.print("Analog input changed: ");
    Serial.println(value);
}

void onAnalogOutputChange(float value) {
    Serial.print("Analog output changed: ");
    Serial.println(value);
}

void simulateHardwareChange(ZigbeeAnalog &analogDev) {
    static float testInput = 0.0f;
    testInput += 1.0f;
    analogDev.setAnalogInput(testInput);

    static float testOutput = 100.0f;
    testOutput -= 1.0f;
    analogDev.setAnalogOutput(testOutput);
}

ZigbeeAnalog analogEndpoint(ANALOG_ENDPOINT_NUMBER);

void setup() {
    Serial.begin(115200);
    analogEndpoint.onAnalogInputChange(onAnalogInputChange);
    analogEndpoint.onAnalogOutputChange(onAnalogOutputChange);
    Serial.println("ZigbeeAnalog endpoint ready.");
}

void loop() {
    simulateHardwareChange(analogEndpoint);
    delay(2000);
}