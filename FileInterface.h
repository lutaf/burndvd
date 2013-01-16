#ifndef _FILE_INTERFACE_H_
#define _FILE_INTERFACE_H_
#include <list>
#include <string>
#include <windows.h>
#include "../../include/NeroAPIGlue.h"

using namespace std;
//end flag

typedef enum EXITCODE {
	EXITCODE_OK = 0,
	EXITCODE_BAD_USAGE,
	EXITCODE_INTERNAL_ERROR,
	EXITCODE_NEROAPI_DLL_NOT_FOUND,
	EXITCODE_NO_SERIAL_NUMBER,
	EXITCODE_BAD_SERIAL_NUMBER,
	EXITCODE_NO_CD_INSERTED,
	EXITCODE_NO_SUPPORT_FOR_CDRW,
	EXITCODE_ERROR_ERASING_CDRW,
	EXITCODE_ERROR_OBTAINING_AVAILABLE_DRIVES,
	EXITCODE_MISSING_DRIVENAME,
	EXITCODE_ERROR_OPENNING_DRIVE,
	EXITCODE_DRIVE_NOT_FOUND,
	EXITCODE_UNKNOWN_CD_FORMAT,
	EXITCODE_INVALID_DRIVE,
	EXITCODE_BURN_FAILED,
	EXITCODE_FUNCTION_NOT_ALLOWED,
	EXITCODE_DRIVE_NOT_ALLOWED,
	EXITCODE_ERROR_GETTING_CD_INFO,
	EXITCODE_TRACK_NOT_FOUND,
	EXITCODE_UNKNOWN_FILE_TYPE,
	EXITCODE_DAE_FAILED,
	EXITCODE_ERROR_OPENNING_FILE,
	EXITCODE_OUT_OF_MEMORY,
	EXITCODE_ERROR_DETERMINING_LENGTH_OF_FILE,
	EXITCODE_EJECT_FAILED,
	EXITCODE_BAD_IMPORT_SESSION_NUMBER,
	EXITCODE_FAILED_TO_CREATE_ISO_TRACK,
	EXITCODE_FILE_NOT_FOUND,
	EXITCODE_UNKNOWN,
	EXITCODE_USER_ABORTED,
	EXITCODE_DEMOVERSION_EXPIRED,
	EXITCODE_BAD_MESSAGE_FILE,
	EXITCODE_ERROR_DOING_ESTIMATION,
	EXITCODE_ERROR_GETTING_VMS_INFO,
	EXITCODE_ERROR_OPEN_LOGFILE
};
typedef struct tagCDPARAM
{
	char szDriver;//刻录机所在盘符			
	char szCDType;////刻录机类型，0：CD， 1：DVD		
	char szBurnMethod;//no use
	bool IsEject;			
	char szTitle[200];
}CDPARAM;
/*
typedef struct tagCDINFO
{
	DWORD            	ncdiFreeCapacityInBlocks;
	BOOL             	ncdiIsWriteable;
	char             		ncdiArtist[65];
	char             		ncdiTitle[65];
	DWORD			 	ncdiAvailableEraseModes;
	DWORD			 	ncdiUnusedBlocks;	
	NERO_MEDIA_TYPE	ncdiMediaType;
	DWORD			 	ncdiMediumFlags;
	DWORD          	ncdiLayer0MaxBlocks;
	DWORD          	ncdiTotalCapacity;	
	DWORD            	ncdiReserved[26];
	DWORD            	ncdiNumTracks;
	NERO_TRACK_INFO  ncdiTrackInfos[1];
} CDINFO;
*/
typedef struct tagCNode
{
 string szLocalPath;
 bool Recursive;

}CNode;
typedef list<CNode*> CFileList;

//----------------------------------------------
class IWriteCD
{
public:
 virtual EXITCODE SetCDParam(CDPARAM *cdParam)=0;
 virtual NERO_CD_INFO * GetCDParam()=0;//return 0 when failed
 virtual EXITCODE BurnData(const CFileList *listRecData)=0;
 virtual void StopBurn()=0;
 virtual EXITCODE InitNero()=0;
};



#endif