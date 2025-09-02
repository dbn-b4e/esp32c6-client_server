#include "ZigbeeAnalogServer.h"

ZigbeeAnalogServer::ZigbeeAnalogServer(uint8_t endpoint_id)
    : _endpoint_id(endpoint_id),
      _onDeviceAnalogOutputChange(nullptr),
      _last_device_output(0.0f) {}

void ZigbeeAnalogServer::bind(uint64_t device_addr) {
    // Implement Zigbee binding to the end device analog cluster
}

void ZigbeeAnalogServer::setAnalogInputOnDevice(uint64_t device_addr, float value) {
    // Send a ZCL write attribute to the end device for analog input
}

float ZigbeeAnalogServer::getDeviceAnalogOutput(uint64_t device_addr) const {
    return _last_device_output;
}

void ZigbeeAnalogServer::onDeviceAnalogOutputChange(void (*callback)(uint64_t, float)) {
    _onDeviceAnalogOutputChange = callback;
}

void ZigbeeAnalogServer::handleZclReport(uint64_t device_addr, const esp_zb_zcl_report_attr_message_t *message) {
    if (!message) return;
    if (message->attr_id == ESP_ZB_ZCL_ATTR_ANALOG_OUTPUT_PRESENT_VALUE_ID) {
        float val = *(float*)message->value;
        _last_device_output = val;
        if (_onDeviceAnalogOutputChange) {
            _onDeviceAnalogOutputChange(device_addr, val);
        }
    }
}