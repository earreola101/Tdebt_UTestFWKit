/****************************************************************************
Module:
CHIDUpdate.cpp
Created on Jun 19, 2013

Description:
impletmentation file

Copyright (c) 2003-2020 Logitech, Inc. All Rights Reserved
This program is a trade secret of LOGITECH, and it is not to be reproduced,
published, disclosed to others, copied, adapted, distributed or displayed
without the prior authorization of LOGITECH.

Licensee agrees to attach or embed this notice on all copies of the program,
including partial copies or modified versions thereof.
****************************************************************************/

#include "StdAfx.h"
#include "CHIDUpdate.h"

extern "C" {
	#include <setupapi.h>
	#include "hidsdi.h"
}

/*******************************************************************************
   Function:     GetReport

   Description:  Get data from HID device

   Parameters:
    pData:       The buffer for receive data
    dwDataLen:   The data length of pData
	hDlg:		 The HWND to Dialog
	hDev:		 The Handle to device
	pwcMessage:	 The message to show on status window

   Returns:      None
*******************************************************************************/
void CHIDUpdate::GetReport(BYTE* pData, DWORD dwDataLen, HWND hDlg, HANDLE hDev, WCHAR* pwcMessage)
{
	WCHAR wcErrorCode[MAX_PATH] = {0};
	DWORD ErrCode = 0;

	if (!HidD_GetInputReport(hDev, pData, dwDataLen))
	{
		ErrCode = GetLastError();
		wsprintf((LPSTR)wcErrorCode, (LPSTR)L"CHIDUpdate::SetReport > Error Code: %d %hs", ErrCode, pwcMessage);
		//m_pstatus->SetWindowText(wcErrorCode);
		OutputDebugString((LPSTR)wcErrorCode);
	}		
}

/*******************************************************************************
   Function:     SetReport

   Description:  Send command to HID device

   Parameters:
    pData:       The buffer contains report data
    dwDataLen:   The data length of pData.
	hDlg:		 The HWND to Dialog
	hDev:		 The Handle to device
	pwcMessage:	 The message to show on status window

   Returns:      None
*******************************************************************************/
void CHIDUpdate::SetReport(BYTE* pData, DWORD dwDataLen, HWND hDlg, HANDLE hDev, WCHAR* pwcMessage)
{
	WCHAR wcErrorCode[MAX_PATH] = {0};
	DWORD ErrCode = 0;
			
	if (!HidD_SetOutputReport(hDev, pData, dwDataLen))
	{				
		ErrCode = GetLastError();
		wsprintf((LPSTR)wcErrorCode, (LPSTR)L"CHIDUpdate::SetReport > Error Code: %d %hs", ErrCode, pwcMessage);
		//m_pstatus->SetWindowText(wcErrorCode);
		//OutputDebugString((LPSTR)wcErrorCode);
	}
}



CHIDUpdate::CHIDUpdate(void)
{
}


CHIDUpdate::~CHIDUpdate(void)
{
}



/*******************************************************************************
   Function:     OpenbinFile

   Description:  Open bin file and return file size

   Parameters:
   file:         Full path name
   hDev:		 Handle to the device

   Returns:      The file size of specific bin file if successful, 
				 otherwise return zero if failed.
*******************************************************************************/
DWORD CHIDUpdate::OpenbinFile(LPWSTR file, HANDLE hDev)
{
  DWORD dwSize = 0;
  DWORD dw;

  m_hFile = CreateFile((LPCSTR)file, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
  if (m_hFile != INVALID_HANDLE_VALUE)
  {
	  dwSize = GetFileSize(m_hFile, NULL);
	  m_lpBuffer = (LPBYTE) HeapAlloc(GetProcessHeap(), HEAP_GENERATE_EXCEPTIONS, dwSize + 1);
	  ReadFile(m_hFile, (LPWSTR)m_lpBuffer, dwSize, &dw, NULL);
  }

  //m_pstatus->SetWindowText(L"Can't open service.bin");

  return dwSize;
}

/*******************************************************************************
   Function:     ClosebinFile

   Description:  Close file handle and free bin buffer

   Parameters:	 None

   Returns:      None
*******************************************************************************/
void CHIDUpdate::ClosebinFile()
{
	if (m_hFile)
	{		
		CloseHandle(m_hFile);
		m_hFile = NULL;
	}

	if (m_lpBuffer)
	{
		HeapFree(GetProcessHeap(), 0, m_lpBuffer);
		m_lpBuffer = NULL;
	}
}

void CHIDUpdate::CloseHIDDevice(HANDLE hDev)
{
	if (hDev)
	{
		CloseHandle(hDev);
		hDev = NULL;
	}
}

HANDLE CHIDUpdate::OpenHIDDevice(int overlapped, WORD vid, WORD pid)
{
    HANDLE hidHandle = ((HANDLE)(LONG_PTR)-1);   
    GUID hidGuid;   
    HidD_GetHidGuid(&hidGuid);   
    HDEVINFO hDevInfo = SetupDiGetClassDevs(&hidGuid,   NULL,   NULL,   (DIGCF_PRESENT | DIGCF_DEVICEINTERFACE));   


    if (hDevInfo == INVALID_HANDLE_VALUE)   
    {   
        return INVALID_HANDLE_VALUE;   
    }   
    SP_DEVICE_INTERFACE_DATA devInfoData;   
    devInfoData.cbSize = sizeof (SP_DEVICE_INTERFACE_DATA);   
    int deviceNo = 0;   
    SetLastError(NO_ERROR);   
    while (GetLastError() != ERROR_NO_MORE_ITEMS)   
    {   
        if (SetupDiEnumInterfaceDevice (hDevInfo,   
                                        0,   
                                        &hidGuid,   
                                        deviceNo,   
                                        &devInfoData))   
        {   
            ULONG  requiredLength = 0;   
            SetupDiGetInterfaceDeviceDetail(hDevInfo,   
                                            &devInfoData,   
                                            NULL,   
                                            0,   
                                            &requiredLength,   
                                            NULL);  
            PSP_INTERFACE_DEVICE_DETAIL_DATA devDetail = (SP_INTERFACE_DEVICE_DETAIL_DATA*) malloc (requiredLength);   
            devDetail->cbSize = sizeof(SP_INTERFACE_DEVICE_DETAIL_DATA);   
            if(!SetupDiGetInterfaceDeviceDetail(hDevInfo,   
                                                 &devInfoData,   
                                                 devDetail,   
                                                 requiredLength,   
                                                 NULL,   
                                                 NULL))   
            {   
                free(devDetail);   
                SetupDiDestroyDeviceInfoList(hDevInfo);   
                return INVALID_HANDLE_VALUE;   
            } 

			if (overlapped)   
			{   
				hidHandle = CreateFile(devDetail->DevicePath,   
										GENERIC_READ | GENERIC_WRITE,   
										FILE_SHARE_READ | FILE_SHARE_WRITE,   
										NULL,   
										OPEN_EXISTING,           
										FILE_FLAG_OVERLAPPED,   
										NULL);   
			}   
			else   
			{   
				hidHandle = CreateFile(devDetail->DevicePath,   
										GENERIC_READ | GENERIC_WRITE,   
										FILE_SHARE_READ | FILE_SHARE_WRITE,   
										NULL,   
										OPEN_EXISTING,           
										0,   
										NULL);   
			}          
		
			free(devDetail);

			if (hidHandle != INVALID_HANDLE_VALUE)
			{
				_HIDD_ATTRIBUTES hidAttributes;   
				if( !HidD_GetAttributes(hidHandle, &hidAttributes))   
				{   
					CloseHandle(hidHandle);   
					SetupDiDestroyDeviceInfoList(hDevInfo);   
					return INVALID_HANDLE_VALUE;   
				}   

				if (vid == hidAttributes.VendorID && pid == hidAttributes.ProductID)   
				{ 
					goto EXIT;   
				}   

				CloseHandle(hidHandle);   
				hidHandle = NULL;				   
			}

			++deviceNo;
        }   
    }   

EXIT:
    SetupDiDestroyDeviceInfoList(hDevInfo);   
    return hidHandle;   
}
