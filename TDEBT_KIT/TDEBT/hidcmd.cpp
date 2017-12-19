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

#include "StdAfx.h"
#include "hidcmd.h"

/* Report ID */
#define REQ_CMD_REPORT_ID			0x28
#define GET_REPORT_ID				0x29


#define HID_MAX_TRANSFER_EXT		(4)

CHidCmd::CHidCmd(void)
{
	m_hDev = NULL;
	m_pHid = NULL;
}


CHidCmd::~CHidCmd(void)
{
	Close();
}

int CHidCmd::Open( int nVid, int nPid)
{
	m_pHid =  new CHIDUpdate();
	if(NULL == m_pHid)
		return -1;
	m_hDev = m_pHid->OpenHIDDevice(0, nVid, nPid);
	if (NULL == m_hDev) 
	{
		delete m_pHid;
		m_pHid = NULL;
		return -1;
	}
	return 0;
}

int CHidCmd::Close( void )
{
	if(NULL != m_hDev)
	{
		m_pHid->CloseHIDDevice(m_hDev);
		m_hDev = NULL;
	}
	if(NULL != m_pHid)
	{
		delete m_pHid;
		m_pHid = NULL;
	}
	return 0;
}


int CHidCmd::setTxDataF(int nCmd)
{
	unsigned char Buf[HID_MAX_TRANSFER_NUM];
	//unsigned char ReportBuf[HID_MAX_TRANSFER_NUM];
	memset(Buf, 0, HID_MAX_TRANSFER_NUM );
	Buf[0] = REQ_CMD_REPORT_ID;					// ReportID			
	Buf[1] =  0x24;						// CMD
	 
	if(NULL != m_pHid)
	{
		m_pHid->SetReport(Buf, sizeof(Buf), NULL, m_hDev,  L" CHidCmd::CmdReq");
	}
	else
		return -1;
	::RtlZeroMemory(m_ReportBuf, HID_MAX_TRANSFER_NUM);
	if(0 > WaitReport( nCmd, m_ReportBuf, HID_MAX_TRANSFER_NUM))
		return -2;
	return 0;
}



int CHidCmd::CmdReqManualSet(int nCmd, void *pData, int nSize, int nMode )
{
	unsigned char Buf[HID_MAX_TRANSFER_NUM];
	memset(Buf, 0, HID_MAX_TRANSFER_NUM );

	nSize = (nSize < (HID_MAX_TRANSFER_NUM-3)) ? nSize: (HID_MAX_TRANSFER_NUM-3);

	memcpy(&Buf[0],pData, nSize);


	if(NULL != m_pHid)
	{
		m_pHid->SetReport(Buf, sizeof(Buf), NULL, m_hDev, L" CHidCmd::CmdReq");
	}
	else
	{

		return -1;
	}

	::RtlZeroMemory(m_ReportBuf, HID_MAX_TRANSFER_NUM);
	//Temporary workaround until we have fix from FW. Vijay K 09/06/2013.
	/*if(TDE_CMD_BT_SET_PSKEY_EXT != nCmd)
	{
		if(0 > WaitReport( nCmd, m_ReportBuf, HID_MAX_TRANSFER_NUM))
			return -2;
	}*/
	return 0;
}
//printf("\r\n%X [%d] \r\n",Buf[0],Buf[0]);
//       printf("%X [%d] \r\n",Buf[1],Buf[1]);
//	printf("%X [%d] \r\n",Buf[2],Buf[2]);
//printf("0x%X [%d] \r\n",Buf[3],Buf[3]);
//   printf("0x%X [%d] \r\n",Buf[4],Buf[4]);

int CHidCmd::CmdReq_Getx(int nCmd, void *pData, int nSize, int nMode )
{
	
	//I added one HID support to detect is it a logicool version.
 //   set output report (report id:0x1b) with command 0x16
	//then use input report (report id:0x19) to get.Value 1 means it's logicool and 0 mean logitech.
 //   I check in the code in version 8.5.740.

	unsigned char Buf[HID_MAX_TRANSFER_NUM];
	memset(Buf, 0, HID_MAX_TRANSFER_NUM );
	Buf[0]=0x1b;					// ReportID			
	Buf[1]=0x16;						// CMD
	Buf[2]=0x00;
	nSize = (nSize < (HID_MAX_TRANSFER_NUM-3)) ? nSize: (HID_MAX_TRANSFER_NUM-3);
	memcpy(&Buf[3],pData, nSize);
	if(NULL != m_pHid)
	{
		m_pHid->SetReport(Buf, sizeof(Buf), NULL, m_hDev,  L" CHidCmd::CmdReq");
	}
	::RtlZeroMemory(m_ReportBuf, HID_MAX_TRANSFER_NUM);
	return WaitReportX( nCmd, m_ReportBuf, HID_MAX_TRANSFER_NUM);
}



int CHidCmd::CmdReq(int nCmd, void *pData, int nSize, int nMode )
{
	//mark
	unsigned char Buf[HID_MAX_TRANSFER_NUM];
	//unsigned char ReportBuf[HID_MAX_TRANSFER_NUM];
	memset(Buf, 0, HID_MAX_TRANSFER_NUM );
	Buf[0] = REQ_CMD_REPORT_ID;					// ReportID			
	Buf[1] =  nCmd;						// CMD
	Buf[2] = 0x02;

	if(0 == nMode) 
	{
		Buf[2] = 0x00;					// Control Flag
	}
	else
	{
		Buf[2] = 0x01;					// Control Flag
	}
	nSize = (nSize < (HID_MAX_TRANSFER_NUM-3)) ? nSize: (HID_MAX_TRANSFER_NUM-3);
	memcpy(&Buf[3],pData, nSize);
	if(NULL != m_pHid)
	{

	/*	printf("\r\n0x%X [%d] \r\n",Buf[0],Buf[0]);
        printf("0x%X [%d] \r\n",Buf[1],Buf[1]);
		printf("0x%X [%d] \r\n",Buf[2],Buf[2]);
		printf("0x%X [%d] \r\n",Buf[3],Buf[3]);
	    printf("0x%X [%d] \r\n",Buf[4],Buf[4]);*/

		m_pHid->SetReport(Buf, sizeof(Buf), NULL, m_hDev,  L" CHidCmd::CmdReq");
	}
	else
		return -1;
	::RtlZeroMemory(m_ReportBuf, HID_MAX_TRANSFER_NUM);
	//Temporary workaround until we have fix from FW. Vijay K 09/06/2013.
	if(TDE_CMD_BT_SET_PSKEY_EXT != nCmd)
	{
		if(0 > WaitReport( nCmd, m_ReportBuf, HID_MAX_TRANSFER_NUM))
			return -2;
	}
	return 0;
}

int CHidCmd::GetReport(void *pReport, int nSize )
{
	nSize = (nSize < (HID_MAX_TRANSFER_NUM-2)) ? nSize: (HID_MAX_TRANSFER_NUM-2);
	memcpy(pReport, m_ReportBuf, nSize);

	CString str;
	BYTE *pB=(BYTE*)pReport;
	for(int i=0;i<nSize;i++)
	{
    str.AppendFormat(_T("0x%X,"), *(pB+i));
	}
	//printf("\n[W]:Debug dump GetReport raw buffer, size=%d: %s",nSize,  str);
	return 0;
}

int CHidCmd::GetReportX(void *pReport, int nSize )
{
	nSize = (nSize < (HID_MAX_TRANSFER_NUM-2)) ? nSize: (HID_MAX_TRANSFER_NUM-2);
	memcpy(pReport, m_ReportBuf, nSize);

	CString str;
	BYTE *pB=(BYTE*)pReport;
	for(int i=0;i<nSize;i++)
	{
    str.AppendFormat(_T("0x%X,"), *(pB+i));
	}
	//printf("\n[W]:Debug dump GetReport raw buffer, size=%d: %s",nSize,  str);
	return 0;
}

int CHidCmd::WaitReportX( int nCmd, void *pReport, int nSize )
{
	unsigned char Buf[HID_MAX_TRANSFER_NUM];
	int nTryNum = 100;
	int result=0;

	Buf[1] = 0;

	Sleep(400);
	Buf[0] = 0x19;		 // Report ID
	Buf[1] = 0;
 


	m_pHid->GetReport(Buf, sizeof(Buf), NULL, m_hDev,  L"CHidCmd::WaitReport");
	nSize = (nSize < (HID_MAX_TRANSFER_NUM-2)) ? nSize: (HID_MAX_TRANSFER_NUM-2);
	memcpy(pReport, &Buf[0], nSize);
	int ret=Buf[1];
	return ret;
}

int CHidCmd::WaitReport( int nCmd, void *pReport, int nSize )
{
	unsigned char Buf[HID_MAX_TRANSFER_NUM];
	int nTryNum = 100;
	Buf[1] = 0;
	//while(0 == Buf[1])
	{
		Sleep(400);
		Buf[0] = GET_REPORT_ID;		 // Report ID
		Buf[1] = 0;
		m_pHid->GetReport(Buf, sizeof(Buf), NULL, m_hDev,  L"CHidCmd::WaitReport");


		nTryNum--;
		if( 0 == nTryNum)
			return -2;
	}

	if(nCmd != Buf[1])
	{
		printf("\n[E]:the Cmd id return from GetReport is(%d) not the same as we assign(%d)!", Buf[1] ,nCmd);
		return -1;
	}

	nSize = (nSize < (HID_MAX_TRANSFER_NUM-2)) ? nSize: (HID_MAX_TRANSFER_NUM-2);
	memcpy(pReport, &Buf[2], nSize);
	return 0;
}
