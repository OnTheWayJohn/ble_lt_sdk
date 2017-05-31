
//  Version : 1.0.2

#ifndef __WEIXIN_H__
#define __WEIXIN_H__

#if _WIN32
#include <string.h>
//#include <stdlib.h>
#include <stdint.h>
#include "aes/aes.h"

#ifdef __cplusplus
extern "C" {
#endif

#else
#include "../../proj/tl_common.h"
#endif

typedef signed int int32_t;
typedef unsigned int uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char uint8_t;

typedef struct
{
    uint8_t *data;
    int len;
} Bytes;

typedef struct
{
    const uint8_t *data;
    int len;
} CBytes;

typedef struct
{
    char *str;
    int len;
} String;

typedef struct
{
    const char *str;
    int len;
} CString;

typedef uint8_t Message;

typedef struct 
{
    const uint8_t *unpack_buf;
    uint8_t *pack_buf;
    int buf_len;
    int buf_offset;
} Epb;

/*
 * embeded protobuf unpack functions
 */

void epb_unpack_init(Epb *e, const uint8_t *buf, int len);
int epb_has_tag(Epb *e, uint16_t tag);

//Varint
int32_t epb_get_int32(Epb *e, uint16_t tag);
uint32_t epb_get_uint32(Epb *e, uint16_t tag);
int32_t epb_get_sint32(Epb *e, uint16_t tag);
int epb_get_bool(Epb *e, uint16_t tag);
int epb_get_enum(Epb *e, uint16_t tag);

//Length Delimited
const char *epb_get_string(Epb *e, uint16_t tag, int *len);
const uint8_t *epb_get_bytes(Epb *e, uint16_t tag, int *len);
const Message *epb_get_message(Epb *e, uint16_t tag, int *len);

//Length Delimited Packed Repeadted Field
//TODO

//Fixed32
uint32_t epb_get_fixed32(Epb *e, uint16_t tag);
int32_t epb_get_sfixed32(Epb *e, uint16_t tag);
float epb_get_float(Epb *e, uint16_t tag);

/*
 * embeded protobuf pack functions
 */

void epb_pack_init(Epb *e, uint8_t *buf, int len);
int epb_get_packed_size(Epb *e);

//Varint
int epb_set_int32(Epb *e, uint16_t tag, int32_t value);
int epb_set_uint32(Epb *e, uint16_t tag, uint32_t value);
int epb_set_sint32(Epb *e, uint16_t tag, int32_t value);
int epb_set_bool(Epb *e, uint16_t tag, int value);
int epb_set_enum(Epb *e, uint16_t tag, int value);

//Length Delimited
int epb_set_string(Epb *e, uint16_t tag, const char *data, int len);
int epb_set_bytes(Epb *e, uint16_t tag, const uint8_t *data, int len);
int epb_set_message(Epb *e, uint16_t tag, const Message *data, int len);

//Length Delimited Packed Repeadted Field
//TODO

//Fixed32
int epb_set_fixed32(Epb *e, uint16_t tag, uint32_t value);
int epb_set_sfixed32(Epb *e, uint16_t tag, int32_t value);
int epb_set_float(Epb *e, uint16_t tag, float value);

//Pack size
int epb_varint32_pack_size(uint16_t tag, uint32_t value, int is_signed);
int epb_fixed32_pack_size(uint16_t tag);
int epb_length_delimited_pack_size(uint16_t tag, int len);

//////////////////////////////////
// epb_MmBp
//////////////////////////////////
typedef enum
{
	ECI_none = 0,
	ECI_req_auth = 10001,
	ECI_req_sendData = 10002,
	ECI_req_init = 10003,
	ECI_resp_auth = 20001,
	ECI_resp_sendData = 20002,
	ECI_resp_init = 20003,
	ECI_push_recvData = 30001,
	ECI_push_switchView = 30002,
	ECI_push_switchBackgroud = 30003,
	ECI_err_decode = 29999
} EmCmdId;

typedef enum
{
	EEC_system = -1,
	EEC_needAuth = -2,
	EEC_sessionTimeout = -3,
	EEC_decode = -4,
	EEC_deviceIsBlock = -5,
	EEC_serviceUnAvalibleInBackground = -6,
	EEC_deviceProtoVersionNeedUpdate = -7,
	EEC_phoneProtoVersionNeedUpdate = -8,
	EEC_maxReqInQueue = -9,
	EEC_userExitWxAccount = -10
} EmErrorCode;

typedef enum
{
	EAM_md5 = 1,
	EAM_macNoEncrypt = 2
} EmAuthMethod;

typedef enum
{
	EIRFF_userNickName = 0x1,
	EIRFF_platformType = 0x2,
	EIRFF_model = 0x4,
	EIRFF_os = 0x8,
	EIRFF_time = 0x10,
	EIRFF_timeZone = 0x20,
	EIRFF_timeString = 0x40
} EmInitRespFieldFilter;

typedef enum
{
	EIS_deviceChat = 1,
	EIS_autoSync = 2
} EmInitScence;

typedef enum
{
	EPT_ios = 1,
	EPT_andriod = 2,
	EPT_wp = 3,
	EPT_s60v3 = 4,
	EPT_s60v5 = 5,
	EPT_s40 = 6,
	EPT_bb = 7
} EmPlatformType;

typedef enum
{
	EDDT_manufatureSvr = 0,
	EDDT_wxWristBand = 1,
	EDDT_wxDeviceHtmlChatView = 10001
} EmDeviceDataType;

typedef enum
{
	ESVO_enter = 1,
	ESVO_exit = 2
} EmSwitchViewOp;

typedef enum
{
	EVI_deviceChatView = 1,
	EVI_deviceChatHtmlView = 2
} EmViewId;

typedef enum
{
	ESBO_enterBackground = 1,
	ESBO_enterForground = 2,
	ESBO_sleep = 3
} EmSwitchBackgroundOp;

typedef struct
{
	void *none;
} BaseRequest;

typedef struct
{
	int32_t err_code;
	int has_err_msg;
	CString err_msg;
} BaseResponse;

typedef struct
{
	void *none;
} BasePush;

typedef struct
{
	BaseRequest *base_request;
	int has_md5_device_type_and_device_id;
	Bytes md5_device_type_and_device_id;
	int32_t proto_version;
	int32_t auth_proto;
	EmAuthMethod auth_method;
	int has_aes_sign;
	Bytes aes_sign;
	int has_mac_address;
	Bytes mac_address;
	int has_time_zone;
	String time_zone;
	int has_language;
	String language;
	int has_device_name;
	String device_name;
} AuthRequest;

typedef struct
{
	BaseResponse *base_response;
	CBytes aes_session_key;
} AuthResponse;

typedef struct
{
	BaseRequest *base_request;
	int has_resp_field_filter;
	Bytes resp_field_filter;
	int has_challenge;
	Bytes challenge;
} InitRequest;

typedef struct
{
	BaseResponse *base_response;
	uint32_t user_id_high;
	uint32_t user_id_low;
	int has_challeange_answer;
	uint32_t challeange_answer;
	int has_init_scence;
	EmInitScence init_scence;
	int has_auto_sync_max_duration_second;
	uint32_t auto_sync_max_duration_second;
	int has_user_nick_name;
	CString user_nick_name;
	int has_platform_type;
	EmPlatformType platform_type;
	int has_model;
	CString model;
	int has_os;
	CString os;
	int has_time;
	int32_t time;
	int has_time_zone;
	int32_t time_zone;
	int has_time_string;
	CString time_string;
} InitResponse;

typedef struct
{
	BaseRequest *base_request;
	Bytes data;
	int has_type;
	EmDeviceDataType type;
} SendDataRequest;

typedef struct
{
	BaseResponse *base_response;
	int has_data;
	CBytes data;
} SendDataResponse;

typedef struct
{
	BasePush *base_push;
	CBytes data;
	int has_type;
	EmDeviceDataType type;
} RecvDataPush;

typedef struct
{
	BasePush *base_push;
	EmSwitchViewOp switch_view_op;
	EmViewId view_id;
} SwitchViewPush;

typedef struct
{
	BasePush *base_push;
	EmSwitchBackgroundOp switch_background_op;
} SwitchBackgroudPush;

BaseResponse *epb_unpack_base_response(const uint8_t *buf, int buf_len);
void epb_unpack_base_response_free(BaseResponse *response);
int epb_auth_request_pack_size(AuthRequest *request);
int epb_pack_auth_request(AuthRequest *request, uint8_t *buf, int buf_len);
AuthResponse *epb_unpack_auth_response(const uint8_t *buf, int buf_len);
void epb_unpack_auth_response_free(AuthResponse *response);
int epb_init_request_pack_size(InitRequest *request);
int epb_pack_init_request(InitRequest *request, uint8_t *buf, int buf_len);
InitResponse *epb_unpack_init_response(const uint8_t *buf, int buf_len);
void epb_unpack_init_response_free(InitResponse *response);
int epb_send_data_request_pack_size(SendDataRequest *request);
int epb_pack_send_data_request(SendDataRequest *request, uint8_t *buf, int buf_len);
SendDataResponse *epb_unpack_send_data_response(const uint8_t *buf, int buf_len);
void epb_unpack_send_data_response_free(SendDataResponse *response);
RecvDataPush *epb_unpack_recv_data_push(const uint8_t *buf, int buf_len);
void epb_unpack_recv_data_push_free(RecvDataPush *push);
SwitchViewPush *epb_unpack_switch_view_push(const uint8_t *buf, int buf_len);
void epb_unpack_switch_view_push_free(SwitchViewPush *push);
SwitchBackgroudPush *epb_unpack_switch_backgroud_push(const uint8_t *buf, int buf_len);
void epb_unpack_switch_backgroud_push_free(SwitchBackgroudPush *push);

int wx_pack_auth_request(uint8_t *buf, int len, uint8_t *p_md5, uint8_t *p_aesSign, uint8_t *p_mac);
int wx_pack_init_request(uint8_t *buf, int buf_len, uint8_t filter, uint8_t *p_challenge);
int wx_pack_send_data_request(uint8_t *buf, int buf_len, uint8_t *p_data, int data_len, int type);

uint8_t * wx_unpack_auth_response(const uint8_t *buf, int buf_len);
int wx_unpack_init_response(const uint8_t *buf, int buf_len, uint32_t *p_user_id_high, uint32_t *p_user_id_low, uint32_t crc32, uint32_t *p_scene);
uint8_t * wx_unpack_send_data_response(const uint8_t *buf, int buf_len, int * p_data_len);
uint8_t * wx_unpack_recv_data_push(const uint8_t *buf, int buf_len, int * p_data_len, int *p_data_type);
int wx_unpack_switch_view_push(const uint8_t *buf, int buf_len, int * p_view_op, int *p_view_id);
int wx_unpack_switch_backgroud_push(const uint8_t *buf, int buf_len, int * p_background_op);


int wx_gen_auth_request(uint8_t *pd, uint8_t *device_id, uint8_t * p_md5, uint8_t *p_mac, uint8_t *key);
int wx_gen_init_request(uint8_t *pd, uint8_t filter, uint8_t *p_challenge, uint8_t *key);
int wx_gen_data_request(uint8_t *pd, uint8_t *p_data, int data_len, int html, uint8_t *key);

int wx_pack_request(uint8_t *buf, int data_len, unsigned short nCmd, const unsigned char * key);
int wx_unpack_response(uint8_t *buf, int data_len, unsigned short *p_nCmd, unsigned short *p_seq, unsigned char * key);
int wx_process_auth_response(uint8_t *buf, int data_len, unsigned char * key);
int wx_process_init_response(uint8_t *buf, int data_len, uint8_t * key, uint32_t *p_user_id_high, uint32_t *p_user_id_low, uint32_t crc32, uint32_t *p_scene);
uint8_t * wx_process_send_data_response(uint8_t *buf, int data_len, uint8_t * key, int * p_data_len);
uint8_t *  wx_process_recv_data_push(uint8_t *buf, int data_len, uint8_t * key, int * p_data_len, int *p_data_type);
int wx_process_switch_view_push(uint8_t *buf, int data_len, uint8_t * key, int * p_view_op, int *p_view_id);
int wx_process_switch_backgroud_push(uint8_t *buf, int data_len, uint8_t * key, int * p_background_op);

///////////////////////
//	CRC32 MD5
///////////////////////
uint32_t weixin_crc32(uint32_t crc, const uint8_t *buf, int len);
void	tl_md5 (unsigned char *in, int len, unsigned char * out);
void ts_md5 (char * type, char *id, unsigned char *out);

void wx_set_adv_mac (unsigned char *p_adv, unsigned char * p_mac, int len);

///////////////////////
// aes
///////////////////////
unsigned int aes_cbc_pks7_enc(const unsigned char *pPlainText, unsigned char *pCipherText, 
				 unsigned int nDataLen, const unsigned char *key);

int aes_cbc_pks7_dec(unsigned char *pPlainText, const unsigned char *pCipherText, unsigned int nDataLen, const unsigned char *key);

#if _WIN32
	#ifdef __cplusplus
	}
	#endif
#endif

#endif
