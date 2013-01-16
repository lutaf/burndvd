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
|* PROGRAM: FindFile.h
|*
|* PURPOSE: Helper class for ISO tree handling
******************************************************************************/


#ifndef _FIND_FILE_H_
#define _FIND_FILE_H_


#include "stdafx.h"
#include <crtdbg.h>
#include <string>
#include <io.h>
using namespace std;
// This class is a helper for enumerating a directory tree.

class CFindFiles
{
private:
	std::string m_sWildcard;
	
protected:
	_finddata_t m_fdFile;
	_finddata_t m_fdDirectory;
	long m_lHandleFile;
	long m_lHandleDirectory;
	bool m_bValidFile;
	bool m_bValidDirectory;
	
public:

	CFindFiles (LPCSTR psPath);
	~CFindFiles ();

	inline bool IsValidEntry (void) const;
	void FindNext (void);
	bool IsSubDir (void) const;
	LPCSTR GetName (void) const;
	time_t GetCreateTime (void) const;
	LPCSTR GetWildcard (void) const;
};


inline time_t CFindFiles::GetCreateTime (void) const
{
	_ASSERTE (IsValidEntry ());

	return (m_bValidFile)? m_fdFile.time_create: m_fdDirectory.time_create;
}

// Get the name of the file or directory.
// 
inline LPCSTR CFindFiles::GetName (void) const
{
	_ASSERTE (IsValidEntry ());

	return (m_bValidFile)? &m_fdFile.name[0]: &m_fdDirectory.name[0];
}

// Check if entry is valid. This is true if either file or directory
// entry is valid.
// 
inline bool CFindFiles::IsValidEntry (void) const
{
	return m_bValidFile || m_bValidDirectory;
}


// Check if entry is a subdirectory

inline bool CFindFiles::IsSubDir (void) const
{
	_ASSERTE (IsValidEntry ());

	return !m_bValidFile && m_bValidDirectory;
}

inline LPCSTR CFindFiles::GetWildcard (void) const
{
	return m_sWildcard.c_str ();
}

#endif
