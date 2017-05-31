/*
 * ble_smp.h
 *
 *  Created on: 2016-9-27
 *      Author: Telink
 */

#ifndef BLE_SMP_H_
#define BLE_SMP_H_


typedef struct {
	u8 addr_type;
	u8 addr_mac[6];
}addr_info_t;


typedef union {
	struct{
		u8 bondingFlag : 2;
		u8 MITM : 1;
		u8 SC	: 1;
		u8 keyPress: 1;
		u8 rsvd: 3;
	};
	u8 authType;
}smp_authReq_t;

typedef union{
	struct {
		u32 encKey : 1;
		u32 idKey : 1;
		u32 sign  : 1;
		u32 linkKey : 4;
	};
	u8 keyIni;
}smp_keyDistribution_t;

typedef struct{
	u8 code;  //req = 0x01; rsp = 0x02;
	u8 ioCapablity;
	u8 oobDataFlag;
	smp_authReq_t authReq;
	u8 maxEncrySize;

	smp_keyDistribution_t initKeyDistribution;
	smp_keyDistribution_t rspKeyDistribution;
}smp_paring_req_rsp_t;



typedef struct{
	u8  code; //0x04
	u16 randomValue[16];
}smp_paring_random_t;

typedef struct{
	u8  code; //0x03
	u16 confirmValue[16];
}smp_paring_confirm_t;

typedef struct{
	u8 code; // 0x05
	u8 reason;
}smp_paring_failed_t;

typedef struct{
	u8 code;//0x0b

	smp_authReq_t authReq;
}smp_secure_req_t;

typedef struct{
	u8  code;//0xa
	u8 	signalKey[16];
}smp_signal_info_t;

typedef struct{
	u8 code;//0x9
	u8 addrType;
	u8 bdAddr[6];
}smp_id_addr_info_t;

typedef struct{
	u8 code;//0x8
	u8 idResolveKey[16];
}smp_id_info_t;

typedef struct{
	u8  code;//0x7
	u16 edivPtr[2];
	u8 	masterRand[8];
}smp_master_id_t;

typedef struct{
	u8  code;//0x6
	u8 	LTK[16];
}smp_enc_info_t;

// -------add core 4.2 ------
typedef struct{
	u8  code;//0xc
	u8  publicKeyX[32];
	u8  publicKeyY[32];
}smp_paring_public_key_t;

typedef struct{
	u8  code;//0xd
	u8  DHKeyCheck[16];
}smp_DHkey_check_t;

typedef struct{
	u8  code;//0xe
	u8  notifyType;
}smp_key_notify_t;

typedef struct{
	u8 sc_sk_dhk_own[32];  // keep sk before receive Ea. and keep dhkey after that.
	u8 sc_pk_own[64];
	u8 sc_pk_peer[64];
}smp_sc_key_t;



/*
 * smp parameter about peer device.
 * */
typedef struct{
	u8		conn_handle;
	u8		role;
	u8		paring_enable;
	u8 		rsvd;
//	u16 	rsvd2;
	u8		peer_addr[6];
	u8 		peer_addr_type;
	u8		peer_key_size;   // bond and key_size
	u16 	peer_ediv;
	u8		peer_random[8];
	u8		is_bond;
	u8 		smp_paring_sequence;
	u8		peer_ltk[16];
	u8		peer_csrk[16];
	u8		peer_irk[16];
	u8		paring_peer_rand[16];  //offset 80

}smp_param_peer_t;

/*
 * smp parameter need save to flash.
 * */
#define		FLAG_SMP_PARAM_SAVE					0x5371
#define		TYPE_WHITELIST						BIT(0)
#define		TYPE_RESOLVINGLIST					BIT(1)

typedef struct {  //82
	u16		flag;
	u8		type;
	u8		peer_key_size;  // 0x00 for unpair,
	u8		peer_addr_type;		//wl_addr_t: type-adr-rsvd
	u8		peer_addr[6];
	u8		rsvd;
	u16		peer_ediv;
	u8		peer_random[8];
	u8		peer_ltk[16];
	u8		peer_csrk[16];
	u8		peer_irk[16];
	u8 		own_ltk[16];

}smp_param_save_t;

/*
 * smp parameter about own device.
 * */
typedef struct{
	u8  					conn_handle;
	smp_paring_req_rsp_t  	paring_req;
	smp_paring_req_rsp_t  	paring_rsp;
	u8						own_conn_type;  //current connection peer own type
	u8						own_conn_addr[6];
	smp_authReq_t			auth_req;
	u8						paring_tk[16];   // in security connection to keep own random
	u8						paring_confirm[16];  // in security connection oob mode to keep peer random
	u8						own_ltk[16];   //used for generate ediv and random
}smp_param_own_t;

u8 cur_enc_keysize;

typedef struct {
	/* data */
	u8 csrk[16];
	u32 signCounter;
} smp_secSigInfo_t;




typedef struct {
	u8  secReq_disable;
	u8	secReq_pending;
	u8	secReq_laterSend;
	u8  rsvd;

	u32  secReq_sendTime_ms;
} smp_ctrl_t;

extern smp_ctrl_t 	blc_smp_ctrl;

void blc_smp_checkSecurityReqeustSending(u32 connStart_tick);


typedef enum {
	JUST_WORKS,
	PK_RESP_INPUT,  // Initiator displays PK, initiator inputs PK
	PK_INIT_INPUT,  // Responder displays PK, responder inputs PK
	OK_BOTH_INPUT,  // Only input on both, both input PK
	OOB             // OOB available on both sides
} stk_generationMethod_t;

// IO Capability Values
typedef enum {
	IO_CAPABILITY_DISPLAY_ONLY = 0,
	IO_CAPABILITY_DISPLAY_YES_NO,
	IO_CAPABILITY_KEYBOARD_ONLY,
	IO_CAPABILITY_NO_INPUT_NO_OUTPUT,
	IO_CAPABILITY_KEYBOARD_DISPLAY, // not used by secure simple pairing
	IO_CAPABILITY_UNKNOWN = 0xff
} io_capability_t;

// horizontal: initiator capabilities
// vertial:    responder capabilities
static const stk_generationMethod_t stk_generation_method[5][5] = {
	{ JUST_WORKS,      JUST_WORKS,       PK_INIT_INPUT,   JUST_WORKS,    PK_INIT_INPUT },
	{ JUST_WORKS,      JUST_WORKS,       PK_INIT_INPUT,   JUST_WORKS,    PK_INIT_INPUT },
	{ PK_RESP_INPUT,   PK_RESP_INPUT,    OK_BOTH_INPUT,   JUST_WORKS,    PK_RESP_INPUT },
	{ JUST_WORKS,      JUST_WORKS,       JUST_WORKS,      JUST_WORKS,    JUST_WORKS    },
	{ PK_RESP_INPUT,   PK_RESP_INPUT,    PK_INIT_INPUT,   JUST_WORKS,    PK_RESP_INPUT },
};

#define IO_CAPABLITY_DISPLAY_ONLY		0x00
#define IO_CAPABLITY_DISPLAY_YESNO		0x01
#define IO_CAPABLITY_KEYBOARD_ONLY		0x02
#define IO_CAPABLITY_NO_IN_NO_OUT		0x03
#define IO_CAPABLITY_KEYBOARD_DISPLAY	0x04

#define PASSKEY_TYPE_ENTRY_STARTED		0x00
#define PASSKEY_TYPE_DIGIT_ENTERED		0x01
#define PASSKEY_TYPE_DIGIT_ERASED		0x02
#define PASSKEY_TYPE_CLEARED			0x03
#define PASSKEY_TYPE_ENTRY_COMPLETED	0x04

#define PARING_FAIL_REASON_PASSKEY_ENTRY			0x01
#define PARING_FAIL_REASON_OOB_NOT_AVAILABLE		0x02
#define PARING_FAIL_REASON_AUTH_REQUIRE				0x03
#define PARING_FAIL_REASON_CONFIRM_FAILED			0x04
#define PARING_FAIL_REASON_PARING_NOT_SUPPORTED		0x05
#define PARING_FAIL_REASON_ENCRYPT_KEY_SIZE			0x06
//-- core 4.2
#define PARING_FAIL_REASON_CMD_NOT_SUPPORT			0x07
#define PARING_FAIL_REASON_UNSPECIFIED_REASON		0x08
#define PARING_FAIL_REASON_REPEATED_ATTEMPT			0x09
#define PARING_FAIL_REASON_INVAILD_PARAMETER		0x0a
#define PARING_FAIL_REASON_DHKEY_CHECK_FAIL			0x0b
#define PARING_FAIL_REASON_NUMUERIC_FAILED			0x0c
#define PARING_FAIL_REASON_BREDR_PARING				0x0d
#define PARING_FAIL_REASON_CROSS_TRANSKEY_NOT_ALLOW		0x0e

#define	ENCRYPRION_KEY_SIZE_MAXINUM				16
#define	ENCRYPRION_KEY_SIZE_MINIMUN				7

typedef enum{
	SMP_PARING_DISABLE_TRRIGER = 0,
	SMP_PARING_CONN_TRRIGER ,
	SMP_PARING_PEER_TRRIGER,
}smp_paringTrriger_t;



/******************************* User Interface  ************************************/


/**************************************************
 * API used for slave enable the device paring.
 * encrypt_en	SMP_PARING_DISABLE_TRRIGER   -  not allow encryption
 * 				SMP_PARING_CONN_TRRIGER      -  paring process start once connect.
 * 				SMP_PARING_PEER_TRRIGER      -  paring process start once peer device start.
 */
int bls_smp_enableParing (smp_paringTrriger_t encrypt_en);


/**************************************************
 * API used for master enable the device paring within connhandle.
 * encrypt_en	SMP_PARING_DISABLE_TRRIGER   -  not allow encryption
 * 				SMP_PARING_CONN_TRRIGER      -  paring process start once connect.
 * 				SMP_PARING_PEER_TRRIGER      -  paring process start once peer device start.
 */
int blm_smp_enableParing ( smp_paringTrriger_t encrypt_en);


/**************************************************
 * API used for master start encryption within the connhandle.
 */
int blm_smp_startEncryption (u16 connhandle);




/*************************************************
 * 	used for enable oob flag
 */
void blc_smp_enableOobFlag (int en, u8 *oobData);

/*************************************************
 * 	used for set MAX key size
 * */
void blc_smp_setMaxKeySize (u8 maxKeySize);

/*************************************************
 * 	@brief 		used for enable authentication MITM
 * 	@return  	0 - setting success
 * 				others - pin code not in ranged.(0 ~ 999,999)
 */
int blc_smp_enableAuthMITM (int en, u32 pinCodeInput);

/*************************************************
 * 	@brief 		used for enable authentication bonding flag.
 */
int blc_smp_enableBonding (int en);

/*************************************************
 * 	used for set IO capability
 * */
void blc_smp_setIoCapability (u8 ioCapablility);

/*
 * API used for set distribute key enable.
 * */
smp_keyDistribution_t blc_smp_setDistributeKey (u8 LTK_distributeEn, u8 IRK_distributeEn, u8 CSRK_DistributeEn);

/*
 * API used for set distribute key enable.
 * */
smp_keyDistribution_t blc_smp_expectDistributeKey (u8 LTK_distributeEn, u8 IRK_distributeEn, u8 CSRK_DistributeEn);

/**************************************************
 * API used for get peer address and address type in slave mode.
 * Parameter : peer_addr_info - [out]
 * Return : BLE_SUCCESS  - exist peer device.
 * 			Others - not exist peer device.
 */
//int bls_smp_getPeerAddrInfo (addr_info_t* peer_addr_info);

/************************* Stack Interface, user can not use!!! ***************************/


/*
 * Return STK generate method.
 * */
int blc_smp_getGenMethod ();

/**************************************************
 * 	used for handle link layer callback (ltk event callback), packet LL_ENC_request .
 */
int bls_smp_getLtkReq (u8 * random, u16 ediv);

/*
 * Used for set smp parameter to default.
 * */
void blc_smp_paramInitDefault ( );

/**************************************************
 * 	used for save parameter in paring buffer
 */
int bls_smp_setAddress (u8 *p);

/*************************************************
 * 	used for smp save in flash clean.
 */
void blc_smp_paramFlashClean ();

/*************************************************
 * 	@brief 		used for reset smp param to default value.
 */
int blc_smp_paramInit ();

/*
 * Used for handle master_self addr and addr type. called from gap
 *
 * */
void blm_smp_setNewConnInfo(u8 role, u8 ownAddrType, u8* ownAddr, u8 peerAddrType, u8* peerAddr);

/**************************************************
 * Used for handle connection complete event
 */
void blm_smp_connComplete(u16 connhandle, u8* bd_addr, u8 link_type, u8 encrypt_en);


/**************************************************
 *  API used for slave start encryption.
 */
int bls_smp_startEncryption ();

/**************************************************
 * Used for notify sm layer the state change in ll;
 */
void blm_smp_cryptStateChanged (u8 connhandle, u8 cur_crypt_state);
#endif /* BLE_SMP_H_ */
