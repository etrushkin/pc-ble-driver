/*
 * ble_nfc_app.c
 *
 *  Created on: Jul 5, 2019
 *      Author: trushke
 */

#include "ble_nfc.h"
#include "ble_nfc_app.h"

#include "ble.h"
#include "ble_serialization.h"
#include "ble_gap_struct_serialization.h"

#include <stdint.h>
#include <assert.h>
#include <string.h>


#ifdef __cplusplus
extern "C" {
#endif

	static uint32_t nfc_ble_gap_dev_name_t_dec(uint8_t const * const p_buf,
	                                           uint32_t              buf_len,
	                                           uint32_t * const      p_index,
	                                           void *                p_void_struct)
	{
		int8_t name_type;
		SER_STRUCT_DEC_BEGIN(nfc_ble_gap_dev_name_t);

		SER_PULL_int8(&name_type);

		uint8_t *p_data = p_struct->name;
		p_struct->len = NFC_BLE_DEVICE_NAME_MAX_LENGTH;
		SER_PULL_len8data(&p_data, &p_struct->len);      // uint8_t * p_name; // uint8_t len;

		p_struct->name_type = (ble_advdata_name_type_t) name_type; // ble_advdata_name_type_t  name_type;  // Tiny enum
		if(NULL == p_data)
		{
			p_struct->name_type = BLE_ADVDATA_NO_NAME;
		}

		SER_STRUCT_DEC_END;
	}


	static uint32_t nfc_ble_gap_addr_t_dec(uint8_t const * const p_buf,
	                                       uint32_t              buf_len,
	                                       uint32_t * const      p_index,
	                                       void *                p_void_struct)
	{
		SER_STRUCT_DEC_BEGIN(ble_gap_addr_t);
		ble_gap_addr_t *p_ble_gap_addr = p_struct;

		SER_PULL_COND(&p_ble_gap_addr, ble_gap_addr_t_dec);
		if(NULL == p_ble_gap_addr)
		{
			p_struct->addr_type = BLE_GAP_ADDR_TYPE_ANONYMOUS;
			p_struct->addr_id_peer = 0;

			assert(sizeof(p_struct->addr) >= BLE_GAP_ADDR_LEN);
			memset(p_struct->addr, 0, sizeof(p_struct->addr));
		}

		SER_STRUCT_DEC_END;
	}

	static uint32_t ble_advdata_tk_value_t_dec(uint8_t const * const p_buf,
	                                           uint32_t              buf_len,
	                                           uint32_t * const      p_index,
	                                           void *                p_void_struct)
	{
		SER_STRUCT_DEC_BEGIN(ble_advdata_tk_value_t);
		SER_PULL_uint8array(p_struct->tk, BLE_GAP_SEC_KEY_LEN);
		SER_STRUCT_DEC_END;
	}


	static uint32_t ble_nfc_ble_advdata_tk_value_t_dec(uint8_t const * const p_buf,
	                                                   uint32_t              buf_len,
	                                                   uint32_t * const      p_index,
	                                                   void *                p_void_struct)
	{
		SER_STRUCT_DEC_BEGIN(nfc_ble_advdata_tk_value_t);
		ble_advdata_tk_value_t *p_ble_advdata_tk_value = &p_struct->tk_struct;

		SER_PULL_COND(&p_ble_advdata_tk_value, ble_advdata_tk_value_t_dec);
		if(NULL == p_ble_advdata_tk_value)
		{
			p_struct->present = false;
			memset(p_struct->tk_struct.tk, 0, BLE_GAP_SEC_KEY_LEN);
		}

		SER_STRUCT_DEC_END;
	}


	static uint32_t nfc_lesc_confirm_value_t_dec(uint8_t const * const p_buf,
	                                             uint32_t              buf_len,
	                                             uint32_t * const      p_index,
	                                             void *                p_void_struct)
	{
		SER_STRUCT_DEC_BEGIN(nfc_lesc_confirm_value_t);
		uint8_t *p_arr = p_struct->value;
		p_struct->len = AD_TYPE_CONFIRM_VALUE_DATA_SIZE;

		SER_PULL_len8data(&p_arr, &p_struct->len);

		if(NULL == p_arr)
		{
			memset(p_struct->value, 0, AD_TYPE_CONFIRM_VALUE_DATA_SIZE);
		}

		SER_STRUCT_DEC_END;
	}

	static uint32_t nfc_lesc_random_value_t_dec(uint8_t const * const p_buf,
	                                            uint32_t              buf_len,
	                                            uint32_t * const      p_index,
	                                            void *                p_void_struct)
	{
		SER_STRUCT_DEC_BEGIN(nfc_lesc_random_value_t);
		uint8_t *p_arr = p_struct->value;
		p_struct->len = AD_TYPE_RANDOM_VALUE_DATA_SIZE;

		SER_PULL_len8data(&p_arr, &p_struct->len);

		if(NULL == p_arr)
		{
			memset(p_struct->value, 0, AD_TYPE_RANDOM_VALUE_DATA_SIZE);
		}

		SER_STRUCT_DEC_END;
	}


	static uint32_t ble_advdata_le_role_t_dec(uint8_t const * const p_buf,
	                                          uint32_t              buf_len,
	                                          uint32_t * const      p_index,
	                                          void *                p_void_struct)
	{
		int8_t le_role;
		SER_STRUCT_DEC_BEGIN(ble_advdata_le_role_t);

		SER_PULL_int8(&le_role);
		*p_struct = (ble_advdata_le_role_t)le_role;

		SER_STRUCT_DEC_END;
	}

	static uint32_t nfc_sec_mgr_oob_flags_t_dec(uint8_t const * const p_buf,
	                                            uint32_t              buf_len,
	                                            uint32_t * const      p_index,
	                                            void *                p_void_struct)
	{
		SER_STRUCT_DEC_BEGIN(nfc_sec_mgr_oob_flags_t);
		uint8_t *p_arr = p_struct->value;
		p_struct->len = AD_TYPE_OOB_FLAGS_DATA_SIZE;

		SER_PULL_len8data(&p_arr, &p_struct->len);

		if(NULL == p_arr)
		{
			memset(p_struct->value, 0, AD_TYPE_OOB_FLAGS_DATA_SIZE);
		}

		SER_STRUCT_DEC_END;
	}


	uint32_t ble_nfc_evt_tag_acquired_dec(uint8_t const * const p_buf,
	                                      uint32_t              packet_len,
	                                      ble_evt_t * const     p_event,
	                                      uint32_t * const      p_event_len)
	{
		nfc_ble_oob_pairing_data_t * const p_struct = &p_event->evt.nfc_evt.params.tag_acquired_evt.ble_oob_pairing_data;


		SER_EVT_DEC_BEGIN(BLE_NFC_EVT_TAG_ACQUIRED, nfc, tag_acquired);

		SER_PULL_FIELD(&p_struct->device_name, nfc_ble_gap_dev_name_t_dec);

		SER_PULL_FIELD(&p_struct->device_addr, nfc_ble_gap_addr_t_dec);
		SER_PULL_FIELD(&p_struct->tk_value, ble_nfc_ble_advdata_tk_value_t_dec);

		SER_PULL_FIELD(&p_struct->lesc_confirm_value, nfc_lesc_confirm_value_t_dec);
		SER_PULL_FIELD(&p_struct->lesc_random_value, nfc_lesc_random_value_t_dec);

		SER_PULL_FIELD(&p_struct->le_role, ble_advdata_le_role_t_dec);
		SER_PULL_uint16(&p_struct->appearance);
		SER_PULL_uint8(&p_struct->flags);

		SER_PULL_FIELD(&p_struct->sec_mgr_oob_flags, nfc_sec_mgr_oob_flags_t_dec);

		SER_EVT_DEC_END;
	}


	uint32_t ble_nfc_field_enable_req_enc(uint8_t               enable,
	                                      uint8_t * const       p_buf,
	                                      uint32_t *            p_buf_len)
	{
		SER_REQ_ENC_BEGIN(BLE_NFC_FIELD_ENABLE);
		SER_PUSH_uint8(&enable);
		SER_REQ_ENC_END;
	}


	uint32_t ble_nfc_field_enable_rsp_dec(uint8_t const * const p_buf,
	                                      uint32_t              packet_len,
	                                      uint32_t * const      p_result_code)
	{
		SER_RSP_DEC_RESULT_ONLY(BLE_NFC_FIELD_ENABLE);
	}


#ifdef __cplusplus
}
#endif
