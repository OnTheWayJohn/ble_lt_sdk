/*
 * blt_cmp_nv.h
 *
 *  Created on: 2016-10-13
 *      Author: Telink
 */

#ifndef BLT_CMP_NV_H_
#define BLT_CMP_NV_H_


extern 			int				SMP_PARAM_NV_ADDR_START;

#define 		SMP_PARAM_NV_UNIT						128
#define			SMP_PARAM_NV_MAX_LEN					4096

#define			SMP_PARAM_NV_ADDR_END					(SMP_PARAM_NV_ADDR_START + SMP_PARAM_NV_MAX_LEN - 1)

#define			SMP_PARAM_NV_SEC_ADDR_START				(SMP_PARAM_NV_ADDR_START + SMP_PARAM_NV_MAX_LEN)
#define			SMP_PARAM_NV_SEC_ADDR_END				(SMP_PARAM_NV_SEC_ADDR_START + SMP_PARAM_NV_MAX_LEN - 1)

#define			SMP_NV_SUCCESS							0
#define			SMP_NV_FAIL								1

#define 		SMP_GET_DEVICE_INFO						BIT(0)
#define 		SMP_SET_DEVICE_INFO 					BIT(1)



void bls_smp_configParingSecurityInfoStorageAddr (int addr);


/*
 * Find the addr with the addr and type.
 * return 0 with no find associated vs parameter. maybe no bond.
 * Research from end to start addr.
 * */
u32 smp_param_flashAddrPeer (u8 addr_type, u8* addr, int unpair_en);

/*
 * Used to clean flash when flash save parameter full. return SMP_NV_SUCCESS or SMP_NV_FAIL
 * */
int smp_param_unpair (u8 addr_type, u8* addr);

/*
 * Used to save data base on data. return nv addr
 * */
u32 smp_param_nv (smp_param_save_t* save_param, u8 list_type);

/*
 * Used for load smp parameter base on addr type
 * */
u32 smp_param_loadByAddr (u8 addr_type, u8* addr, smp_param_save_t* smp_param_load);

/*
 * Used for load smp parameter base ediv and random[8]
 * */
u32 smp_param_loadByRand(u16 ediv, u8* random, smp_param_save_t* smp_param_load);

/*
 * Used for load smp parameter base on addr type
 * */
u32 smp_param_loadByAddr(u8 addr_type, u8* addr, smp_param_save_t* smp_param_load);

void smp_param_loadflash ();
void smp_param_reset ();

#endif /* BLT_CMP_NV_H_ */
