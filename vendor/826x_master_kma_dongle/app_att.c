/*
 * host_att.c
 *
 *  Created on: 2017-4-22
 *      Author: Administrator
 */


#include "../../proj/tl_common.h"


#if (    __PROJECT_8261_MASTER_KMA_DONGLE__ || __PROJECT_8266_MASTER_KMA_DONGLE__ \
	  || __PROJECT_8267_MASTER_KMA_DONGLE__ || __PROJECT_8269_MASTER_KMA_DONGLE__ )



#include "../../proj_lib/ble/ll/ll.h"
#include "../../proj_lib/ble/ble_smp.h"
#include "../../proj_lib/ble/trace.h"
#include "../../proj_lib/ble/service/ble_ll_ota.h"
#include "../../proj_lib/ble/blt_config.h"




const u8 my_MicUUID[16]		= TELINK_MIC_DATA;
const u8 my_SpeakerUUID[16]	= TELINK_SPEAKER_DATA;
const u8 my_OtaUUID[16]		= TELINK_SPP_DATA_OTA;
const u8 my_SppS2CUUID[16]		= TELINK_SPP_DATA_SERVER2CLIENT;
const u8 my_SppC2SUUID[16]		= TELINK_SPP_DATA_CLIENT2SERVER;



u8 read_by_type_req_uuid[16] = {};
u8 read_by_type_req_uuidLen;



void host_att_set_current_readByTypeReq_uuid(u8 *uuid, u8 uuid_len)
{
	read_by_type_req_uuidLen = uuid_len;
	memcpy(read_by_type_req_uuid, uuid, uuid_len);
}







u8	*p_att_response = 0;

volatile u32	host_att_req_busy = 0;

int host_att_client_handler (u16 connHandle, u8 *p)
{
	att_readByTypeRsp_t *p_rsp = (att_readByTypeRsp_t *) p;
	if (p_att_response)
	{
		if ((connHandle & 7) == (host_att_req_busy & 7) && p_rsp->chanId == 0x04 &&
				(p_rsp->opcode == 0x01 || p_rsp->opcode == ((host_att_req_busy >> 16) | 1)))
		{
			memcpy (p_att_response, p, 32);
			host_att_req_busy = 0;
		}
	}
	return 0;
}

void host_att_service_disccovery_clear(void)
{
	p_att_response = 0;
}

typedef int (*host_att_idle_func_t) (void);
host_att_idle_func_t host_att_idle_func = 0;

int host_att_register_idle_func (void *p)
{
	if (host_att_idle_func)
		return 1;

	host_att_idle_func = p;
	return 0;
}

int host_att_response ()
{
	return host_att_req_busy == 0;
}


int host_att_service_wait_event (u16 handle, u8 *p, u32 timeout)
{
	host_att_req_busy = handle | (p[6] << 16);
	p_att_response = p;
	blm_push_fifo (handle, p);

	u32 t = clock_time ();
	while (!clock_time_exceed (t, timeout))
	{
		if (host_att_response ())
		{
			return 0;
		}
		if (host_att_idle_func)
		{
			if (host_att_idle_func ())
			{
				break;
			}
		}
	}
	return 1;
}


ble_sts_t  host_att_discoveryService (u16 handle, att_db_uuid16_t *p16, int n16, att_db_uuid128_t *p128, int n128)
{
	att_db_uuid16_t *ps16 = p16;
	att_db_uuid128_t *ps128 = p128;
	int i16 = 0;
	int i128 = 0;

	ps16->num = 0;
	ps128->num = 0;

	// char discovery: att_read_by_type
		// hid discovery: att_find_info
	u8  dat[32];
	u16 s = 1;
	u16 uuid = GATT_UUID_CHARACTER;
	do {

		att_req_read_by_type (dat, s, 0xffff, &uuid, 2);
		if (host_att_service_wait_event(handle, dat, 1000000))
		{
			return  SERVICE_DISCOVERY_TIEMOUT;			//timeout
		}

		// process response data
		att_readByTypeRsp_t *p_rsp = (att_readByTypeRsp_t *) dat;
		if (p_rsp->opcode != ATT_OP_READ_BY_TYPE_RSP)
		{
			break;
		}

		if (p_rsp->datalen == 21)		//uuid128
		{
			s = p_rsp->data[3] + p_rsp->data[4] * 256;
			if (i128 < n128)
			{
				p128->property = p_rsp->data[2];
				p128->handle = s;
				memcpy (p128->uuid, p_rsp->data + 5, 16);
				i128++;
				p128++;
			}
		}
		else if (p_rsp->datalen == 7) //uuid16
		{
			u8 *pd = p_rsp->data;
			while (p_rsp->l2capLen > 7)
			{
				s = pd[3] + pd[4] * 256;
				if (i16 < n16)
				{
					p16->property = pd[2];
					p16->handle = s;
					p16->uuid = pd[5] | (pd[6] << 8);
					p16->ref = 0;
					i16 ++;
					p16++;
				}
				p_rsp->l2capLen -= 7;
				pd += 7;
			}
		}
	} while (1);

	ps16->num = i16;
	ps128->num = i128;

	//--------- use att_find_info to find the reference property for hid ----------
	p16 = ps16;
	for (int i=0; i<i16; i++)
	{
		if (p16->uuid == CHARACTERISTIC_UUID_HID_REPORT)		//find reference
		{

			att_req_find_info (dat, p16->handle, 0xffff);
			if (host_att_service_wait_event(handle, dat, 1000000))
			{
				return  SERVICE_DISCOVERY_TIEMOUT;			//timeout
			}

			att_findInfoRsp_t *p_rsp = (att_findInfoRsp_t *) dat;
			if (p_rsp->opcode == ATT_OP_FIND_INFO_RSP && p_rsp->format == 1)
			{
				int n = p_rsp->l2capLen - 2;
				u8 *pd = p_rsp->data;
				while (n > 0)
				{
					if ((pd[2]==U16_LO(GATT_UUID_CHARACTER) && pd[3]==U16_HI(GATT_UUID_CHARACTER)) ||
						(pd[2]==U16_LO(GATT_UUID_PRIMARY_SERVICE) && pd[3]==U16_HI(GATT_UUID_PRIMARY_SERVICE))	)
					{
						break;
					}

					if (pd[2]==U16_LO(GATT_UUID_REPORT_REF) && pd[3]==U16_HI(GATT_UUID_REPORT_REF))
					{
					//-----------		read attribute ----------------

						att_req_read (dat, pd[0]);
						if (host_att_service_wait_event(handle, dat, 1000000))
						{
								return  SERVICE_DISCOVERY_TIEMOUT;			//timeout
						}

						att_readRsp_t *pr = (att_readRsp_t *) dat;
						if (pr->opcode == ATT_OP_READ_RSP)
						{
							p16->ref = pr->value[0] | (pr->value[1] << 8);
						}

						break;
					}
					n -= 4;
					pd += 4;
				}
			}
		} //----- end for if CHARACTERISTIC_UUID_HID_REPORT

		p16++;
	}

	return  BLE_SUCCESS;
}




#endif
