/*
 * ble_nfc_impl.cpp
 *
 *  Created on: Jul 2, 2019
 *      Author: trushke
 */

#include "ble_nfc.h"
#include "ble_nfc_app.h"

#include "adapter.h"
#include "ble_common.h"
#include "ble_serialization.h"
#include "ble_gap_struct_serialization.h"
#include "ble_gap.h"

#include <cstdint>


uint32_t nfc_field_enable(adapter_t *adapter, uint8_t enable)
{
    encode_function_t encode_function = [&] (uint8_t *buffer, uint32_t *length) -> uint32_t {
        return ble_nfc_field_enable_req_enc(enable, buffer, length);
    };

    decode_function_t decode_function = [&] (uint8_t *buffer, uint32_t length, uint32_t *result) -> uint32_t {
        return ble_nfc_field_enable_rsp_dec(buffer, length, result);
    };

    return encode_decode(adapter, encode_function, decode_function);
}
