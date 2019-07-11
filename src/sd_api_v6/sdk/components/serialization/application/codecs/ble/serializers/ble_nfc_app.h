/*
 * ble_nfc_app.h
 *
 *  Created on: Jul 5, 2019
 *      Author: trushke
 */

#ifndef PC_BLE_DRIVER_SRC_SD_API_V6_SDK_COMPONENTS_SERIALIZATION_APPLICATION_CODECS_BLE_SERIALIZERS_BLE_NFC_APP_H_
#define PC_BLE_DRIVER_SRC_SD_API_V6_SDK_COMPONENTS_SERIALIZATION_APPLICATION_CODECS_BLE_SERIALIZERS_BLE_NFC_APP_H_

#include "ble_nfc.h"
#include "ble.h"

#ifdef __cplusplus
extern "C" {
#endif

uint32_t ble_nfc_evt_tag_acquired_dec(uint8_t const * const p_buf,
                                      uint32_t              packet_len,
                                      ble_evt_t * const     p_event,
                                      uint32_t * const      p_event_len);

uint32_t ble_nfc_field_enable_req_enc(uint8_t               enable,
                                      uint8_t * const       p_buf,
                                      uint32_t *            p_buf_len);


uint32_t ble_nfc_field_enable_rsp_dec(uint8_t const * const p_buf,
                                      uint32_t              packet_len,
                                      uint32_t * const      p_result_code);

#ifdef __cplusplus
}
#endif

#endif /* PC_BLE_DRIVER_SRC_SD_API_V6_SDK_COMPONENTS_SERIALIZATION_APPLICATION_CODECS_BLE_SERIALIZERS_BLE_NFC_APP_H_ */
