/*
 * ble_nfc.h
 *
 *  Created on: Jul 2, 2019
 *      Author: trushke
 */

#ifndef PC_BLE_DRIVER_INCLUDE_SD_API_V6_BLE_NFC_H_
#define PC_BLE_DRIVER_INCLUDE_SD_API_V6_BLE_NFC_H_

#include "ble_ranges.h"
#include "nrf_svc.h"
#include "adapter.h"
#include "ble_gap.h"

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define AD_TYPE_OOB_FLAGS_DATA_SIZE     1UL
#define AD_TYPE_CONFIRM_VALUE_DATA_SIZE 16UL
#define AD_TYPE_RANDOM_VALUE_DATA_SIZE  16UL
#define NFC_BLE_DEVICE_NAME_MAX_LENGTH	30

	/**@brief NFC Event IDs.
	 */
	enum BLE_NFC_EVTS
	{
		BLE_NFC_EVT_TAG_ACQUIRED  = BLE_NFC_EVT_BASE,
	};

	/**@brief NFC Commands IDs.
	 */
	enum BLE_NFC_SVCS
	{
		BLE_NFC_FIELD_ENABLE  = BLE_NFC_SVC_BASE,
	};

	/**@brief Advertising data name type. This enumeration contains the options available for the device name inside
	 *        the advertising data. */
	typedef enum
	{
		BLE_ADVDATA_NO_NAME,                                              /**< Include no device name in advertising data. */
		BLE_ADVDATA_SHORT_NAME,                                           /**< Include short device name in advertising data. */
		BLE_ADVDATA_FULL_NAME                                             /**< Include full device name in advertising data. */
	} ble_advdata_name_type_t;

	/**@brief Advertising data LE Role types. This enumeration contains the options available for the LE role inside
	 *        the advertising data. */
	typedef enum
	{
		BLE_ADVDATA_ROLE_NOT_PRESENT = 0,                                 /**< LE Role AD structure not present. */
		BLE_ADVDATA_ROLE_ONLY_PERIPH,                                     /**< Only Peripheral Role supported. */
		BLE_ADVDATA_ROLE_ONLY_CENTRAL,                                    /**< Only Central Role supported. */
		BLE_ADVDATA_ROLE_BOTH_PERIPH_PREFERRED,                           /**< Peripheral and Central Role supported. Peripheral Role preferred for connection establishment. */
		BLE_ADVDATA_ROLE_BOTH_CENTRAL_PREFERRED                           /**< Peripheral and Central Role supported. Central Role preferred for connection establishment */
	} ble_advdata_le_role_t;



	/**@brief Security Manager TK value. */
	typedef struct
	{
	  uint8_t tk[BLE_GAP_SEC_KEY_LEN];      /**< Array containing TK value in little-endian format. */
	} ble_advdata_tk_value_t;

	typedef struct
	{
		bool present;
		ble_advdata_tk_value_t tk_struct;      /**< Array containing TK value in little-endian format. */
	} nfc_ble_advdata_tk_value_t;

	/**@brief Bluetooth Low Energy GAP device name. */
	typedef struct
	{
		ble_advdata_name_type_t   name_type;    /**< See @ref ble_advdata_name_type_t. */
		uint8_t                   len;          /**< Length of device name. */
		uint8_t                   name[NFC_BLE_DEVICE_NAME_MAX_LENGTH];       /**< Pointer to the buffer with device name. */
	} nfc_ble_gap_dev_name_t;

	typedef struct
	{
		uint8_t                   len;
		uint8_t                   value[AD_TYPE_CONFIRM_VALUE_DATA_SIZE];
	} nfc_lesc_confirm_value_t;

	typedef struct
	{
		uint8_t                   len;
		uint8_t                   value[AD_TYPE_RANDOM_VALUE_DATA_SIZE];
	} nfc_lesc_random_value_t;

	typedef struct
	{
		uint8_t                   len;
		uint8_t                   value[AD_TYPE_OOB_FLAGS_DATA_SIZE];
	} nfc_sec_mgr_oob_flags_t;

#if 0
	// Brace-bridge -side (hardware) definition
	typedef struct
	{
	    ble_gap_dev_name_t        device_name;              /**< See @ref ble_gap_dev_name_t. */
	    ble_gap_addr_t          * p_device_addr;            /**< See @ref ble_gap_addr_t. */
	    ble_advdata_tk_value_t  * p_tk_value;               /**< See @ref ble_advdata_tk_value_t. */
	    uint8_t                 * p_lesc_confirm_value;     /**< LESC OOB confirmation data. */
	    uint8_t                 * p_lesc_random_value;      /**< LESC OOB random data. */
	    ble_advdata_le_role_t     le_role;                  /**< See @ref ble_advdata_le_role_t. */
	    uint16_t                  appearance;               /**< Advertising data Appearance field. */
	    uint8_t                   flags;                    /**< Advertising data Flags field. */
	    uint8_t                 * p_sec_mgr_oob_flags;      /**< Security Manager Out Of Band Flags data field. */
	} nfc_ble_oob_pairing_data_t;
#endif

	// This definition has statically allocated memory for de-serialization
	/**@brief BLE Advertising data that is relevant for OOB pairing. */
	typedef struct
	{
		nfc_ble_gap_dev_name_t     device_name;         //
		ble_gap_addr_t             device_addr;         // Conditional
		nfc_ble_advdata_tk_value_t tk_value;            // Conditional
		nfc_lesc_confirm_value_t   lesc_confirm_value;  // Conditional
		nfc_lesc_random_value_t    lesc_random_value;   // Conditional
		ble_advdata_le_role_t      le_role;             //
		uint16_t                   appearance;          //
		uint8_t                    flags;               //
		nfc_sec_mgr_oob_flags_t    sec_mgr_oob_flags;   // Conditional
	} nfc_ble_oob_pairing_data_t;

	typedef struct
	{
		uint8_t enable; /**< Enable field. */
	} nfc_opt_field_t;

	typedef union
	{
		nfc_opt_field_t  field_opt;
	} nfc_opt_t;


	typedef struct
	{
		nfc_ble_oob_pairing_data_t  ble_oob_pairing_data;
	} ble_nfc_evt_tag_acquired_t;

	typedef struct
	{
		union
		{
			ble_nfc_evt_tag_acquired_t tag_acquired_evt;
		}params;
	} ble_nfc_evt_t;


	SVCALL(BLE_NFC_FIELD_ENABLE, uint32_t, nfc_field_enable(adapter_t *adapter, uint8_t enable));

#ifdef __cplusplus
}
#endif

#endif /* PC_BLE_DRIVER_INCLUDE_SD_API_V6_BLE_NFC_H_ */
