#pragma once

#include "esp_zigbee_core.h"
#include "esp_zigbee_zcl.h"

#define ANALOG_ENDPOINT_NUMBER 6

class ZigbeeAnalogServer {
public:
    ZigbeeAnalogServer(uint8_t endpoint_id = ANALOG_ENDPOINT_NUMBER);
    void bind(uint64_t device_addr);
    void setAnalogInputOnDevice(uint64_t device_addr, float value);
    float getDeviceAnalogOutput(uint64_t device_addr) const;
    void onDeviceAnalogOutputChange(void (*callback)(uint64_t, float));
    void handleZclReport(uint64_t device_addr, const esp_zb_zcl_report_attr_message_t *message);
private:
    uint8_t _endpoint_id;
    void (*_onDeviceAnalogOutputChange)(uint64_t, float);
    float _last_device_output;
};