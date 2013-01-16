/******************************************************************************
|* THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
|* ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
|* THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
|* PARTICULAR PURPOSE.
|* 
|* Copyright 1995-2004 Ahead Software AG. All Rights Reserved.
|*-----------------------------------------------------------------------------
|* NeroSDK / NeroCmd
|*
|* PROGRAM: CtrlHandler.cpp
|*
|* PURPOSE: Handle Ctrl events
******************************************************************************/


#include "stdafx.h"
#include "BurnCore.h"


bool CWriteToDVD::s_bAborted = false;


BOOL WINAPI CWriteToDVD::CtrlHandler (DWORD dwCtrlType)
{
	// Whatever event occurred, we are handling it by aborting
	// any current operation. The operation will be aborted the 
	// next time that NeroAPI calls AbortedCallback
	s_bAborted =true;
	return TRUE;
}
