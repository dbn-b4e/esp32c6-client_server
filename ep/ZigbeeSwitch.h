/* Class of Zigbee On/Off Switch endpoint inherited from common EP class */

#pragma once

#include "soc/soc_caps.h"
#include "sdkconfig.h"
#if CONFIG_ZB_ENABLED

#include "ZigbeeEP.h"
#include "ha/esp_zigbee_ha_standard.h"

class ZigbeeSwitch : public ZigbeeEP {
public:
  ZigbeeSwitch(uint8_t endpoint);
  ~ZigbeeSwitch() {}

  // methods to control the on/off light
  void lightToggle();
  void lightToggle(uint16_t group_addr);
  void lightToggle(uint8_t endpoint, uint16_t short_addr);
  void lightToggle(uint8_t endpoint, esp_zb_ieee_addr_t ieee_addr);

  void lightOn();
  void lightOn(uint16_t group_addr);
  void lightOn(uint8_t endpoint, uint16_t short_addr);
  void lightOn(uint8_t endpoint, esp_zb_ieee_addr_t ieee_addr);

  void lightOff();
  void lightOff(uint16_t group_addr);
  void lightOff(uint8_t endpoint, uint16_t short_addr);
  void lightOff(uint8_t endpoint, esp_zb_ieee_addr_t ieee_addr);

  void lightOffWithEffect(uint8_t effect_id, uint8_t effect_variant);
  void lightOnWithTimedOff(uint8_t on_off_control, uint16_t time_on, uint16_t time_off);
  void lightOnWithSceneRecall();

  // Use to set a cb function to be called on light change
  void onLightChange(void (*callback)(bool)) {
    _on_light_change = callback;
  }
  // Use to set a cb function to be called on light change with source
  void onLightChangeWithSource(void (*callback)(bool, uint8_t, esp_zb_zcl_addr_t)) {
    _on_light_change_with_source = callback;
  }

private:
  void (*_on_light_change)(bool);
  void (*_on_light_change_with_source)(bool, uint8_t, esp_zb_zcl_addr_t);

  // save instance of the class in order to use it in static functions
  static ZigbeeSwitch *_instance;
  zb_device_params_t *_device;
  void findEndpoint(esp_zb_zdo_match_desc_req_param_t *cmd_req);
  void bindCb(esp_zb_zdp_status_t zdo_status, void *user_ctx);
  void findCb(esp_zb_zdp_status_t zdo_status, uint16_t addr, uint8_t endpoint, void *user_ctx);
  static void findCbWrapper(esp_zb_zdp_status_t zdo_status, uint16_t addr, uint8_t endpoint, void *user_ctx);
  static void bindCbWrapper(esp_zb_zdp_status_t zdo_status, void *user_ctx);

  void zbAttributeRead(uint16_t cluster_id, const esp_zb_zcl_attribute_t *attribute, uint8_t src_endpoint, esp_zb_zcl_addr_t src_address) override;
};

#endif  // CONFIG_ZB_ENABLED
