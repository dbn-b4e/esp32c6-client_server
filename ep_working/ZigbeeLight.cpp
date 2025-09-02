#include "ZigbeeLight.h"
#if CONFIG_ZB_ENABLED

ZigbeeLight::ZigbeeLight(uint8_t endpoint) : ZigbeeEP(endpoint) {
  _device_id = ESP_ZB_HA_ON_OFF_LIGHT_DEVICE_ID;

  esp_zb_on_off_light_cfg_t light_cfg = ESP_ZB_DEFAULT_ON_OFF_LIGHT_CONFIG();
  _cluster_list = esp_zb_on_off_light_clusters_create(&light_cfg);  // use esp_zb_zcl_cluster_list_create() instead of esp_zb_on_off_light_clusters_create()
  _ep_config = {.endpoint = endpoint, .app_profile_id = ESP_ZB_AF_HA_PROFILE_ID, .app_device_id = ESP_ZB_HA_ON_OFF_LIGHT_DEVICE_ID, .app_device_version = 0};
  log_v("Light endpoint created %d", _endpoint);
}

//set attribute method -> method overridden in child class
void ZigbeeLight::zbAttributeSet(const esp_zb_zcl_set_attr_value_message_t *message) {
  //check the data and call right method
  if (message->info.cluster == ESP_ZB_ZCL_CLUSTER_ID_ON_OFF) {
    if (message->attribute.id == ESP_ZB_ZCL_ATTR_ON_OFF_ON_OFF_ID && message->attribute.data.type == ESP_ZB_ZCL_ATTR_TYPE_BOOL) {
      _current_state = *(bool *)message->attribute.data.value;
      lightChanged();
    } else {
      log_w("Received message ignored. Attribute ID: %d not supported for On/Off Light", message->attribute.id);
    }
  } else {
    log_w("Received message ignored. Cluster ID: %d not supported for On/Off Light", message->info.cluster);
  }
}

void ZigbeeLight::lightChanged() {
  if (_on_light_change) {
    _on_light_change(_current_state);
  } else {
    log_w("No callback function set for light change");
  }
}

bool ZigbeeLight::setLight(bool state) {
  esp_zb_zcl_status_t ret = ESP_ZB_ZCL_STATUS_SUCCESS;
  _current_state = state;
  lightChanged();

  log_v("Updating on/off light state to %d", state);
  /* Update on/off light state */
  esp_zb_lock_acquire(portMAX_DELAY);
  ret = esp_zb_zcl_set_attribute_val(
    _endpoint, ESP_ZB_ZCL_CLUSTER_ID_ON_OFF, ESP_ZB_ZCL_CLUSTER_SERVER_ROLE, ESP_ZB_ZCL_ATTR_ON_OFF_ON_OFF_ID, &_current_state, false
  );
  esp_zb_lock_release();

  if (ret != ESP_ZB_ZCL_STATUS_SUCCESS) {
    log_e("Failed to set light state: 0x%x: %s", ret, esp_zb_zcl_status_to_name(ret));
    return false;
  }
  return reportLight();
}

bool ZigbeeLight::reportLight() {
  /* Send report attributes command */
  esp_zb_zcl_report_attr_cmd_t report_attr_cmd;
  report_attr_cmd.address_mode = ESP_ZB_APS_ADDR_MODE_16_ENDP_PRESENT;
  report_attr_cmd.zcl_basic_cmd.dst_addr_u.addr_short = 0x0000;
  report_attr_cmd.zcl_basic_cmd.dst_endpoint = 0x05;
  report_attr_cmd.attributeID = ESP_ZB_ZCL_ATTR_ON_OFF_ON_OFF_ID;
  report_attr_cmd.direction = ESP_ZB_ZCL_CMD_DIRECTION_TO_CLI;
  report_attr_cmd.clusterID = ESP_ZB_ZCL_CLUSTER_ID_ON_OFF;
  report_attr_cmd.zcl_basic_cmd.src_endpoint = _endpoint;
  report_attr_cmd.manuf_code = ESP_ZB_ZCL_ATTR_NON_MANUFACTURER_SPECIFIC;

  esp_zb_lock_acquire(portMAX_DELAY);
  esp_err_t ret = esp_zb_zcl_report_attr_cmd_req(&report_attr_cmd);
  esp_zb_lock_release();
  if (ret != ESP_OK) {
    log_e("Failed to send Light report: 0x%x: %s", ret, esp_err_to_name(ret));
    return false;
  }
  log_v("Light report sent");
  return true;
}

#endif  // CONFIG_ZB_ENABLED
