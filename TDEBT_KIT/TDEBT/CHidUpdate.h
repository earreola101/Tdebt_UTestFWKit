/****************************************************************************
Module:
CHIDUpdate.cpp
Created on Jun 19, 2013

Description:
header file

Copyright (c) 2003-2020 Logitech, Inc. All Rights Reserved
This program is a trade secret of LOGITECH, and it is not to be reproduced,
published, disclosed to others, copied, adapted, distributed or displayed
without the prior authorization of LOGITECH.

Licensee agrees to attach or embed this notice on all copies of the program,
including partial copies or modified versions thereof.
****************************************************************************/

#pragma once
class CHIDUpdate
{
public:
	CHIDUpdate(void);
	~CHIDUpdate(void);
	void   GetReport(BYTE* pData, DWORD dwDataLen, HWND hDlg, HANDLE hDev, WCHAR* pwcMessage = NULL);
	void   SetReport(BYTE* pData, DWORD dwDataLen, HWND hDlg, HANDLE hDev, WCHAR* pwcMessage = NULL);
	DWORD  OpenbinFile(LPWSTR file, HANDLE hDev);
	void   ClosebinFile();
	HANDLE OpenHIDDevice(int overlapped, WORD vid, WORD pid);
	void   CloseHIDDevice(HANDLE hDev);
	LPBYTE GetBinBuffer() {return m_lpBuffer;}

private:
	LPBYTE m_lpBuffer;
	HANDLE m_hFile;

};

