#include "BurnCore.h"
#include "FindFile.h"
//---------------------------------------------------------------------------------
void StoreFileName (char (& shortFilename) [252], char * psLongFilename, LPCSTR psFilename)
{
	// If filename can fit in the old field, store it there
	// to prevent unnecessary allocation. If not, allocate
	// a sufficient buffer to hold the string.
	// 
	int iLen = strlen (psFilename);

	if (iLen < sizeof (shortFilename))
	{
		strcpy (shortFilename, psFilename);
	}
	else
	{
		psLongFilename = new char [iLen + 1];
		if (psLongFilename != NULL)
		{
			strcpy (psLongFilename, psFilename);
		}
	}
}
inline LPCSTR GetFilename (const NERO_ISO_ITEM * pItem)
{
	return (pItem->longFileName != NULL)? pItem->longFileName: pItem->fileName;
}
//---------------------------------------------------------------------------------
void DeleteIsoItemTree (NERO_ISO_ITEM * pItem)
{
	// First free our own long filename strings, then free the whole tree.
	// 
	FreeOurOwnResources (pItem);
	NeroFreeIsoItemTree (pItem);
}
//---------------------------------------------------------------------------------
void FreeOurOwnResources (NERO_ISO_ITEM * pItem)
{
	// Step through the tree until the 
	// ISO item tree pointer becomes NULL

	while (NULL != pItem)
	{
		NERO_ISO_ITEM* pNextItem = pItem->nextItem;

		if (pItem->isDirectory)
		{
			// We have encountered another ISO item tree;
			// recurse another level.

			FreeOurOwnResources (pItem->subDirFirstItem);
		}

		// It the item is not a reference, free its associated long
		// filenames, if any.
		// 
		if (!pItem->isReference)
		{
			delete pItem->longFileName;
			delete (char *) pItem->longSourceFilePath;
		}

		pItem = pNextItem;
	}
}

//---------------------------------------------------------------------------------
EXITCODE  GetIsoTrack (string s_VolumeName,const CFileList *fList,CNeroIsoTrack** ppIsoTrack, NERO_ISO_ITEM** ppItem)
{
	*ppIsoTrack = NULL;

	// Iterate through the file list and it each one to the tree.
	// If directory is stumbled upon, recurse it and it all of
	// its contents.

	for (CFileList::const_iterator it=fList->begin();
		it!=fList->end();it++)
	{
		NERO_ISO_ITEM* pItem=NULL;
		EXITCODE code;

		// Create a tree from the filename supplied.

		code = CreateIsoTree ((*it)->Recursive, (*it)->szLocalPath.c_str(), &pItem);

		// If there was a problem creating the tree then delete 
		// the whole tree that has been created so far and
		// return the error code.

		if (code != EXITCODE_OK)
		{
			DeleteIsoItemTree (*ppItem);
			*ppItem=NULL;
			return code;
		}

		// Merge the new track with the existing one. The user could
		// specify similar paths/content on the command line. We must make
		// sure the duplicates are weeded out. If return value is false,
		// we need to quit.
		// 
		if (!MergeIsoTrack (ppItem, pItem))
		{
			return EXITCODE_FAILED_TO_CREATE_ISO_TRACK;
		}
	}

	if (NULL != *ppItem)
	{
		
		// This is used only for debugging purposese. Never allow this
		// call to take place in Release code.
		// 
		//		DebugPrintIsoTrack (*ppItem);

		DWORD dwFlags=NCITEF_USE_JOLIET|NCITEF_CREATE_ISO_FS;
		NERO_CITE_ARGS citeArgs;

		memset (&citeArgs, 0, sizeof (citeArgs));

		citeArgs.dwBurnOptions = dwFlags;
		citeArgs.name = s_VolumeName.c_str();
		citeArgs.firstRootItem = *ppItem;
		/*
		citeArgs.abstract = params.GetAbstract ();
		citeArgs.application = params.GetApplication ();
		citeArgs.bibliographic = params.GetBibliographic ();
		citeArgs.copyright = params.GetCopyright ();
		citeArgs.dataPreparer = params.GetDataPreparer ();
		citeArgs.publisher = params.GetPublisher ();
		citeArgs.systemIdentifier = params.GetSystemIdentifier ();
		citeArgs.volumeSet = params.GetVolumeSet ();
		*/
		// Finally, create the ISO track.

		*ppIsoTrack = NeroCreateIsoTrackEx (NULL,
			(const char *) &citeArgs,
			NCITEF_USE_STRUCT);

		// If the ISO track could not be created then delete the 
		// ISO item tree and return with an error

		if (NULL == *ppIsoTrack)
		{
			DeleteIsoItemTree (*ppItem);
			return EXITCODE_FAILED_TO_CREATE_ISO_TRACK;
		}
	}

	return EXITCODE_OK;
}
//---------------------------------------------------------------
EXITCODE  CreateIsoTree (bool Recursive, LPCSTR psFilename, NERO_ISO_ITEM ** ppItem, int iLevel)
{
	// CFindFiles is a helper class for file and subdirectory handling 

	CFindFiles ff (psFilename);

	*ppItem = NULL;

	if (!ff.IsValidEntry())
	{
		if (0 == iLevel)
		{
			// If we haven't found any entries and we are on the
			// first level of recursion then this should be
			// reported as an error.
			return EXITCODE_FILE_NOT_FOUND;
		}
		else
		{
			// If we are on a level other than first, it is ok
			// not to find any entries. This simply means we
			// stumbled upon an empty directory somewhere in a tree.

			return EXITCODE_OK;
		}
	}

	char sPath[MAX_PATH];

	// Make sure that we have no relative path names, but only absolute paths

	if (NULL == _fullpath (sPath, psFilename, sizeof (sPath)))
	{
		// Our path buffer is too small. Bail out!

		return EXITCODE_INTERNAL_ERROR;
	}

	// Find the last blackslash and remove it if found.
	// This will leave us with a root directory.

	LPSTR psBackslash = strrchr (sPath, '\\');
	if (NULL != psBackslash)
	{
		*psBackslash = '\0';
	}

	do
	{
		std::string sNewPath;

		sNewPath = sPath;

		sNewPath += "\\";
		sNewPath += ff.GetName ();

		if (ff.IsSubDir())
		{
			// Here we handle subdirectories

			// strcmp returns 0 on equal strings.
			// Proceed if name contains none of "." or ".."

			if ((0 != strcmp (ff.GetName (), ".")) && (0 != strcmp (ff.GetName (), "..")))
			{
				// Append a wildcard to the path and do a recursive search.

				sNewPath += "\\";
				sNewPath += ff.GetWildcard ();

				NERO_ISO_ITEM * pNewItem = NeroCreateIsoItem ();
				if (NULL == pNewItem)
				{
					DeleteIsoItemTree (*ppItem);
					return EXITCODE_OUT_OF_MEMORY;
				}

				// Attach this item to the beginning of the list.

				if (*ppItem != NULL)
				{
					pNewItem->nextItem = *ppItem;
				}
				*ppItem = pNewItem;

				pNewItem->isDirectory = TRUE;
				time_t t = ff.GetCreateTime ();
				pNewItem->entryTime = *localtime (&t);

				StoreFileName (pNewItem->fileName, pNewItem->longFileName, ff.GetName ());

				// If we the user wants recursive search, only then do we
				// actually recurse at deeper levels otherwise ignore directories
				// alltogether.
				// 
				if (Recursive)
				{
					// Create an ISO item tree at a deeper level

					EXITCODE code = CreateIsoTree (Recursive, sNewPath.c_str (), &pNewItem->subDirFirstItem, iLevel + 1);
					if (EXITCODE_OK != code)
					{
						DeleteIsoItemTree (*ppItem);
						return code;
					}

					// We don't allow empty directories. This should probably
					// be regulated by a command line switch.
					// 
					if (pNewItem->subDirFirstItem == NULL)
					{
						// If the newly added directory is empty, remove it!
						// We first detach it from the list and then
						// deallocate it.
						// 
						*ppItem = pNewItem->nextItem;
						pNewItem->nextItem = NULL;
						DeleteIsoItemTree (pNewItem);
					}
				}
			}
		}
		else
		{
			// Here we handle regular files

			NERO_ISO_ITEM * pNewItem = NeroCreateIsoItem ();
			if (NULL == pNewItem)
			{
				DeleteIsoItemTree (*ppItem);
				return EXITCODE_OUT_OF_MEMORY;
			}

			StoreFileName (pNewItem->sourceFilePath, (char *) pNewItem->longSourceFilePath, sNewPath.c_str ());

			pNewItem->isDirectory = FALSE;
			time_t t = ff.GetCreateTime ();
			pNewItem->entryTime = *localtime (&t);

			StoreFileName (pNewItem->fileName, pNewItem->longFileName, ff.GetName ());

			// Attach this item to the beginning of the list.

			if (*ppItem != NULL)
			{
				pNewItem->nextItem = *ppItem;
			}
			*ppItem = pNewItem;
		}

		ff.FindNext ();
	}
	while (ff.IsValidEntry ());

	return EXITCODE_OK;
}
bool MergeIsoTrack (NERO_ISO_ITEM ** ppItemFirst, NERO_ISO_ITEM * pItemToAdd)
{
	bool bSuccess = true;

	// Two loops. Outter loops the first tree, the inner loops the second
	// tree.
	// 
	for (; bSuccess && *ppItemFirst != NULL; ppItemFirst = &(*ppItemFirst)->nextItem)
	{
		for (NERO_ISO_ITEM ** ppItemSecond = &pItemToAdd; *ppItemSecond != NULL; )
		{
			// Compare entry names...
			// 
			if ((*ppItemFirst)->isDirectory == (*ppItemSecond)->isDirectory &&
				0 == stricmp (GetFilename (*ppItemFirst), GetFilename (*ppItemSecond)))
			{
				bool bReplace = true;

				// The items point to the same file/directory. We need
				// to remove one of them. Which one is removed depends
				// on whether "--backup" is specified on command line.
				// If it is a directory, make sure to recurse anyway
				// because the trees below may not be identical.
				// 
				if ((*ppItemFirst)->isDirectory)
				{
					bSuccess = MergeIsoTrack (&(*ppItemFirst)->subDirFirstItem, (*ppItemSecond)->subDirFirstItem);
					(*ppItemSecond)->subDirFirstItem = NULL;

					if (!bSuccess)
					{
						break;
					}

					// Since the item is a directory, we won't replace
					// the old one as it doesn't matter which one we
					// keep. Directory is just a name. It has no file
					// access time stamp.
					// 
					bReplace = false;
				}
				else
				{
					// If there is a file name conflict between iso items
					// that don't belong to imported sessions, ask the user
					// what to do.
					// 
					if (!(*ppItemFirst)->isReference)
					{


						time_t timeOld = mktime (&(*ppItemFirst)->entryTime);
						time_t timeNew = mktime (&(*ppItemSecond)->entryTime);

						bReplace = timeOld < timeNew;

					}
				}

				if (bReplace)
				{
					// We will now switch places of items in the first and
					// second tree. Since one of the items has to be deleted
					// eventually, this operation will essentially keep
					// the item from the second tree and delete the item
					// from the first tree.
					// 
					NERO_ISO_ITEM * pTempItem = *ppItemSecond;
					*ppItemSecond = *ppItemFirst;
					*ppItemFirst = pTempItem;
					pTempItem = (*ppItemSecond)->nextItem;
					(*ppItemSecond)->nextItem = (*ppItemFirst)->nextItem;
					(*ppItemFirst)->nextItem = pTempItem;
				}

				// Remove the item from the second tree.
				// 
				NERO_ISO_ITEM * pTempItem = *ppItemSecond;
				*ppItemSecond = pTempItem->nextItem;
				pTempItem->nextItem = NULL;
				DeleteIsoItemTree (pTempItem);
			}
			else
			{
				// No match, advance to the next item.
				// 
				ppItemSecond = &(*ppItemSecond)->nextItem;
			}
		}
	}

	// Attach whatever is left of the new tree to the main tree.
	// 
	*ppItemFirst = pItemToAdd;

	// Returning true means, everything is fine, continue.
	// 
	return bSuccess;
}

