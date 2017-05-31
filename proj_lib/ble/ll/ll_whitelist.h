/*
 * ll_whitelist.h
 *
 *  Created on: 2016-9-22
 *      Author: Administrator
 */

#ifndef LL_WHITELIST_H_
#define LL_WHITELIST_H_

#include "../ble_common.h"



#define  	MAX_WHITE_LIST_SIZE    				4
#define 	MAX_WHITE_IRK_LIST_SIZE          	4

#define 	IS_RESOLVABLE_PRIVATE_ADDR(addr)  	((addr[5]&0xC0) == 0x40)

#define		MAC_MATCH8(md,ms)	(md[0]==ms[0] && md[1]==ms[1] && md[2]==ms[2] && md[3]==ms[3] && md[4]==ms[4] && md[5]==ms[5])
#define		MAC_MATCH16(md,ms)	(md[0]==ms[0] && md[1]==ms[1] && md[2]==ms[2])
#define		MAC_MATCH32(md,ms)	(md[0]==ms[0] && md[1]==ms[1])


//adv filter policy
#define 	ALLOW_SCAN_WL								BIT(0)
#define 	ALLOW_CONN_WL								BIT(1)

#define		ADV_FP_ALLOW_SCAN_ANY_ALLOW_CONN_ANY        0x00  // Process scan and connection requests from all devices
#define 	ADV_FP_ALLOW_SCAN_WL_ALLOW_CONN_ANY         0x01  // Process connection requests from all devices and only scan requests from devices that are in the White List.
#define 	ADV_FP_ALLOW_SCAN_ANY_ALLOW_CONN_WL         0x02  // Process scan requests from all devices and only connection requests from devices that are in the White List..
#define 	ADV_FP_ALLOW_SCAN_WL_ALLOW_CONN_WL          0x03  // Process scan and connection requests only from devices in the White List.

//adv filter policy set to zero, not use whitelist
#define		ADV_FP_NONE								    ADV_FP_ALLOW_SCAN_ANY_ALLOW_CONN_ANY


#define		SCAN_FP_ALLOW_ADV_ANY						0x00  //except direct adv address not match
#define 	SCAN_FP_ALLOW_ADV_WL        				0x01  //except direct adv address not match
#define 	SCAN_FP_ALLOW_UNDIRECT_ADV      			0x02  //and direct adv address match initiator's resolvable private MAC
#define 	SCAN_FP_ALLOW_ADV_WL_DIRECT_ADV_MACTH		0x03  //and direct adv address match initiator's resolvable private MAC


#define 	INITIATE_FP_ADV_SPECIFY        				0x00  //adv specified by host
#define 	INITIATE_FP_ADV_WL         					0x01  //adv in whitelist


typedef u8 irk_key_t[16];

typedef struct {
	u8 type;
	u8 address[BLE_ADDR_LEN];
	u8 reserved;
} wl_addr_t;

typedef struct {
	wl_addr_t  wl_addr_tbl[MAX_WHITE_LIST_SIZE];
	u8 	wl_addr_tbl_index;
	u8 	wl_irk_tbl_index;
} ll_whiteListTbl_t;


typedef struct {
	u8 type;
	u8 address[BLE_ADDR_LEN];
	u8 reserved;
	u8 irk[16];
} rl_addr_t;

typedef struct {
	rl_addr_t	tbl[MAX_WHITE_IRK_LIST_SIZE];
	u8 			idx;
} ll_ResolvingListTbl_t;




/*********************************************************************
 * @fn      ll_whiteList_reset
 *
 * @brief   API to reset the white list table.
 *
 * @param   None
 *
 * @return  LL Status
 */
ble_sts_t ll_whiteList_reset(void);

/*********************************************************************
 * @fn      ll_whiteList_add
 *
 * @brief   API to add new entry to white list
 *
 * @param   None
 *
 * @return  LL Status
 */
ble_sts_t ll_whiteList_add(u8 type, u8 *addr);
ble_sts_t ll_whiteList_add2(u8 *p);
u8 ll_whiteList_rsvd_field(u8 type, u8 *addr);

/*********************************************************************
 * @fn      ll_whiteList_delete
 *
 * @brief   API to delete entry from white list
 *
 * @param   type - The specified type
 *          addr - The specified address to be delete
 *
 * @return  LL Status
 */
ble_sts_t ll_whiteList_delete(u8 type, u8 *addr);

/*********************************************************************
 * @fn      ll_whiteList_getSize
 *
 * @brief   API to get total number of white list entry size
 *
 * @param   returnSize - The returned entry size
 *
 * @return  LL Status
 */
ble_sts_t ll_whiteList_getSize(u8 *returnPublicAddrListSize) ;


/*********************************************************************
 * @fn      ll_whiteList_search
 *
 * @brief   API to check if address is existed in white list table
 *
 * @param   None
 *
 * @return  BLE_SUCCESS(0, Exist in table)
 *              LL_ERR_ADDR_NOT_EXIST_IN_WHITE_LIST (0x46, Addr not exist in white list table)
 */
ble_sts_t ll_whiteList_search(u8 type, u8 *addr) ;

u8 * ll_searchAddrInWhiteListTbl(u8 type, u8 *addr);  //stack use

ble_sts_t 	ll_whiteList_reset(void);

ble_sts_t 	ll_resolvingList_reset(void);

ble_sts_t  ll_resolvingList_add(u8 type, u8 *addr, u8 *irk);

ble_sts_t  ll_resolvingList_delete(u8 type, u8 *addr);

ble_sts_t ll_resolvingList_getSize(u8 *Size);

ll_whiteListTbl_t	ll_whiteList_tbl;
ll_ResolvingListTbl_t	ll_resolvingList_tbl;

#endif /* LL_WHITELIST_H_ */
