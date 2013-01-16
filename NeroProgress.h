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
|* PROGRAM: NeroProgress.h
|*
|* PURPOSE: Wrapper for NERO_PROGRESS structure
******************************************************************************/

#include "stdafx.h"

class CNeroProgress
{
protected:
	NERO_PROGRESS * m_pNeroProgress;

public:
	CNeroProgress ()
	{
		m_pNeroProgress = NeroCreateProgress ();
		_ASSERTE (m_pNeroProgress != NULL);
	}
	~CNeroProgress ()
	{
		NeroFreeMem (m_pNeroProgress);
	}
	NERO_PROGRESS * operator-> ()
	{
		return m_pNeroProgress;
	}
	operator NERO_PROGRESS * ()
	{
		return m_pNeroProgress;
	}
};
