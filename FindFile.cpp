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
|* PROGRAM: FindFile.cpp
|*
|* PURPOSE: Implementation of helper class for ISO tree handling
******************************************************************************/


#include "stdafx.h"
#include "FindFile.h"
#include <string>
using namespace std;

// Constructor

CFindFiles::CFindFiles (LPCSTR psPath)
{
	// Make sure the path is not NULL.
	// 
	_ASSERTE (psPath != NULL);

	// Store the path in std::string for ease of use.
	// 
	const string sOriginalPath (psPath);
	string sPathBeforeWildcard;

	// Find the last occurrence of backslash...
	//
	string::size_type pos = sOriginalPath.rfind ("\\");
	if (pos == string::npos)
	{
		// Let the wildcard be the whole path as no backslash was found
		// and the path-before-wildcard is a DOT, which means the current
		// directory.
		// 
		m_sWildcard = sOriginalPath;
		sPathBeforeWildcard = ".";
	}
	else
	{
		// If there is a backslash, let the wildcard be everything after
		// the backslash and path-before-wildcard is everything up to the
		// backslash (excluding).
		// 
		m_sWildcard.assign (sOriginalPath.c_str () + pos + 1);
		sPathBeforeWildcard = sOriginalPath;
		sPathBeforeWildcard.erase (pos);
	}

	// Now that we have extracted what we believe is a wildcard, make
	// sure it really is. It must contain at least one of * or ? (asterisk
	// and question mark). Depending on whether it is a wildcard or not
	// we will use a different path for directory search.
	// 
	string sDirectorySearchPath;

	if (m_sWildcard.find ("*") == string::npos &&
		m_sWildcard.find ("?") == string::npos)
	{
		// If asterisk and question mark are not found, then it wasn't a
		// wildcard at all. Let the wildcard be a single asterisk which
		// means ALL FILES.
		// 
		m_sWildcard = "*";

		sDirectorySearchPath = sOriginalPath;
	}
	else
	{
		sDirectorySearchPath = sPathBeforeWildcard + "\\*";
	}

	// Obtain a find handle.
	// 
	m_lHandleFile = _findfirst ((char*)sOriginalPath.c_str (), &m_fdFile);
	
	// The operation succeeded if the returned handle is not -1.
	// 
	m_bValidFile = (-1 != m_lHandleFile);

	// Search for a first non-directory entry.
	// 
	while (m_bValidFile && 0 != (m_fdFile.attrib & _A_SUBDIR))
	{
		m_bValidFile = (0 == _findnext (m_lHandleFile, &m_fdFile));
	}
	
	m_lHandleDirectory = _findfirst ((char*)sDirectorySearchPath.c_str (), &m_fdDirectory);
	m_bValidDirectory = (m_lHandleDirectory != -1);

	// Search for a first non-file entry.
	// 
	while (m_bValidDirectory && 0 == (m_fdDirectory.attrib & _A_SUBDIR))
	{
		m_bValidDirectory = (0 == _findnext (m_lHandleDirectory, &m_fdDirectory));
	}
}

// Destructor

CFindFiles::~CFindFiles ()
{
	// If the handle is valid, make sure it is released.
	// 
	if (m_lHandleFile != -1)
	{
		_findclose (m_lHandleFile);
	}

	if (m_lHandleDirectory != -1)
	{
		_findclose (m_lHandleDirectory);
	}
}

// Find the next entry and set valid flag

void CFindFiles::FindNext (void)
{
	// _findnext returns 0 for success
	
	if (m_bValidFile)
	{
		// We will now seach for a valid file and skip all directories
		// found as we don't need them here. We have a separate directory
		// search.
		// 
		do
		{
			m_bValidFile = (0 == _findnext (m_lHandleFile, &m_fdFile));
		}
		while (m_bValidFile && 0 != (m_fdFile.attrib & _A_SUBDIR));
	}
	else if (m_bValidDirectory)
	{
		// If there are no more files, we search for directories only
		// with a wildcard of *.*
		//
		do
		{
			m_bValidDirectory = (0 == _findnext (m_lHandleDirectory, &m_fdDirectory));
		}
		while (m_bValidDirectory && 0 == (m_fdDirectory.attrib & _A_SUBDIR));
	}

	// If both flags are false on entry to this function, no files or
	// directories can be found as both lists are exausted.
	// 
}

