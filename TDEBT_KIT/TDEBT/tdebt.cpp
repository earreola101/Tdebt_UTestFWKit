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

#include "stdafx.h"
#include "hidcmd.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <AtlStr.h>
static void ShowUsage(void);
static int CmdParse(CMD_PAR *pCmdPar, int nCtrl, char *pCmd, int argc, _TCHAR* argv[]);
static int ProcessCmd(CMD_PAR *pCmdPar);
int HStrConvert(BYTE *pBuf, char *pHexStr,int nLen);

//Sets
int Set_LED_COLOR( char *pcmd, int nLen );
int Set_HDCP_Key( char *pcmd, int nLen );
int Set_Tx_CARRIER_ON_OFF( char *pcmd, int nLen );
int Set_Audio_USB_SerialNum( char *pcmd, int nLen );
int Set_Video_USB_SerialNum( char *pcmd, int nLen );
int Set_TestStationN_FAIL( char *pcmd, int nLen );
int Set_TestStationN_PASS( char *pcmd, int nLen );

//Gets
int Get_BATTERY_POWERED( char *pcmd, int nLen );
int Get_BUTTON_STATUS( char *pcmd, int nLen );
int Get_BATTERY_POWERED( char *pcmd, int nLen );
int Get_USB_POWERED( char *pcmd, int nLen );
int Get_ADAPTER_POWERED( char *pcmd, int nLen );
int Get_POWER_MODE( char *pcmd, int nLen );
int Get_BATTERY_TEMP( char *pcmd, int nLen );
int Get_BATTERY_VOLTAGE( char *pcmd, int nLen );
int Get_BATTERY_STATUS( char *pcmd, int nLen );
int Get_HDCP_Key( char *pcmd, int nLen );
int Get_Tx_CARRIER_ON_OFF( char *pcmd, int nLen );
int Get_Audio_USB_SerialNum( char *pcmd, int nLen );
int Get_TestStationN_Flag( char *pcmd, int nLen );
int GetCountryCode();
int BTCmdPSKeyGet(BT_PSKEY_EXT &PSKey);

#define	LOGI_VID										0x046d
#define	AUDIO_PID										0x0881
#define	DFU_PID											0x0881
#define	OPERATION_NOT_IMPLEMENTED                       0x9999
#define SUCCESSF                                         0
#define FAILF                                           -1


//_tmain
int _tmain(int argc, _TCHAR* argv[])
{
	char *pCmd = NULL;
	CMD_PAR stCmdPar;
	memset(&stCmdPar, 0, sizeof(stCmdPar));
	for(int i = 0; i < argc; i++)
	{
		if(argv[i]==NULL || argc==1)
		{
			ShowUsage();
			break;
		}
		if((0 == strcmp(argv[i],"--help"))||(0 == strcmp(argv[i],"-h")))
		{
			ShowUsage();
			return FAILF;
		}
		if((0 == strcmp(argv[i],"--get"))||(0 == strcmp(argv[i],"-g")))
		{
			i++; pCmd = argv[i++];
			CmdParse(&stCmdPar, 0, pCmd, argc - i, &argv[i]);
			break;
		} 
		if((0 == strcmp(argv[i],"--set"))||(0 == strcmp(argv[i],"-s")))
		{
			i++; pCmd = argv[i++]; 
			CmdParse(&stCmdPar, 1, pCmd, argc - i, &argv[i]);
			break;
		}
	}
	ProcessCmd(&stCmdPar);
	return SUCCESSF;
}



//IsGetCmd-this determines if its a get or set
bool IsGetCmd(int nCtrl)
{
	return SUCCESSF == nCtrl;
}


// CmdParse -command parser 
int CmdParse(CMD_PAR *pCmdPar, int nCtrl, char *pCmd, int argc, _TCHAR* argv[])
{	     
	if (NULL == pCmdPar || NULL == pCmd || 0 > argc)
	{
		return FAILF;
	}
	
	if (SUCCESSF == strcmp(pCmd, "fwversion"))
	{
		if(IsGetCmd(nCtrl))
		{
			pCmdPar->nCmd = TDE_CMD_BT_GET_FW_VER;
		} 
	}     
	else if (SUCCESSF == strcmp(pCmd, "hwversion"))
	{
		if(IsGetCmd(nCtrl))
		{
			pCmdPar->nCmd = TDE_CMD_BT_GET_HW_VER;
		} 
	}
	else if (SUCCESSF == strcmp(pCmd, "freq"))
	{
		if(IsGetCmd(nCtrl))
		{
			pCmdPar->nCmd = TDE_CMD_BT_READ_FREQ;
		}
		else
		{
			pCmdPar->nCmd = TDE_CMD_BT_WRITE_FREQ;
			pCmdPar->data.uFreq = atoi(argv[0]);
		} 
	}       
	else if (SUCCESSF == strcmp(pCmd, "macaddress"))
	{
		if(IsGetCmd(nCtrl))
		{
			pCmdPar->nCmd = TDE_CMD_BT_GET_HW_ADDR;
		}
		else 
		{
			pCmdPar->nCmd = TDE_CMD_BT_SET_HW_ADDR;
			strncpy(pCmdPar->data.MacStr, ((char *)argv[0]), sizeof(pCmdPar->data.MacStr));

			char *hexstring = pCmdPar->data.MacStr, *pos = hexstring;
			unsigned char val[12];
			while( *pos )
			{
				if( !((pos-hexstring)&1) )
					sscanf(pos,"%02x",&val[(pos-hexstring)>>1]);
				++pos;
			}
			printf(_T("\nMacAddress="));
			
			for(int i=0; i<=5; i++)
			{
				pCmdPar->data.uHWAddr[i] = val[5-i];
				printf(_T("%x"),val[i]);
			}
		}
	}
	else if (SUCCESSF == strcmp(pCmd, "serialnumber"))
	{
		if(IsGetCmd(nCtrl))
		{
			pCmdPar->nCmd = TDE_CMD_BT_GET_SER_NO;

		}
		else
		{
			pCmdPar->nCmd = TDE_CMD_BT_SET_SER_NO;
		 	strncpy(pCmdPar->data.SerNo, ((char *)argv[0]), sizeof(pCmdPar->data.SerNo));

		} 

	}    
	else if (SUCCESSF == strcmp(pCmd, "devicename"))
	{
		if(IsGetCmd(nCtrl))
		{
			pCmdPar->nCmd = TDE_CMD_BT_GET_DEV_NAME;
		}
		else
		{
			pCmdPar->nCmd = TDE_CMD_BT_SET_DEV_NAME;
			strncpy(pCmdPar->data.DeviceName,(char *)argv[0], sizeof(pCmdPar->data.DeviceName));
		} 
	}
	else if (SUCCESSF == strcmp(pCmd, "pskey"))
	{
		if(IsGetCmd(nCtrl))
		{
			pCmdPar->nCmd = TDE_CMD_BT_GET_PSKEY_EXT;
		}
		else
		{
			pCmdPar->nCmd = TDE_CMD_BT_SET_PSKEY_EXT;
			pCmdPar->data.pskey.uMixerCTrimChan0=  atoi(argv[0]);
			
			for(int i=0;i<5;i++)
			{
			 pCmdPar->data.pskey.uTxMixerCTrimOffset[i]=  atoi(argv[i+1]);
			}
		} 
	}
	else if (SUCCESSF == strcmp(pCmd, "crystal"))
	{
		if(IsGetCmd(nCtrl))
		{
			pCmdPar->nCmd = TDE_CMD_BT_GET_CRYSTAL_TRIM;
		}
		else
		{
			pCmdPar->nCmd = TDE_CMD_BT_SET_CRYSTAL_TRIM;
			pCmdPar->data.CrystalTrim  = atoi(argv[0]);
			
		} 
	}
	else if (SUCCESSF == strcmp(pCmd, "crystal"))
	{
		if(IsGetCmd(nCtrl))
		{
			pCmdPar->nCmd = TDE_CMD_BT_SET_DEV_NAME;
			strncpy(pCmdPar->data.DeviceName, (char *)argv[0], sizeof(pCmdPar->data.DeviceName));
		}
		else
		{
			pCmdPar->nCmd = TDE_CMD_BT_GET_DEV_NAME;
		} 
	}
	else if (SUCCESSF == strcmp(pCmd, "paired_info"))
	{
		if(IsGetCmd(nCtrl))
		{
			pCmdPar->nCmd = TDE_CMD_BT_GET_DEV_NAME;
		}
		else
		{
			pCmdPar->nCmd = TDE_CMD_BT_SET_DEV_NAME;
			strncpy(pCmdPar->data.DeviceName,(char *)argv[0], sizeof(pCmdPar->data.DeviceName));
		} 
	}
	else if (SUCCESSF == strcmp(pCmd, "devicename"))
	{
		if(IsGetCmd(nCtrl))
		{
			pCmdPar->nCmd = TDE_CMD_BT_SET_DEV_NAME;
			strncpy(pCmdPar->data.DeviceName, (char *)argv[0], sizeof(pCmdPar->data.DeviceName));
		}
		else
		{
			pCmdPar->nCmd = TDE_CMD_BT_GET_DEV_NAME;
		} 
	}
	else if (SUCCESSF == strcmp(pCmd, "btclear_paired_dev"))
	{
			pCmdPar->nCmd = TDE_CMD_BT_CLEAR_PAIRED_DEV;
	}
	else if (SUCCESSF == strcmp(pCmd, "btclear_paired_dev"))
	{
		if(IsGetCmd(nCtrl))
		{
			pCmdPar->nCmd = TDE_CMD_BT_SET_DEV_NAME;
			strncpy(pCmdPar->data.DeviceName, (char *)argv[0], sizeof(pCmdPar->data.DeviceName));
		}
		else
		{
			pCmdPar->nCmd = TDE_CMD_BT_GET_DEV_NAME;
		} 
	}
	else if (SUCCESSF == strcmp(pCmd, "btread_sig_level_dev"))
	{
		if(IsGetCmd(nCtrl))
		{
			pCmdPar->nCmd = TDE_CMD_BT_READ_SIG_LEVEL;
		}
		else
		{
			pCmdPar->nCmd = TDE_CMD_BT_READ_SIG_LEVEL;
		} 
	}
	else if (SUCCESSF == strcmp(pCmd, "btusbpidvid"))
	{
 		  pCmdPar->nCmd = TDE_CMD_BT_GET_USB_PID_VID;
	}
	else if (SUCCESSF == strcmp(pCmd, "mfgstr"))
	{
		if(IsGetCmd(nCtrl))
		{
			pCmdPar->nCmd = TDE_CMD_BT_GET_MF_STR;
		}
		else
		{
			pCmdPar->nCmd = TDE_CMD_BT_SET_MF_STR;
			strncpy(pCmdPar->data.MFGSTR,(char *)argv[0], sizeof(pCmdPar->data.MFGSTR));
		} 
	}
	else if (SUCCESSF == strcmp(pCmd, "bt_uartdisable"))
	{
		if(IsGetCmd(nCtrl))
		{
			pCmdPar->nCmd = TDE_CMD_BT_UART_DISABLE;
		}
		else
		{
			pCmdPar->nCmd = TDE_CMD_BT_UART_DISABLE;
		} 
	}
	else if (SUCCESSF == strcmp(pCmd, "audio_dsp"))
	{
		if(IsGetCmd(nCtrl))
		{
			pCmdPar->nCmd = TDE_CMD_SET_AUDIODSP;
		}
		else
		{
			pCmdPar->nCmd = TDE_CMD_SET_AUDIODSP;
			strncpy(pCmdPar->data.DeviceName, (char *)argv[0], sizeof(pCmdPar->data.DeviceName));

			strncpy(pCmdPar->data.AUDIOSET,(char *)argv[0], sizeof(pCmdPar->data.AUDIOSET));
		} 
	}
	else if (SUCCESSF == strcmp(pCmd, "audio_ch"))
		{
			if(IsGetCmd(nCtrl))
			{
				pCmdPar->nCmd = TDE_CMD_SET_CH_AUDIO;
			}
			else
			{
				pCmdPar->nCmd = TDE_CMD_SET_CH_AUDIO;
				strncpy(pCmdPar->data.AUDIOSET,(char *)argv[0], sizeof(pCmdPar->data.AUDIOSET));
			} 
		}
	else if (SUCCESSF == strcmp(pCmd, "bt_eeprom"))
	{
		if(IsGetCmd(nCtrl))
		{
			pCmdPar->nCmd = TDE_CMD_BT_GET_DEV_NAME;
		}
		else
		{
			pCmdPar->nCmd = TDE_CMD_BT_SET_DEV_NAME;
			strncpy(pCmdPar->data.DeviceName,(char *)argv[0], sizeof(pCmdPar->data.DeviceName));
		} 
	}
	else if (SUCCESSF == strcmp(pCmd, "bt_eeprom"))
	{
		if(IsGetCmd(nCtrl))
		{
			pCmdPar->nCmd = TDE_CMD_BT_SET_DEV_NAME;
			strncpy(pCmdPar->data.DeviceName, (char *)argv[0], sizeof(pCmdPar->data.DeviceName));
		}
		else
		{
			pCmdPar->nCmd = TDE_CMD_BT_GET_DEV_NAME;
		} 
	}
	else if (SUCCESSF == strcmp(pCmd, "bt_tx_start"))
	{
		pCmdPar->nCmd = TDE_CMD_BT_RADIO_TX_START;
		int nn=3;
		if(argc==nn)
		{
			for(int i=0;i<nn;i++)
			{
				pCmdPar->data.XxStart[i] = atoi(argv[i]);
			}
		}
	}
	else if (SUCCESSF == strcmp(pCmd, "radiotxdata"))
	{
		if(IsGetCmd(nCtrl))
		{
			pCmdPar->nCmd = TDE_CMD_BT_RADIO_TX_DATA;
		}
		else
		{
			pCmdPar->nCmd = TDE_CMD_BT_RADIO_TX_DATA;
			int nn=2;
			if(argc==nn)
			{
				for(int i=0;i<nn;i++)
				{
					pCmdPar->data.XxStart[i] = atoi(argv[i]);
				}
			}
		} 
	}
	else if (SUCCESSF == strcmp(pCmd, "radiorxstart"))
	{
		pCmdPar->nCmd = TDE_CMD_BT_RADIO_RX_START;
		int nn=3;
		if(argc==nn)
		{
			for(int i=0;i<nn;i++)
			{
				pCmdPar->data.XxStart[i] = atoi(argv[i]);
			}
		}
	}
	else if (SUCCESSF == strcmp(pCmd, "radiorxdata"))
	{
		pCmdPar->nCmd = TDE_CMD_BT_RADIO_RX_DATA;
		int nn=3;
		if(argc==nn)
		{
			for(int i=0;i<nn;i++)
			{
				pCmdPar->data.XxStart[i] = atoi(argv[i]);
			}
		}
	}
	else if (SUCCESSF == strcmp(pCmd, "radiorxdata"))
	{
		if(IsGetCmd(nCtrl))
		{
			pCmdPar->nCmd = TDE_CMD_BT_SET_DEV_NAME;
			strncpy(pCmdPar->data.DeviceName, (char *)argv[0], sizeof(pCmdPar->data.DeviceName));
		}
		else
		{
			pCmdPar->nCmd = TDE_CMD_BT_GET_DEV_NAME;
		} 
	}
	else if (SUCCESSF == strcmp(pCmd, "btsleepmode"))
	{
			pCmdPar->nCmd = TDE_CMD_BT_SET_SLEEP_MODE;
	}
	else if (SUCCESSF == strcmp(pCmd, "bt_nfcstatus"))
	{
		if(IsGetCmd(nCtrl))
		{
			pCmdPar->nCmd = TDE_CMD_BT_GET_NFC_STATUS;
		}
		else
		{
			pCmdPar->nCmd = TDE_CMD_BT_GET_NFC_STATUS;
			strncpy(pCmdPar->data.DeviceName,(char *)argv[0], sizeof(pCmdPar->data.DeviceName));
		} 
	}
	else if (SUCCESSF == strcmp(pCmd, "country"))
	{
		if(IsGetCmd(nCtrl))
		{
			pCmdPar->nCmd = TDE_CMD_BT_GET_COUNTRY_STATUS;
		}
		else
		{
			pCmdPar->nCmd = TDE_CMD_BT_GET_COUNTRY_STATUS;
			strncpy(pCmdPar->data.DeviceName,(char *)argv[0], sizeof(pCmdPar->data.DeviceName));
		} 
	}
	else if (SUCCESSF == strcmp(pCmd, "lcdpattern"))
	{
		if(IsGetCmd(nCtrl))
		{
			pCmdPar->nCmd = TDE_CMD_LCD_SET_PATTERN;
			strncpy(pCmdPar->data.DeviceName, (char *)argv[0], sizeof(pCmdPar->data.DeviceName));
		}
		else
		{
			pCmdPar->nCmd = TDE_CMD_BT_GET_DEV_NAME;
		} 
	}
	else if (SUCCESSF == strcmp(pCmd, "tstStationPass"))
	{
		if(IsGetCmd(nCtrl))
		{
			pCmdPar->nCmd = TDE_CMD_GET_TestStationN_PASS;
		}
		else
		{
			pCmdPar->nCmd = TDE_CMD_SET_TestStationN_PASS;
			strncpy(pCmdPar->data.DeviceName,(char *)argv[0], sizeof(pCmdPar->data.DeviceName));
		} 
	}
	else if (SUCCESSF == strcmp(pCmd, "tstStationFail"))
	{
		if(IsGetCmd(nCtrl))
		{
			pCmdPar->nCmd = TDE_CMD_GET_TestStationN_FAIL;
		}
		else
		{
			pCmdPar->nCmd = TDE_CMD_SET_TestStationN_FAIL;
			strncpy(pCmdPar->data.DeviceName,(char *)argv[0], sizeof(pCmdPar->data.DeviceName));
		} 	 
	}
	else if (SUCCESSF == strcmp(pCmd, "tstStationFlag"))
	{
	    if(IsGetCmd(nCtrl))
		{
			pCmdPar->nCmd = TDE_CMD_Get_TestStationN_Flag;
		}
		else
		{
			pCmdPar->nCmd = TDE_CMD_Set_TestStationN_Flag;
			strncpy(pCmdPar->data.DeviceName,(char *)argv[0], sizeof(pCmdPar->data.DeviceName));
		} 
	}
	else if (SUCCESSF == strcmp(pCmd, "vidUSBSerNum"))
	{
		if(IsGetCmd(nCtrl))
		{
			pCmdPar->nCmd = TDE_CMD_GET_Video_USB_SerialNum;
		}
		else
		{
			pCmdPar->nCmd = TDE_CMD_SET_Video_USB_SerialNum;
			strncpy(pCmdPar->data.DeviceName,(char *)argv[0], sizeof(pCmdPar->data.DeviceName));
		} 
	}    
	else if (SUCCESSF == strcmp(pCmd, "audioUSBserNum"))
	{
		if(IsGetCmd(nCtrl))
		{
			pCmdPar->nCmd = TDE_CMD_GET_Video_USB_SerialNum;
		}
	}  
	else if (SUCCESSF == strcmp(pCmd, "audioUSBserNum"))
	{
	    if(IsGetCmd(nCtrl))
		{
			pCmdPar->nCmd = TDE_CMD_GET_Audio_USB_SerialNum;
		}
		else
		{
			pCmdPar->nCmd = TDE_CMD_SET_Audio_USB_SerialNum;
			strncpy(pCmdPar->data.DeviceName,(char *)argv[0], sizeof(pCmdPar->data.DeviceName));
		} 
	} 
	 else if (SUCCESSF == strcmp(pCmd, "tx_carrier"))
	{
		 if(IsGetCmd(nCtrl))
		{
			pCmdPar->nCmd = TDE_CMD_GET_Audio_USB_SerialNum;
		}
		else
		{
			pCmdPar->nCmd = TDE_CMD_SET_Audio_USB_SerialNum;
			strncpy(pCmdPar->data.DeviceName,(char *)argv[0], sizeof(pCmdPar->data.DeviceName));
		} 
	} 
	else if (SUCCESSF == strcmp(pCmd, "hdcp_key"))
	{
		if(IsGetCmd(nCtrl))
		{
			pCmdPar->nCmd = TDE_CMD_GET_HDCP_Key;
		}
		else
		{
			pCmdPar->nCmd = TDE_CMD_SET_HDCP_Key;
			strncpy(pCmdPar->data.DeviceName,(char *)argv[0], sizeof(pCmdPar->data.DeviceName));
		} 

	}
	else if (SUCCESSF == strcmp(pCmd, "battery_status"))
	{
		if(IsGetCmd(nCtrl))
		{
			pCmdPar->nCmd = TDE_CMD_GET_BATTERY_STATUS;
		}
	} 
	else if (SUCCESSF == strcmp(pCmd, "battery_voltage"))
	{
		if(IsGetCmd(nCtrl))
		{
	       //na
		}
		else
		{
			pCmdPar->nCmd = TDE_CMD_GET_BATTERY_VOLTAGE;
		} 
	} 
	else if (SUCCESSF == strcmp(pCmd, "battery_temp")) //mainer
	{
		if(IsGetCmd(nCtrl))
		{
		 //na
		}
		else
		{
			pCmdPar->nCmd = TDE_CMD_GET_BATTERY_TEMP;
		} 
	}    
   else if (SUCCESSF == strcmp(pCmd, "device_powermode")) //mainer
	{
		if(IsGetCmd(nCtrl))
		{
		 //na
		}
		else
		{
		 	pCmdPar->nCmd = TDE_CMD_GET_POWER_MODE;
		} 
	}    
	else if (SUCCESSF == strcmp(pCmd, "adapter_power_state")) //mainer
	{
		if(IsGetCmd(nCtrl))
		{
		 //na
		}
		else
		{
		 	pCmdPar->nCmd = TDE_CMD_GET_ADAPTER_POWERED;
		} 
	}     
	 else if (SUCCESSF == strcmp(pCmd, "usb_power_mode")) //mainer
	{
		if(IsGetCmd(nCtrl))
		{
		 //na
		}
		else
		{
		 	pCmdPar->nCmd = TDE_CMD_GET_USB_POWERED;
		} 
	}     
	  else if (SUCCESSF == strcmp(pCmd, "battery_powered")) //mainer
	{
		if(IsGetCmd(nCtrl))
		{
		 //na
		}
		else
		{
		 	pCmdPar->nCmd = TDE_CMD_GET_BATTERY_POWERED;
		} 
	}   
	   else if (SUCCESSF == strcmp(pCmd, "btn_status")) //mainer
	{
		if(IsGetCmd(nCtrl))
		{
		 //na
		}
		else
		{
		 pCmdPar->nCmd = TDE_CMD_GET_BUTTON_STATUS;
		} 
	}    
	return SUCCESSF;
}


//gets the firmware version
int BTCmdFwVersionGet(CString & fmversionStr)
{
#define GRBuffSize 28
	int nLen=GRBuffSize;
	BYTE Buf[GRBuffSize];::RtlZeroMemory(Buf,nLen); 
	CHidCmd stHidCmd;
	if(0 > stHidCmd.Open(LOGI_VID, AUDIO_PID)){	return -1;}
	Buf[0] = 0;
	if(0 > stHidCmd.CmdReq(TDE_CMD_BT_GET_FW_VER,Buf,0, 0))
	{
		return FAILF;
	}
	stHidCmd.GetReport( Buf, nLen);

	fmversionStr.Format( _T("%u"), Buf[0]);
	return SUCCESSF;
}


//BTCmdDevNameGet
int BTCmdDevNameGet( char *pName, int nLen )
{
	char Buf[40];
	CHidCmd stHidCmd;
	if(NULL == pName || 0 == nLen)
		return -1;
	if(0 > stHidCmd.Open(LOGI_VID, AUDIO_PID)){	return -1;}
	Buf[0] = 0;
	if(0 > stHidCmd.CmdReq(TDE_CMD_BT_GET_DEV_NAME,Buf, 0, 0))
	{
		return FAILF;
	}
	stHidCmd.GetReport( Buf, sizeof(Buf));

	strncpy(pName, Buf, nLen);
	return SUCCESSF;
}


//BTCmdDevNameSet
int BTCmdDevNameSet( char *pName, int nLen )
{
	char Buf[40];
	CHidCmd stHidCmd;
	if(NULL == pName || 0 == nLen)
		return -1;
	if(0 > stHidCmd.Open(LOGI_VID, AUDIO_PID)){	return -1;}
	Buf[0] = 0;
	memcpy(Buf, pName, nLen+1);
	if(0 > stHidCmd.CmdReq(TDE_CMD_BT_SET_DEV_NAME,  Buf, strlen(Buf)+1, 1))
	{
		return FAILF;
	}

	return SUCCESSF;
}

//BTCmdHwVersionGet
int BTCmdHwVersionGet(DWORD & HwVersion)
{
	char Buf[40];
	CHidCmd stHidCmd;
	if(0 > stHidCmd.Open(LOGI_VID, AUDIO_PID)){	return -1;}
	Buf[0] = 0;
	if(0 > stHidCmd.CmdReq(TDE_CMD_BT_GET_HW_VER,Buf, 0, 0))	{	return -2;	}
	stHidCmd.GetReport( Buf, sizeof(Buf));
	DWORD *pDW=(DWORD *)Buf;
	HwVersion=*pDW;
	return SUCCESSF;
}

//BTCmdFreqGet
int BTCmdFreqGet(DWORD & freq)
{
	char Buf[40];
	CHidCmd stHidCmd;
	if(0 > stHidCmd.Open(LOGI_VID, AUDIO_PID)){	return -1;}
	Buf[0] = 0;
	if(0 > stHidCmd.CmdReq(TDE_CMD_BT_READ_FREQ,Buf, 0, 0))	{	return -2;	}
	stHidCmd.GetReport( Buf, sizeof(Buf));

	DWORD *pDW=(DWORD *)Buf;

	freq=*pDW;
}

//BTCmdFreqSet
int BTCmdFreqSet(DWORD & freq)
{
	char Buf[40];
	CHidCmd stHidCmd;
	if(0 > stHidCmd.Open(LOGI_VID, AUDIO_PID)){	return -1;}
	Buf[0] = 0;
	memcpy(Buf, &freq, 4+1);
	if(0 > stHidCmd.CmdReq(TDE_CMD_BT_WRITE_FREQ,  Buf,6, 1))
	{

		return FAILF;
	}
	return SUCCESSF;
}


//BTCmdMacAddressGet
int BTCmdMacAddressGet(BYTE *macAddress)
{
	int nLen=40;
	char Buf[40];
	CHidCmd stHidCmd;

	if(0 > stHidCmd.Open(LOGI_VID, AUDIO_PID)){	return -1;}

	Buf[0] = 0;
	if(0 > stHidCmd.CmdReq(TDE_CMD_BT_GET_HW_ADDR,Buf,0, 0))
	{
		return FAILF;
	}
	stHidCmd.GetReport( Buf, sizeof(Buf));


	memcpy(macAddress,Buf,nLen+1);
	return SUCCESSF;
}


//BTCmdMacAddressSet
int BTCmdMacAddressSet( BYTE *mac, int nLen )
{
	char Buf[40];
	CHidCmd stHidCmd;
	if(NULL == mac || 0 == nLen)
		return -1;
	if(0 > stHidCmd.Open(LOGI_VID, AUDIO_PID)){	return -1;}
	memcpy(Buf, mac, nLen+1);
	if(0 > stHidCmd.CmdReq(TDE_CMD_BT_SET_HW_ADDR,  Buf,6, 1))
	{
		return FAILF;
	}
	return SUCCESSF;
}

//BTCmdCrystalTrimGet
int BTCmdCrystalTrimGet(WORD CrystalTrim)
{
	char Buf[40];
	CHidCmd stHidCmd;
	if(0 > stHidCmd.Open(LOGI_VID, AUDIO_PID)){	return -1;}
	Buf[0] = 0;
	if(0 > stHidCmd.CmdReq(TDE_CMD_BT_GET_CRYSTAL_TRIM, Buf, 0, 0))	{	return -2;	}
	stHidCmd.GetReport( Buf, sizeof(Buf));

	WORD *pW=(WORD *)Buf;

	CrystalTrim=*pW;
	return SUCCESSF;
}


//BTCmdCrystalTrimSet
int BTCmdCrystalTrimSet(WORD CrystalTrim)
{
	char Buf[40];
	CHidCmd stHidCmd;
 
	if(0 > stHidCmd.Open(LOGI_VID, AUDIO_PID)){	return -1;}
	memcpy(Buf, &CrystalTrim, 2);
	if(0 > stHidCmd.CmdReq(TDE_CMD_BT_SET_CRYSTAL_TRIM,  Buf,2, 0))
	{
		return FAILF;
	}
	return SUCCESSF;
}


//GetCountryCode
int GetCountryCode()
{
	char Buf[40];
	CHidCmd stHidCmd;

	if(0 > stHidCmd.Open(LOGI_VID, AUDIO_PID)){	return -1;}
	Buf[0] = 0;
	if(0 > stHidCmd.CmdReq_Getx(0,Buf, 0, 0)){	return -2;	}
	stHidCmd.GetReport(Buf, sizeof(Buf));

    if(Buf[1]==1)
	{
		printf("logicool");
	}else
	{
		printf("WW");
	}

	return Buf[0];
}


//Get_Paired_Info
int Get_Paired_Info(void)
{
	return OPERATION_NOT_IMPLEMENTED;
}

//Set_Paired_Info
int Set_Paired_Info( char *pName, int nLen )
{
	return SUCCESSF;
}


//BTCmdPSKeyShow
void BTCmdPSKeyShow(CMD_PAR *pCmdPar)
{
	WORD cc=	pCmdPar->data.pskey.uMixerCTrimChan0;
	printf("%d",cc );
	for(int i=0;i<5;i++)
	{
		WORD TT=pCmdPar->data.pskey.uTxMixerCTrimOffset[i];
		printf(" %d",TT);
	}
 
}


//Hex2Dec
int Hex2Dec(std::string s)
{
	std::stringstream ss(s);
	int i;
	ss >> std::hex >> i;
	return i;
}



//BTCmdPSKeySet
int BTCmdPSKeySet(BT_PSKEY_EXT &PSKey)
{
    char Buf[40];
	CHidCmd stHidCmd;
	if(0 > stHidCmd.Open(LOGI_VID, AUDIO_PID)){	return -1;}	
	memcpy(Buf, &PSKey, 12);
	if(0 > stHidCmd.CmdReq(TDE_CMD_BT_SET_PSKEY_EXT, Buf, 12, 1))
	{
		return -2;
	}
	BT_PSKEY_EXT currentValue;
	::ZeroMemory(&currentValue,sizeof(BT_PSKEY_EXT));
	BTCmdPSKeyGet(currentValue);
	if(0 != memcmp(&PSKey,&currentValue,12))
	{
		printf("Setting PS Key failed");
	}
	else
	{
		printf("[OK]\n");
	}
	return SUCCESSF;
}



//BTCmdPSKeyGet
int BTCmdPSKeyGet(BT_PSKEY_EXT &PSKey)
{
  char Buf[40];
  ::ZeroMemory(Buf,40); 
	CHidCmd stHidCmd;
	if(0 > stHidCmd.Open(LOGI_VID, AUDIO_PID)){	return -1;}

	Buf[0] = 0;
	if(0 > stHidCmd.CmdReq(TDE_CMD_BT_GET_PSKEY_EXT,Buf, 0,0))
	{

		return FAILF;
	}
	stHidCmd.GetReport( Buf, sizeof(Buf));

	BT_PSKEY_EXT *pW=(BT_PSKEY_EXT *)Buf;
	PSKey=*pW;
	memcpy(&PSKey,Buf,12);

	return SUCCESSF;
}



//BTCmdClearPairedDevSet
int BTCmdClearPairedDevSet(void)
{
	char Buf[40];
	CHidCmd stHidCmd;
	if(0 > stHidCmd.Open(LOGI_VID, AUDIO_PID)){	return -1;}
	Buf[0] = 0;
	if(0 > stHidCmd.CmdReq(TDE_CMD_BT_CLEAR_PAIRED_DEV,  Buf,0, 0))
	{
		return FAILF;
	}
	return SUCCESSF;
}




//BTCmdSigLevelGet
int BTCmdSigLevelGet(WORD & sign)
{
	char Buf[40];
	CHidCmd stHidCmd;
	if(0 > stHidCmd.Open(LOGI_VID, AUDIO_PID)){	return -1;}

	Buf[0] = 0;
	if(0 > stHidCmd.CmdReq(TDE_CMD_BT_READ_SIG_LEVEL,Buf, 0, 0))
	{
		return FAILF;
	}
	stHidCmd.GetReport( Buf, sizeof(Buf));

	WORD *pW=(WORD *)Buf;

	sign=*pW;
	return SUCCESSF;
}


//BTCmdUSBPidVidGet
int BTCmdUSBPidVidGet(USHORT &vid1, USHORT &pid1)
{
	char Buf[40];
	CHidCmd stHidCmd;
	if(0 > stHidCmd.Open(LOGI_VID, AUDIO_PID)){	return -1;}

	Buf[0] = 0;
	if(0 > stHidCmd.CmdReq(TDE_CMD_BT_GET_USB_PID_VID,Buf, 0, 0))
	{
		return FAILF;
	}
	stHidCmd.GetReport( Buf, sizeof(Buf));

	WORD *pW=(WORD *)Buf;

	WORD vid=*pW;
	WORD pid=*(pW+1);
    vid1=vid;
    pid1=pid;
	return SUCCESSF;
}


//BTCmdSerialNumberGet
int BTCmdSerialNumberGet( char *pSerialNumber, int nLen )
{
	char Buf[40];RtlZeroMemory(Buf,40); 
	CHidCmd stHidCmd;

	if(0 > stHidCmd.Open(LOGI_VID, AUDIO_PID)){	return -1;}
	Buf[0] = 0;
	if(0 > stHidCmd.CmdReq(TDE_CMD_BT_GET_SER_NO,Buf,0, 0))
	{
		return FAILF;
	}
	stHidCmd.GetReport( Buf, sizeof(Buf));

	memcpy(pSerialNumber,Buf,nLen+1);

	return SUCCESSF;
}


//BTCmdSerialNumberSet
int BTCmdSerialNumberSet( char *pSerialNumber, int nLen )
{
	char Buf[40];	
	RtlZeroMemory(Buf,40); 
	CHidCmd stHidCmd;

	if(0 > stHidCmd.Open(LOGI_VID, AUDIO_PID)){	return -1;}

	Buf[0] = 0;
	memcpy(Buf, pSerialNumber, nLen+1);
	if(0 > stHidCmd.CmdReq(TDE_CMD_BT_SET_SER_NO,  Buf,nLen+1, 1))
	{
		return FAILF;
	}

	memcpy(pSerialNumber,Buf,nLen+1);

	return SUCCESSF;
}


//GetMFGStr
int GetMFGStr( char *pName, int nLen )
{
	char Buf[40];
	CHidCmd stHidCmd;
	if(NULL == pName || 0 == nLen)
		return -1;
	if(0 > stHidCmd.Open(LOGI_VID, AUDIO_PID)){	return -1;}
	Buf[0] = 0;
	if(0 > stHidCmd.CmdReq(TDE_CMD_BT_GET_MF_STR,Buf, 0, 0))
	{
		return FAILF;
	}
	stHidCmd.GetReport( Buf, sizeof(Buf));

	strncpy(pName, Buf, nLen);
	return SUCCESSF;
}


//SetMfgStr
int SetMfgStr( char *pName, int nLen )
{
	char Buf[40];
	CHidCmd stHidCmd;
	if(NULL == pName || 0 == nLen)
		return -1;
	if(0 > stHidCmd.Open(LOGI_VID, AUDIO_PID)){	return -1;}
	Buf[0] = 0;
	memcpy(Buf, pName, nLen+1);
	if(0 > stHidCmd.CmdReq(TDE_CMD_BT_SET_MF_STR,  Buf, strlen(Buf)+1, 1))
	{
		return FAILF;
	}
	return SUCCESSF;
}


//BTCmdUartDisableSet
int BTCmdUartDisableSet(void)
{
	char Buf[40];
	CHidCmd stHidCmd;

	if(0 > stHidCmd.Open(LOGI_VID, AUDIO_PID)){	return -1;}
	Buf[0] = 0;

	if(0 > stHidCmd.CmdReq(TDE_CMD_BT_UART_DISABLE,  Buf, 0, 0))
	{

		return FAILF;
	}

	return SUCCESSF;
}


//Get_Read_EEPROM
int Get_Read_EEPROM( char *pName, int nLen )
{
	return SUCCESSF;
}


//Set_BT_EEPROM
int Set_BT_EEPROM( char *pName, int nLen )
{
	return SUCCESSF;
}


//BTCmdRadioTxStart
int BTCmdRadioTxStart( BYTE *pBuf)
{
	char Buf[40];
	CHidCmd stHidCmd;
	if(0 > stHidCmd.Open(LOGI_VID, AUDIO_PID)){	return -1;}
	if(0 > stHidCmd.CmdReq(TDE_CMD_BT_RADIO_TX_START, pBuf,6, 0))
	{
		return FAILF;
	}
	stHidCmd.GetReport( Buf, sizeof(Buf));
	return SUCCESSF;
}


//BTCmdRadioTxDataSet
int BTCmdRadioTxDataSet( BYTE *pBuf)
{
	char Buf[40];
	CHidCmd stHidCmd;
	if(0 > stHidCmd.Open(LOGI_VID, AUDIO_PID)){	return -1;}
	if(0 > stHidCmd.CmdReq(TDE_CMD_BT_RADIO_TX_DATA, pBuf, 6, 0)) 
	{
		return FAILF;
	}
	return SUCCESSF;
}

//BTCmdRadioRxStart
int BTCmdRadioRxStart( BYTE *pBuf)
{
	char Buf[40];
	CHidCmd stHidCmd;
	if(0 > stHidCmd.Open(LOGI_VID, AUDIO_PID)){	return -1;}
	if(0 > stHidCmd.CmdReq(TDE_CMD_BT_RADIO_RX_START, pBuf, 8, 0))
	{
		return FAILF;
	}
	stHidCmd.GetReport( Buf, sizeof(Buf));
	return SUCCESSF;
}


//BTCmdRadioRxDataSet
int BTCmdRadioRxDataSet( BYTE *pBuf)
{
	char Buf[40];
	CHidCmd stHidCmd;
	if(0 > stHidCmd.Open(LOGI_VID, AUDIO_PID)){	return -1;}
	if(0 > stHidCmd.CmdReq(TDE_CMD_BT_RADIO_RX_DATA, pBuf, 6, 0))
	{
		return FAILF;
	}
	stHidCmd.GetReport( Buf, sizeof(Buf));
	return SUCCESSF;
}

//SetBTSleepMode
int SetBTSleepMode(BYTE sm)
{
    char Buf[40];	RtlZeroMemory(Buf,40); 
	CHidCmd stHidCmd;

	if(0 > stHidCmd.Open(LOGI_VID, AUDIO_PID)){	return -1;}
	Buf[0] = sm;
	if(0 > stHidCmd.CmdReq(TDE_CMD_BT_SET_SLEEP_MODE,  Buf,1, 0))
	{
		return FAILF;
	}
	return SUCCESSF;
}

//GetLasterror
int GetLasterror(void)
{
	return SUCCESSF;
}


//Set_BTLoopBack
int Set_BTLoopBack(void)
{
	return SUCCESSF;
}



//SetLCDMode
int SetLCDMode(int mode)
{
	if(mode==0)
	{
		char Buf[40];
		CHidCmd stHidCmd;

		if(0 > stHidCmd.Open(LOGI_VID, AUDIO_PID))
		{
			return SUCCESSF;
		}
		Buf[0] = 0;
		//Buf[1] = 0x06;
		if(0 > stHidCmd.CmdReq(TDE_CMD_BT_RADIO_TX_START,  Buf,2, 0))
		{

			return FAILF;
		}

	}

	return SUCCESSF;
}


//audioChannelControl
int audioChannelControl(int mode)
{
	return OPERATION_NOT_IMPLEMENTED;
}


//HStrConvert
int HStrConvert(BYTE *pBuf, char *pHexStr,int nLen)
{
	return OPERATION_NOT_IMPLEMENTED;
}

//Get_BATTERY_POWERED
int Get_BATTERY_POWERED( char *pcmd, int nLen )
{
	return OPERATION_NOT_IMPLEMENTED;
}


//Set_AudioDsp
int Set_AudioDsp(char pName, int nLen )
{
    //For Audio DSP OFF the HID control spec. is:
	//1. Audio process disable - HID report ID(40), Index(63)
	//Same HID control as Mariner
	//Expected 1 byte value
	//0 : Processing enabled
	//1 : Bypass with Illusonic
	//2 : True bypass

	nLen = 6;
	char Buf[40];
	RtlZeroMemory(Buf,40); 
	CHidCmd stHidCmd;

    if(pName == 48)
	{
        printf("dsp on 0\r\n");  

	   Buf[0] = 0x28;
	   Buf[1] = 0x3f;
	   Buf[2] = 0x00;

	}
	if(pName == 49)
	{
	   printf("dsp off 1\r\n"); 
	  
	   Buf[0] = 0x28;
	   Buf[1] = 0x3f;
	   Buf[2] = 0x01;
	}
    if(pName == 50)
	{
        printf("dsp off 2\r\n");  

	   Buf[0] = 0x28;
	   Buf[1] = 0x3f;
	   Buf[2] = 0x02;

	}


    if(0 > stHidCmd.Open(LOGI_VID, AUDIO_PID))
	{
		printf("command access Failed"); 
		return FAILF;
	}else
	{
		printf("device access OK"); 
	}

	int siz = 47; // sizeof(Buf);
	if (0 > stHidCmd.CmdReqManualSet(TDE_CMD_SET_AUDIODSP, Buf, siz, 0))
	{
		printf("command set Failed"); 
		return FAILF;
	}else
	{
			printf("command sent OK"); 
	}

	return SUCCESSF;
}



//Set_AudioCh
int Set_AudioCh(char pName, int nLen )
{
    //For Audio DSP OFF the HID control spec. is:
	//1. Audio process disable - HID report ID(40), Index(63)
	//Same HID control as Mariner
	//Expected 1 byte value
	//0 : Processing enabled
	//1 : Bypass with Illusonic
	//2 : True bypass

	nLen = 6;
	char Buf[40];
	RtlZeroMemory(Buf,40); 
	CHidCmd stHidCmd;
	Buf[0]= pName;

	if(Buf[0]==0x31)  // channel 1
	{
	   printf("Ch1"); 
	   Buf[0] = 0x28;
	   Buf[1] = 0x62;
	   Buf[2] = 0x01;
	}
	
	if(Buf[0]==0x32) // channel 2
	{
		printf("Ch2");  
		Buf[0] = 0x28;
		Buf[1] = 0x62;
		Buf[2] = 0x02;
	}
	
	if(Buf[0]==0x33) // channel 3
	{
		printf("Ch3");
		Buf[0] = 0x28;
		Buf[1] = 0x62;
		Buf[2] = 0x03;
	}

	if(Buf[0]==0x34) // channel 4
	{
		printf("Ch4");  
		Buf[0] = 0x28;
		Buf[1] = 0x62;
		Buf[2] = 0x04;
	}
	
	if(Buf[0]==0x35) // channel 5
	{
		printf("Ch5");
		Buf[0] = 0x28;
		Buf[1] = 0x62;
		Buf[2] = 0x05;
	}
   if(Buf[0] == '6')  // channel 5
	{
		printf("Ch6");
		Buf[0] = 0x28;
		Buf[1] = 0x62;
		Buf[2] = 0x06;
	}

    if(0 > stHidCmd.Open(LOGI_VID, AUDIO_PID))
	{
		printf("command access Failed"); 
		return -1;
	}else
	{
		printf("device access OK"); 
	}

	if(0 > stHidCmd.CmdReqManualSet(TDE_CMD_SET_AUDIODSP,Buf,sizeof(Buf),0))
	{
		printf("command set Failed"); 
		return -2;
	}else
	{
		printf("command sent OK"); 
	}

	return SUCCESSF;
}


//BTCmdNFCStatusGet
int BTCmdNFCStatusGet()
{
	char Buf[40];
	CHidCmd stHidCmd;

	if(0 > stHidCmd.Open(LOGI_VID, AUDIO_PID)){	return -1;}
	Buf[0] = 0;
	if(0 > stHidCmd.CmdReq(TDE_CMD_BT_GET_NFC_STATUS,Buf, 0, 0)){	return -2;	}
	stHidCmd.GetReport( Buf, sizeof(Buf));

    if(Buf[0]==1)
	{
		printf("1");
	}else
	{
		printf("0");
	}

	return Buf[0];
}

//Set_LED_COLOR
int Set_LED_COLOR( char *pcmd, int nLen )
{
	return OPERATION_NOT_IMPLEMENTED;
}

//Get_BUTTON_STATUS
int Get_BUTTON_STATUS( char *pcmd, int nLen )
{
	return OPERATION_NOT_IMPLEMENTED;
}

//Get_USB_POWERED
int Get_USB_POWERED( char *pcmd, int nLen )
{
	//empty stub
	return OPERATION_NOT_IMPLEMENTED;
}

//Get_ADAPTER_POWERED
int Get_ADAPTER_POWERED( char *pcmd, int nLen )
{
	//empty stub
	return OPERATION_NOT_IMPLEMENTED;
}

//Get_POWER_MODE
int Get_POWER_MODE( char *pcmd, int nLen )
{
	//empty stub
	return OPERATION_NOT_IMPLEMENTED;
}

//Get_BATTERY_TEMP
int Get_BATTERY_TEMP( char *pcmd, int nLen )
{
	//empty stub
	return OPERATION_NOT_IMPLEMENTED;
}

//Get_BATTERY_VOLTAGE
int Get_BATTERY_VOLTAGE( char *pcmd, int nLen )
{
	//empty stub
	return OPERATION_NOT_IMPLEMENTED;
}

//Get_BATTERY_STATUS
int Get_BATTERY_STATUS( char *pcmd, int nLen )
{
	//empty stub
	return OPERATION_NOT_IMPLEMENTED;
}

//Set_HDCP_Key
int Set_HDCP_Key( char *pcmd, int nLen )
{
	//empty stub
	return OPERATION_NOT_IMPLEMENTED;
}

//Get_HDCP_Key
int Get_HDCP_Key( char *pcmd, int nLen )
{
	//empty stub
	return OPERATION_NOT_IMPLEMENTED;
}

//Get_Tx_CARRIER_ON_OFF
int Get_Tx_CARRIER_ON_OFF( char *pcmd, int nLen )
{
	//empty stub
	return OPERATION_NOT_IMPLEMENTED;
}

//Set_Tx_CARRIER_ON_OFF
int Set_Tx_CARRIER_ON_OFF( char *pcmd, int nLen )
{
//empty stub
	return OPERATION_NOT_IMPLEMENTED;
}


//Get_Audio_USB_SerialNum
int Get_Audio_USB_SerialNum( char *pcmd, int nLen )
{
//empty stub
	return OPERATION_NOT_IMPLEMENTED;
} 


//Set_Audio_USB_SerialNum
int Set_Audio_USB_SerialNum( char *pcmd, int nLen )
{
	//empty stub
	return OPERATION_NOT_IMPLEMENTED;
}


//Set_Video_USB_SerialNum
int Set_Video_USB_SerialNum( char *pcmd, int nLen )
{
	//empty stub
	return OPERATION_NOT_IMPLEMENTED;
}

//Get_TestStationN_Flag
int Get_TestStationN_Flag( char *pcmd, int nLen )
{
	//empty stub
	return OPERATION_NOT_IMPLEMENTED;
}

//Set_TestStationN_FAIL
int Set_TestStationN_FAIL( char *pcmd, int nLen )
{
//empty stub
	return OPERATION_NOT_IMPLEMENTED;
}

//Set_TestStationN_PASS
int Set_TestStationN_PASS( char *pcmd, int nLen )
{
    //empty stub
	return OPERATION_NOT_IMPLEMENTED;
}

// command execution
int ProcessCmd(CMD_PAR *pCmdPar)
{
	switch(pCmdPar->nCmd)
	{
	case TDE_CMD_BT_GET_FW_VER:
		{
			CString fmversionStr;
			if( 0 > BTCmdFwVersionGet(fmversionStr)){
				printf("\n[E]:Fail to read firmware version!");
				return FAILF;
			}
			else{
				printf("%s", fmversionStr);
			}
			break;
		}
	case TDE_CMD_BT_GET_HW_VER:
		{
			DWORD HwVersion;
		if( 0 > BTCmdHwVersionGet(HwVersion)){
			printf("\n[E]:Fail to read BT hardware version");
			return FAILF;
		}
		else{
			printf("%u", HwVersion);
		}
		break;
		}
	case TDE_CMD_BT_READ_FREQ:
		{
			DWORD  freq;
		if( 0 > BTCmdFreqGet(freq)){
			printf("\n[E]:Fail to read frequency!");
			return FAILF;
		}
		else{
			printf("%u", freq);
		}
		break;
		}
	case TDE_CMD_BT_WRITE_FREQ:
		{
			DWORD  freq=pCmdPar->data.uFreq;
		if( 0 > BTCmdFreqSet(freq)){
			printf("\n[E]:Fail to set frequency");
			return FAILF;
		}
		else{
			printf("%u", pCmdPar->data.uFreq);
		}
		break;
		}
	case TDE_CMD_BT_GET_HW_ADDR:
	{
		if (0 > BTCmdMacAddressGet(pCmdPar->data.uHWAddr)){
			printf("\n[E]:Fail to read Mac Address");
			return 1;
		}
		else{

			for (int i = 0; i<6; i++)
			{
				printf("%02x", pCmdPar->data.uHWAddr[5 - i]);
			}
		}
		break;
	}
	case TDE_CMD_BT_SET_HW_ADDR:
		{
			if( 0 > BTCmdMacAddressSet(pCmdPar->data.uHWAddr, sizeof(pCmdPar->data.uHWAddr))){
				printf("\n[E]:Fail to set mac address!\r\n");
				return FAILF;
			}
			else{
				printf("[OK]");
			}
			break;
		}
	case TDE_CMD_BT_GET_DEV_NAME:
	{
		if (0 > BTCmdDevNameGet(pCmdPar->data.DeviceName, sizeof(pCmdPar->data.DeviceName))){
			printf("\n[E]:Fail to read Bluetooth device name!\r\n");
			return FAILF;
		}
		else{
			printf("%s", pCmdPar->data.DeviceName);
		}
		break;
	}
	case TDE_CMD_BT_SET_DEV_NAME:
	{
		if (0 > BTCmdDevNameSet(pCmdPar->data.DeviceName, strlen(pCmdPar->data.DeviceName)))
		{
			printf("\n[E]:Fail to write Bluetooth device name!\r\n");
			return FAILF;
		}
		else{
			printf("%s", pCmdPar->data.DeviceName);
		}
		break;
	}
	case TDE_CMD_BT_SET_PSKEY_EXT:
		{
			if( 0 > BTCmdPSKeySet(pCmdPar->data.pskey )){
				printf("\n[E]:Fail to set pskey value!");
				return FAILF;
			}
			else
			{
				BTCmdPSKeyShow(pCmdPar);
			}
			break;
		}
	case TDE_CMD_BT_GET_PSKEY_EXT:
		{
			if( 0 > BTCmdPSKeyGet(pCmdPar->data.pskey )){
				printf("\n[E]:Fail to read PSkey value!");
				return FAILF;
			}
			else
			{
				BTCmdPSKeyShow(pCmdPar);
			}
			break;
		}

	case TDE_CMD_BT_GET_CRYSTAL_TRIM:
		{
			if( 0 > BTCmdCrystalTrimGet(pCmdPar->data.CrystalTrim)){
				printf("\n[E]:Fail to read CrystalTrim value!");
				return FAILF;
			}
			else{
				printf("%u", pCmdPar->data.CrystalTrim);
			}
			break;
		}
    case TDE_CMD_BT_GET_COUNTRY_STATUS:
		{
			GetCountryCode();
					break;
		}
	case TDE_CMD_BT_GET_PAIRED_INFO:
	{
		if (0 > Get_Paired_Info()){
			printf("\n[E]:Fail to read Get_Paired_Info!\r\n");
			return FAILF;
		}
		else{
			printf("%d", pCmdPar->data.uBTHWVer);
		}
		break; 
	}
	case TDE_CMD_BT_SET_PAIRED_INFO:
	{
		if (0 > Set_Paired_Info(pCmdPar->data.DeviceName, sizeof(pCmdPar->data.uFreq))){
			printf("\n[E]:Fail to read Set_Paired_Info!\r\n");
			return FAILF;
		}
		else{
			printf("%d", pCmdPar->data.uBTHWVer);
		}
		break;
	}

	case TDE_CMD_BT_CLEAR_PAIRED_DEV:
	{
		if (0 > BTCmdClearPairedDevSet()){
			printf("\n[E]:Fail to Clear Paired Device!\r\n");
			return FAILF;
		}
		else{
			printf("[OK]");
		}
		break;
	}

	case TDE_CMD_BT_READ_SIG_LEVEL:
		{
			printf(_T("\n[M]:Read BT Signal level, range test by reading RSSI"));	

			if( 0 > BTCmdSigLevelGet(pCmdPar->data.uSigLevel )){
				printf("\n[E]:Fail to Read BT Signal level!");
				return FAILF;

			}
			else{
				printf("\nBtSignalLevel=%d", pCmdPar->data.uSigLevel);
			}
			break;
		}
	case TDE_CMD_BT_GET_USB_PID_VID:
		{
			if( 0 > BTCmdUSBPidVidGet(pCmdPar->data.uUSBVIDPID[0], pCmdPar->data.uUSBVIDPID[1] )){
				printf("\n[E]:Fail to read USB Pid Vid!");
				return FAILF;

			}
			else{
				printf("[M]:VID=0x%x, PID=0x%x", pCmdPar->data.uUSBVIDPID[0], pCmdPar->data.uUSBVIDPID[1]);
			}
			break;
		}
	case TDE_CMD_BT_GET_SER_NO:
	{
		if (0 > BTCmdSerialNumberGet(pCmdPar->data.SerNo, sizeof(pCmdPar->data.SerNo))){
			printf("\n[E]:Fail to read BTCmdSerialNumberGet!\r\n");
			return 1;
		}
		else{
			for (int i = 0; i<16; i++)
			{
				printf("%s", pCmdPar->data.SerNo[i]);
			}
		}
		break;
	}
	case TDE_CMD_BT_SET_SER_NO:
	{
		if (0 > BTCmdSerialNumberSet(pCmdPar->data.SerNo, sizeof(pCmdPar->data.SerNo))){
			printf("\n[E]:Fail to write BTCmdSerialNumberSet!\r\n");
			return 1;
		}
		else{
			for (int i = 0; i<16; i++)
			{
				printf("%s", pCmdPar->data.SerNo[i]);
			}
		}
		break;
	}
	case TDE_CMD_BT_GET_MF_STR:
	{
		if (0 > GetMFGStr(pCmdPar->data.MFGSTR, sizeof(pCmdPar->data.MFGSTR))){
			printf("\n[E]:Fail to read GetMFGStr!\r\n");
			return FAILF;
		}
		else{
			printf("%s", pCmdPar->data.MFGSTR);
		}
		break;
	}
	case TDE_CMD_BT_SET_MF_STR:
	{
		if (0 > SetMfgStr(pCmdPar->data.MFGSTR, sizeof(pCmdPar->data.MFGSTR))){
			printf("\n[E]:Fail to read SetMfgStr!\r\n");
			return FAILF;
		}
		else{
			printf("%s", pCmdPar->data.MFGSTR);
		}
		break;
	}
	case TDE_CMD_BT_UART_DISABLE:
		{
			if( 0 > BTCmdUartDisableSet()){
				printf("\n[E]:Fail to set BTCmdUartDisableSet!");
				return FAILF;
			}
			else{
				printf("[OK]");
			}
			break;
		}
	case TDE_CMD_BT_READ_EEPROM:
	{
		if (0 > Get_Read_EEPROM(pCmdPar->data.DeviceName, sizeof(pCmdPar->data.uFreq))){
			printf("\n[E]:Fail to read Get_Read_EEPROM!\r\n");
			return FAILF;

		}
		else{
			printf("%d", pCmdPar->data.uBTHWVer);
		}
		break;
	}
	case TDE_CMD_BT_WRITE_EEPROM:
	{
		if (0 > Set_BT_EEPROM(pCmdPar->data.DeviceName, sizeof(pCmdPar->data.uFreq))){
			printf("\n[E]:Fail to read Set_BT_EEPROM!\r\n");
			return FAILF;

		}
		else{
			printf("%d", pCmdPar->data.uBTHWVer);

		}
		break;
	}
	case TDE_CMD_BT_RADIO_TX_START:
	{
		if (0 > BTCmdRadioTxStart(pCmdPar->data.Payload)){
			printf("\n[E]:Fail to Radio Tx Start!");
			return FAILF;

		}
		else{
			printf("[OK]");
		}
		break;
	}
	case TDE_CMD_BT_RADIO_TX_DATA:
	{
		if (0 > BTCmdRadioTxDataSet(pCmdPar->data.Payload)){
			printf("[E]:Fail to set Radio Tx Data!");
			return FAILF;

		}
		else{
			printf("[OK]");
		}
		break; 
	}
	case TDE_CMD_BT_RADIO_RX_START:
	{
		if (0 > BTCmdRadioRxStart(pCmdPar->data.Payload)){
			printf("\n[E]:Fail to Radio Rx Start!");
			return FAILF;

		}
		else{
			printf("[OK]");
		}
		break;
	}
	case TDE_CMD_BT_RADIO_RX_DATA:
	{
		if (0 > BTCmdRadioRxDataSet(pCmdPar->data.Payload)){
			printf("[E]:Fail to set Radio Rx Data!");
			return FAILF;

		}
		else{
			printf("[OK]");
		}
		break;
	}
	case TDE_CMD_BT_LOOPBACK:
	{
		if (0 > Set_BTLoopBack()){
			printf("[E]:Fail to read Set_SerialNumber!");
			return FAILF;

		}
		else{
			printf("%d", pCmdPar->data.uBTHWVer);
		}
		break;
	}
	case TDE_CMD_LCD_SET_PATTERN:
	{
		if (0 > SetLCDMode(0)){
			printf("\n[E]:Fail to read TDE_CMD_LCD_SET_PATTERN!");
			return FAILF;

		}
		else{
			printf("[OK]");
		}
		break;
	}
	case TDE_CMD_BT_SET_SLEEP_MODE:
	{
		if (0 > SetBTSleepMode(pCmdPar->data.SleepMode)){
			printf("[E]:Fail to se sleep mode!");
			return 1;
		}
		else{
			printf("%d", pCmdPar->data.SleepMode);
		}
		break;
	}
	case TDE_CMD_BT_GET_NFC_STATUS:
		{
			if( 0 > BTCmdNFCStatusGet()){
				printf("[E]");;
						return 1;

			}
			break;
		}
	case TDE_CMD_BT_GET_LAST_ERR:
		{
			if (0 > GetLasterror()){
				printf("[FAIL]");
				return 1;
			}
			else{
				printf("%d ", pCmdPar->data.uBTHWVer);
			}
			break;
		}
	case TDE_CMD_SET_TestStationN_PASS:
	{
	    int _result = Set_TestStationN_PASS(pCmdPar->data.TestStationN_PASS, sizeof(pCmdPar->data.TestStationN_PASS));
		if(OPERATION_NOT_IMPLEMENTED==_result)
		{
			printf("\n[E]:Operation Not Yet Implemented\r\n%d", OPERATION_NOT_IMPLEMENTED);
			return 1;
		}
		if( 0 > _result){
			printf("\n[E]:Fail to read TDE_CMD_SET_TestStationN_PASS device name!\r\n");
			return 1;
		}
		else{
			printf("%s", pCmdPar->data.TestStationN_PASS);
		}
	    break;
	}
	case TDE_CMD_SET_TestStationN_FAIL:
		{
			int _result = Set_TestStationN_FAIL(pCmdPar->data.TestStationN_FAIL, sizeof(pCmdPar->data.TestStationN_FAIL));
			if(OPERATION_NOT_IMPLEMENTED==_result)
			{
				printf("\n[E]:Operation Not Yet Implemented\r\n%d", OPERATION_NOT_IMPLEMENTED);
				return 1;
			}
			if( 0 >_result ){
			    printf("\n[E]:Fail to read TDE_CMD_SET_TestStationN_PASS device name!\r\n");
				return 1;
			}
			else{
			    printf("%s", pCmdPar->data.TestStationN_FAIL);
			}
			break;
	    }

	case TDE_CMD_Get_TestStationN_Flag:
	{
			int _result = Get_TestStationN_Flag(pCmdPar->data.TestStationN_Flag, sizeof(pCmdPar->data.TestStationN_Flag));
			if(OPERATION_NOT_IMPLEMENTED==_result)
			{
				printf("\n[E]:Operation Not Yet Implemented\r\n%d", OPERATION_NOT_IMPLEMENTED);
				return 1;
			}
			if( 0 > _result){
			    printf("\n[E]:Fail to read TestStationN_Flag device name!\r\n");
				return 1;
			}
			else{
			    sprintf("%s", pCmdPar->data.TestStationN_Flag);
			}
			break;
     }
	case TDE_CMD_SET_Video_USB_SerialNum:
	{
			int _result = Set_Video_USB_SerialNum(pCmdPar->data.Video_USB_SerialNum, sizeof(pCmdPar->data.Video_USB_SerialNum));
			if(OPERATION_NOT_IMPLEMENTED==_result)
			{
				printf("\n[E]:Operation Not Yet Implemented\r\n%d", OPERATION_NOT_IMPLEMENTED);
				return 1;
			}
			if( 0 >_result ){
				printf("\n[E]:Fail to read TDE_CMD_SET_Video_USB_SerialNum device name!\r\n");
				return 1;
			}
			else{
				printf("%s", pCmdPar->data.Video_USB_SerialNum);
			}
			break;
	 }

	case TDE_CMD_SET_Audio_USB_SerialNum:
	{
			int _result =  Set_Audio_USB_SerialNum(pCmdPar->data.Audio_USB_SerialNum, sizeof(pCmdPar->data.Audio_USB_SerialNum));

			if(OPERATION_NOT_IMPLEMENTED==_result)
			{
				printf("\n[E]:Operation Not Yet Implemented\r\n%d", OPERATION_NOT_IMPLEMENTED);
				return 1;
			}
			if( 0 >_result){
			    printf("\n[E]:Fail to read Audio_USB_SerialNum device name!\r\n");
				return 1;
			}
			else{
			    printf("%s", pCmdPar->data.Audio_USB_SerialNum);
			}
			break;
	}

	case TDE_CMD_GET_Audio_USB_SerialNum:
	{
			int _result = Get_Audio_USB_SerialNum(pCmdPar->data.Audio_USB_SerialNum, sizeof(pCmdPar->data.Audio_USB_SerialNum));

			if(OPERATION_NOT_IMPLEMENTED==_result)
			{
				printf("\n[E]:Operation Not Yet Implemented\r\n%d", OPERATION_NOT_IMPLEMENTED);
				return 1;
			}
			if( 0 > _result){
			    printf("\n[E]:Fail to read Audio_USB_SerialNum device name!\r\n");
				return 1;
			}
			else{
			    printf("%s", pCmdPar->data.Audio_USB_SerialNum);
			}
			break;
	}

	case TDE_CMD_SET_Tx_CARRIER_ON_OFF:
	{
			int _result = Set_Tx_CARRIER_ON_OFF(pCmdPar->data.Tx_CARRIER_ON_OFF, sizeof(pCmdPar->data.Tx_CARRIER_ON_OFF));

			if(OPERATION_NOT_IMPLEMENTED==_result)
			{
				printf("\n[E]:Operation Not Yet Implemented\r\n%d", OPERATION_NOT_IMPLEMENTED);
				return 1;
			}
			if( 0 > _result){
			    printf("\n[E]:Fail to read Tx_CARRIER_ON_OFF device name!\r\n");
				return 1;
			}
			else{
			    printf("%s", pCmdPar->data.Tx_CARRIER_ON_OFF);
			}
			break;
	}
	case TDE_CMD_GET_Tx_CARRIER_ON_OFF:
	{
			int _result =Get_Tx_CARRIER_ON_OFF(pCmdPar->data.Tx_CARRIER_ON_OFF, sizeof(pCmdPar->data.Tx_CARRIER_ON_OFF));

			if(OPERATION_NOT_IMPLEMENTED==_result)
			{
				printf("\n[E]:Operation Not Yet Implemented\r\n%d", OPERATION_NOT_IMPLEMENTED);
				return 1;
			}
			if( 0 > _result){
				printf("\n[E]:Fail to read Tx_CARRIER_ON_OFF device name!\r\n");
				return 1;
			}
			else{
				printf("%s", pCmdPar->data.Tx_CARRIER_ON_OFF);
			}
			break;
	}
	case TDE_CMD_GET_HDCP_Key:
	{
			int _result = Get_HDCP_Key(pCmdPar->data.HDCP_Key, sizeof(pCmdPar->data.HDCP_Key));

			if(OPERATION_NOT_IMPLEMENTED==_result)
			{
				printf("\n[E]:Operation Not Yet Implemented\r\n%d", OPERATION_NOT_IMPLEMENTED);
				return 1;
			}
			if( 0 > _result){
				printf("\n[E]:Fail to read HDCP_Key device name!\r\n");
				return 1;
			}
			else{
			    printf("%s", pCmdPar->data.HDCP_Key);
			}
			break;
	}
	case TDE_CMD_SET_HDCP_Key:
	{
			int _result = Set_HDCP_Key(pCmdPar->data.HDCP_Key, sizeof(pCmdPar->data.HDCP_Key));
			if(OPERATION_NOT_IMPLEMENTED==_result)
			{
				printf("\n[E]:Operation Not Yet Implemented\r\n%d", OPERATION_NOT_IMPLEMENTED);
				return 1;
			}
			if( 0 > _result){
			printf("\n[E]:Fail to read HDCP_Key device name!\r\n");
				return 1;
			}
			else{
			printf("%s", pCmdPar->data.HDCP_Key);
			}
			break;
	}
	case TDE_CMD_GET_BATTERY_STATUS:
	{
				int _result = Get_BATTERY_STATUS(pCmdPar->data.BATTERY_STATUS, sizeof(pCmdPar->data.BATTERY_STATUS));

				if(OPERATION_NOT_IMPLEMENTED==_result)
				{
				printf("\n[E]:Operation Not Yet Implemented\r\n%d", OPERATION_NOT_IMPLEMENTED);
				return 1;
				}
				if( 0 > _result){
				printf("\n[E]:Fail to read BATTERY_STATUS device name!\r\n");
				return 1;
				}
				else{
				printf("%s", pCmdPar->data.BATTERY_STATUS);
				}
				break;
	}
    case TDE_CMD_GET_BATTERY_VOLTAGE:
    {
			int _result = Get_BATTERY_VOLTAGE(pCmdPar->data.BATTERY_VOLTAGE, sizeof(pCmdPar->data.BATTERY_VOLTAGE));
			if(OPERATION_NOT_IMPLEMENTED==_result)
			{
				printf("\n[E]:Operation Not Yet Implemented\r\n%d", OPERATION_NOT_IMPLEMENTED);
				return 1;
			}
			if( 0 >_result ){
			    printf("\n[E]:Fail to read BATTERY_VOLTAGE device name!\r\n");
				return 1;
			}
			else{
			    printf("%s", pCmdPar->data.BATTERY_VOLTAGE);
			}
		    break;
     }
    case TDE_CMD_GET_BATTERY_TEMP:
    {
			int _result = Get_BATTERY_TEMP(pCmdPar->data.BATTERY_TEMP, sizeof(pCmdPar->data.BATTERY_TEMP));
		   	if(OPERATION_NOT_IMPLEMENTED==_result)
			{
				printf("\n[E]:Operation Not Yet Implemented\r\n%d", OPERATION_NOT_IMPLEMENTED);
				return 1;
			}
			if( 0 > _result){
				printf("\n[E]:Fail to read BATTERY_TEMP device name!\r\n");
					return 1;
			}
			else{
				printf("%s", pCmdPar->data.BATTERY_TEMP);
			}
			break;
	}
    case TDE_CMD_GET_POWER_MODE:
	{
			int _result =  Get_POWER_MODE(pCmdPar->data.POWER_MODE, sizeof(pCmdPar->data.POWER_MODE));
			if(OPERATION_NOT_IMPLEMENTED==_result)
			{
				printf("\n[E]:Operation Not Yet Implemented\r\n%d", OPERATION_NOT_IMPLEMENTED);
				return 1;
			}
			if( 0 >_result){
				printf("\n[E]:Fail to read POWER_MODE device name!\r\n");
					return 1;
			}
			else{
				printf("%s", pCmdPar->data.POWER_MODE);
			}
			break;
	}
    case TDE_CMD_GET_ADAPTER_POWERED:
	{
			int _result = Get_ADAPTER_POWERED(pCmdPar->data.ADAPTER_POWERED, sizeof(pCmdPar->data.ADAPTER_POWERED));
		   	if(OPERATION_NOT_IMPLEMENTED==_result)
			{
				printf("\n[E]:Operation Not Yet Implemented\r\n%d", OPERATION_NOT_IMPLEMENTED);
				return 1;
			}
			if( 0 > _result){
				printf("\n[E]:Fail to read ADAPTER_POWERED device name!\r\n");
					return 1;
			}
			else{
				printf("%s", pCmdPar->data.ADAPTER_POWERED);
			}
			break;
	}
    case TDE_CMD_GET_USB_POWERED:
    {
			int _result = Get_USB_POWERED(pCmdPar->data.USB_POWERED, sizeof(pCmdPar->data.USB_POWERED));
			if(OPERATION_NOT_IMPLEMENTED==_result)
			{
				printf("\n[E]:Operation Not Yet Implemented\r\n%d", OPERATION_NOT_IMPLEMENTED);
				return 1;
			}
			if( 0 >_result ){
				printf("\n[E]:Fail to read USB_POWERED device name!\r\n");
					return 1;
			}
			else{
				printf("%s", pCmdPar->data.USB_POWERED);
			}
			break;
	}
    case TDE_CMD_GET_BATTERY_POWERED:
    {
			int _result =Get_BATTERY_POWERED(pCmdPar->data.BATTERY_POWERED, sizeof(pCmdPar->data.BATTERY_POWERED)); 
			if(OPERATION_NOT_IMPLEMENTED==_result)
			{
				printf("\n[E]:Operation Not Yet Implemented\r\n%d", OPERATION_NOT_IMPLEMENTED);
				return 1;
			}

			if( 0 > _result){
				printf("\n[E]:Fail to read BATTERY_POWERED device name!\r\n");
					return 1;
			}
			else{
				printf("%s", pCmdPar->data.BATTERY_POWERED);
			}
		break;
    }
    case TDE_CMD_SET_LED_COLOR:
    {
		int _result= Set_LED_COLOR(pCmdPar->data.LED_COLOR, sizeof(pCmdPar->data.LED_COLOR));
		if(OPERATION_NOT_IMPLEMENTED==_result)
		{
			printf("\n[E]:Operation Not Yet Implemented\r\n%d", OPERATION_NOT_IMPLEMENTED);
			return 1;
		}

		if( 0 > _result){
			printf("\n[E]:Fail to read LED_COLOR device name!\r\n");
				return 1;
		}
		else{
			printf("%s", pCmdPar->data.LED_COLOR);
		}
		break;
	}
    case TDE_CMD_GET_BUTTON_STATUS:
	{
		int _result=Get_BUTTON_STATUS(pCmdPar->data.BUTTON_STATUS, sizeof(pCmdPar->data.BUTTON_STATUS));
		if(OPERATION_NOT_IMPLEMENTED==_result)
		{
			printf("\n[E]:Operation Not Yet Implemented\r\n%d", OPERATION_NOT_IMPLEMENTED);
			return 1;
		}

		if( 0 > _result){
			printf("\n[E]:Fail to read GET_BUTTON_STATUS device name!\r\n");
				return 1;
		}
		else{
			printf("%s", pCmdPar->data.BUTTON_STATUS);
		}
		break;
	}
	case TDE_CMD_SET_AUDIODSP:
	{
		int _result= Set_AudioDsp(pCmdPar->data.AUDIOSET[0],1);
		if(OPERATION_NOT_IMPLEMENTED==_result)
		{
			printf("\n[E]:Operation Not Yet Implemented\r\n%d", OPERATION_NOT_IMPLEMENTED);
			return 1;
		}

		if( 0 > _result){
			printf("\n[E]:Fail to read back\r\n");
				return 1;
		}
		else{
			printf("%s", pCmdPar->data.AUDIOSET);
		}
		break;
	}
	case TDE_CMD_SET_CH_AUDIO:
	{
		int _result= Set_AudioCh(pCmdPar->data.AUDIOSET[0],1);
		if(OPERATION_NOT_IMPLEMENTED==_result)
		{
			printf("\n[E]:Operation Not Yet Implemented\r\n%d", OPERATION_NOT_IMPLEMENTED);
			return 1;
		}

		if( 0 > _result){
			printf("\n[E]:Fail to read back\r\n");
				return 1;
		}
		else{
			printf("%s", pCmdPar->data.AUDIOSET);
		}
		break;
	}
	}
	return SUCCESSF;
}



static void ShowUsage(void)
{
	printf("\n<<tstupdater>>: TDE Firmware Access Tool version %d.\n", _TOOVERSION);
	printf("Usage: tdebt [OPTION]...\n");
	printf("  -g --get\t\tread command request\n");
	printf("  -s --set\t\twrite command request\n");
	printf("  -h --help\t\tshow usage of this program\n");
	printf(" \n");
	printf("* \n");
	printf(_T("Progress Character:  [_].still cooking, [X].cmd is implemented but the parameters still need to confirm, [O].TdeBT.exe tool ready, [*].verified, both tool and FW ready.\n"));
	printf("PS: you should match the symbol case for below cmds.\n");
	printf(" \n");
	printf("[X].fwversion             -gets/Sets the firmware version          Usage: tdebt.exe -g fwversion\n");
	printf("[X].hwversion             -gets/Sets the harware version           Usage: tdebt.exe -g hwversion\n");
	printf("[X].freq                  -gets/Sets the radio frequency           Usage: tdebt.exe -g freq\n");
	printf("[X].radiorxstart          -gets/Sets starts radio rx mode          Usage: tdebt.exe -s radiorxstart \n");
	printf("[X].radiorxdata           -gets/Sets radio rx data                 Usage: tdebt.exe -s radiorxdata 1 2 3\n");
	printf("[X].btsleepmode           -gets/Sets the bluetooth sleep mode      Usage: tdebt.exe -g btsleepmode\n");
	printf("[X].crystal               -gets/Sets or gets the crystal           Usage: tdebt.exe -g crystal\n");
	printf("[O].serialnumber          -gets/Sets the serial number             Usage: tdebt.exe -g serialnumber, tdebt.exe -s serialnumber <serial number>\n");
	printf("[X].bt_tx_start           -gets/Sets starts radio tx mode          Usage: tdebt.exe -s bt_tx_start 2402 177462 0\n");


	printf("[O].btusbpidvid           -gets/Sets the usb pid vid               Usage: tdebt.exe -g btusbpidvid\n");
	printf("[O].bt_uartdisable        -gets/Sets the uart lines to low         Usage: tdebt.exe -s bt_uartdisable\n");
	printf("[O].radiotxdata           -gets/Sets radio tx data                 Usage: tdebt.exe -s radiotxdata \n");
	printf("[O].bt_nfcstatus          -gets/Sets the NFC status                Usage: tdebt.exe -g bt_nfcstatus\n");
	printf("[O].btclear_paired_dev    -gets/Sets clear the bluetooth key       Usage: tdebt.exe -s btclear_paired_dev\n");
	printf("[O].btread_sig_level_dev  -gets/Sets reads the signal level(RSSI)  Usage: tdebt.exe -g btread_sig_level_dev\n");
	printf("[O].macaddress            -gets/Sets or gets the macaddress        Usage: tdebt.exe -g macaddress, tdebt.exe -s macaddress\n");
	printf("[O].devicename            -gets/Sets or gets the device name       Usage: tdebt.exe -s devicename SL_BT_TST1 ,tdebt.exe -g devicename\n");
	printf("[O].pskey                 -gets/Sets or gets the pskey             Usage: TdeBt -s pskey uMixerCTrimChan0 uTxMixerCTrimOffset[0] uTxMixerCTrimOffset[1] uTxMixerCTrimOffset[2]  uTxMixerCTrimOffset[3] uTxMixerCTrimOffset[4]");
	printf(" \n");
	printf("[O].audio_dsp              -gets/Sets audio dsp function           Usage: tdebt.exe -s audio_dsp 1\n");
	printf("//Instruction for Audio DSP functions ON C2-2015\n");
	printf(" \n");
	printf("Step1. Select Microphone 1,2,3,4,5,6 \n");
	printf("CMD. Tdebt.exe -s audio_ch 1 \n");
	printf("Step2. To turn off audio dsp \n");
	printf("CMD. Tdebt.exe -s audio_dsp 0 \n");
	printf("or for bypass \n");
	printf("CMD. Tdebt.exe -s audio_dsp 2 \n");
	printf(" \n");
	printf("[O].logo                     -gets/Sets the device logo on screen        Usage: tdebt.exe -s logo logicool\n");

	printf("[_].tstStationPass           -gets/Sets the station pass status    Usage: tdebt.exe -s tstStationPass\n");
	printf("[_].tstStationFail           -gets/Sets the station fail status    Usage: tdebt.exe -s tstStationFail\n");
	printf("[_].tstStationFlag           -gets/Sets the station flag status    Usage: tdebt.exe -g tstStationFlag\n");
	printf("[_].vidUSBSerNum             -gets/Sets the video usb serial       Usage: tdebt.exe -s vidUSBSerNum\n");
	printf("[_].audioUSBserNum           -gets/Sets the video usb serial       Usage: tdebt.exe -s audioUSBserNum\n");
	printf("[_].tx_power_level           -gets/Sets the tx power level         Usage: tdebt.exe -s tx_power_level\n");
	printf("[_].tx_carrier               -gets/Sets the tx carrier             Usage: tdebt.exe -s tx_carrier\n");
	printf("[_].hdcp_key                 -gets/Sets the hdcp key               Usage: tdebt.exe -s hdcp_key\n");
	printf("[_].battery_status           -gets the battery Status              Usage: tdebt.exe -s battery_status\n");
	printf("[_].battery_voltage          -gets the battery voltage             Usage: tdebt.exe -s battery_temp\n");
	printf("[_].battery_temp             -gets the battery temp                Usage: tdebt.exe -s battery_voltage\n");
	printf("[_].device_powermode         -gets reads the device powermode      Usage: tdebt.exe -s device_powermode\n");
	printf("[_].adapter_power_state      -gets the device adapter state        Usage: tdebt.exe -s adapter_power_state\n");
	printf("[_].usb_power_mode           -gets the device usb powermode        Usage: tdebt.exe -s usb_power_mode\n");
	printf("[_].battery_powered          -gets the device battery state        Usage: tdebt.exe -s battery_powered\n");
	printf("[_].led_colors               -gets/Sets the device led colors      Usage: tdebt.exe -s led_colors\n");
	printf("[_].btn_status               -gets/Sets the device btn status      Usage: tdebt.exe -s btn_status\n");



	printf("\nExample to set pskey: tdebt.exe -s pskey 7 bbbc aabb aaaa aaaa aaaa");
	printf("\nExample to get pskey: tdebt.exe -g pskey");

}