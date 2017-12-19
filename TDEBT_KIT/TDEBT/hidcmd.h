//******************************************************************************************************************************
//  Copyright (c) 2007-2017 Logitech, Inc.  All Rights Reserved
//
//  This program is a trade secret of LOGITECH, and it is not to be reproduced,
//  published, disclosed to others, copied, adapted, distributed or displayed
//  without the prior authorization of LOGITECH.
//
//  Licensee agrees to attach or embbed this notice on all copies of the program,
//  including partial copies or modified versions thereof.
//
//  Description:
//
//  Created by Eduardo Arreola,Lumin Shang
//******************************************************************************************************************************

#pragma once

#include "CHIDUpdate.h"

#define HID_MAX_TRANSFER_NUM		(48+1)



typedef enum eBTCmd
{
	TDE_CMD_NULL 				= 0,
	TDE_CMD_BT_GET_FW_VER 		= 1,
	TDE_CMD_BT_GET_HW_VER 		= 2,
	TDE_CMD_BT_READ_FREQ		= 3,
	TDE_CMD_BT_WRITE_FREQ		= 4,
	TDE_CMD_BT_GET_HW_ADDR		= 5,
	TDE_CMD_BT_SET_HW_ADDR		= 6,
	TDE_CMD_BT_GET_DEV_NAME		= 7, // done
	TDE_CMD_BT_SET_DEV_NAME		= 8, // done
	TDE_CMD_BT_GET_CRYSTAL_TRIM	= 9,
	TDE_CMD_BT_SET_CRYSTAL_TRIM	= 10,
	TDE_CMD_BT_GET_PAIRED_INFO	= 11,
	TDE_CMD_BT_SET_PAIRED_INFO	= 12,
	TDE_CMD_BT_CLEAR_PAIRED_DEV	= 13,
	TDE_CMD_BT_READ_SIG_LEVEL	= 14,
	TDE_CMD_BT_GET_USB_PID_VID	= 15,
	TDE_CMD_BT_GET_SER_NO		= 16,
	TDE_CMD_BT_SET_SER_NO		= 17,
	TDE_CMD_BT_GET_MF_STR		= 18,
	TDE_CMD_BT_SET_MF_STR		= 19,
	TDE_CMD_BT_UART_DISABLE		= 20,
	TDE_CMD_BT_READ_EEPROM		= 21,
	TDE_CMD_BT_WRITE_EEPROM		= 22,
	TDE_CMD_BT_RADIO_TX_START	= 23,
	TDE_CMD_BT_RADIO_TX_DATA	= 24,
	TDE_CMD_BT_RADIO_RX_START	= 25,
	TDE_CMD_BT_RADIO_RX_DATA	= 26,
	TDE_CMD_BT_LOOPBACK			= 27,
	TDE_CMD_BT_SET_SLEEP_MODE	= 28,
	TDE_CMD_BT_GET_NFC_STATUS	= 29,
	TDE_CMD_BT_GET_LAST_ERR		= 30,
	TDE_CMD_BT_GET_PSKEY_EXT	= 31, //
    TDE_CMD_BT_SET_PSKEY_EXT	= 32, // 
	TDE_CMD_SET_TestStationN_PASS   = 33,// for C2
	TDE_CMD_GET_TestStationN_PASS	= 34,// for C2
	TDE_CMD_SET_TestStationN_FAIL	= 35,// for C2
	TDE_CMD_GET_TestStationN_FAIL 	= 36,// for C2
	TDE_CMD_Get_TestStationN_Flag   = 37,// for C2
	TDE_CMD_Set_TestStationN_Flag   = 38,// for C2
	TDE_CMD_SET_Video_USB_SerialNum	= 39,// for C2
	TDE_CMD_GET_Video_USB_SerialNum	= 40,// for C2
	TDE_CMD_SET_Audio_USB_SerialNum	= 41,// for C2
	TDE_CMD_GET_Audio_USB_SerialNum = 42,// for C2
	TDE_CMD_SET_Tx_CARRIER_ON_OFF   = 43,// for C2
	TDE_CMD_GET_HDCP_Key         	= 44,// for C2
	TDE_CMD_SET_HDCP_Key 	        = 45,// for C2
	TDE_CMD_GET_BATTERY_STATUS 	    = 46,// for C2
	TDE_CMD_GET_BATTERY_VOLTAGE	    = 47,// for C2
	TDE_CMD_GET_BATTERY_TEMP 	    = 48,// for C2
	TDE_CMD_GET_POWER_MODE 	        = 49,// for C2
	TDE_CMD_GET_ADAPTER_POWERED 	= 50,// for C2
	TDE_CMD_GET_USB_POWERED	        = 51, // for C2
	TDE_CMD_GET_BATTERY_POWERED	    = 52, // for C2	
    TDE_CMD_SET_LED_COLOR	        = 53, // for C2	
    TDE_CMD_GET_BUTTON_STATUS       = 54, // for C2
	TDE_CMD_GET_Tx_CARRIER_ON_OFF   = 55, // for C2
	TDE_CMD_SET_CH_AUDIO   = 98, // for C2
    TDE_CMD_SET_AUDIODSP   = 63, // for C2
    TDE_CMD_BT_GET_COUNTRY_STATUS   = 10, // for C2
	TDE_CMD_BLE_GET_CENTRAL_ADDR = 120,
	TDE_CMD_BLE_SET_CENTRAL_ADDR = 121,
	TDE_CMD_BLE_GET_PERIPHERAL_ADDR = 122,
	TDE_CMD_BLE_SET_PERIPHERAL_ADDR = 123,

	TDE_CMD_FAIL				    = 0xFF,
	TDE_CMD_MAX
} TDE_CMD; 


typedef struct __bt_pskey_ext__
{
	WORD	uMixerCTrimChan0;
	WORD    uTxMixerCTrimOffset[5]; 
} BT_PSKEY_EXT, *PBT_PSKEY_EXT;



typedef enum 
{
	TDE_CMD_LCD_SET_PATTERN 			=60,
	TDE_CMD_LCD_SET_PATTERN1 
	
} TDE_CMD_LCD; 

#pragma pack(push,1)
typedef struct __eep_rw__ 
{
	unsigned short uAddr;
	BYTE uNum;
	BYTE uDat[32];
}EEP_RW, *PEEP_RW;

typedef struct __radio_rw__ 
{
	unsigned short uFreq;
	unsigned short uLvl;
	unsigned short uAtt;
}RADIO_RW, *PRADIO_RW;


 

typedef struct __CMD_PAR__ 
{
	BYTE nCmd;
	BYTE nFlag;
	union
	{
		EEP_RW stEepRW;
		char SerNo[16];
		char MFGSTR[28];
		char DeviceName[28];
		char MacStr[20];
		UINT uBTVer;
		UINT uBTHWVer;
		UINT uFreq;
		BYTE uHWAddr[6];
		UINT uHWAddiTrim;
		WORD uSigLevel;
		USHORT uUSBVIDPID[2];
		RADIO_RW stRadio; 
		BYTE Payload[40];
		char fmversionStr[28];
		WORD XxStart[4];// for radio test Tx/Rx start
		WORD CrystalTrim;
		BYTE SleepMode;
		BT_PSKEY_EXT pskey;
	    char TestStationN_PASS[28];
		char TestStationN_FAIL[28];
		char TestStationN_Flag[28];
		char Video_USB_SerialNum[28];
		char Audio_USB_SerialNum[28];
		char Tx_CARRIER_ON_OFF[28];
		char HDCP_Key[28];
		char BATTERY_STATUS[28];
		char BATTERY_VOLTAGE[28];
		char BATTERY_TEMP[28];
		char POWER_MODE[28];
		char ADAPTER_POWERED[28];
		char USB_POWERED[28];
		char BATTERY_POWERED[28];
		char LED_COLOR[28];
		char BUTTON_STATUS[28];
		char AUDIOSET[28];
		

	}data;
}CMD_PAR, *PCMDPAR;
#pragma pack(pop)

class CHidCmd
{
public:
	CHidCmd(void);
	~CHidCmd(void);

	int Open( int nVid, int nPid);
	int Close( void );
	int CmdReq(int nCmd, void *pData, int nSize, int nMode );
	int GetReport(void *pData, int nSize );
	int GetReportX(void *pData, int nSize );
	int setTxDataF(int nCmd);
    int CmdReqManualSet(int nCmd, void *pData, int nSize, int nMode );
	int CmdReq_Getx(int nCmd, void *pData, int nSize, int nMode );
private:
	int WaitReport( int nCmd, void *pReport, int nSize );
	int WaitReportX( int nCmd, void *pReport, int nSize );
	CHIDUpdate*   m_pHid;
	HANDLE       m_hDev;
	unsigned char m_ReportBuf[HID_MAX_TRANSFER_NUM];
};

