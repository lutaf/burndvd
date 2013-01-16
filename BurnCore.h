#include "FileInterface.h"
#include "../../include/NeroAPIGlue.h"
#include "../../include/NeroAPI.h"
#include <stdio.h>
#include "NeroProgress.h"

class CBurnContextProgress : public CNeroProgress
{
public:
	CBurnContextProgress ();
};

class CWriteToDVD:public IWriteCD
{
friend  CBurnContextProgress; 
private:
	NERO_DEVICEHANDLE m_handle;
	FILE*m_log;


	string m_cdtitle;
	int m_cd_type;//0=cd,1=dvd
	bool m_IsEject;
	//bool m_Burning;
	//NERO_PROGRESS npProgress;
	//bool m_Aborted;
protected:
	static bool s_bAborted;
	static NERO_SETTINGS s_NeroSettings;
public:
	CWriteToDVD():m_handle(0),
		m_cdtitle(""),m_cd_type(1){
		};
	~CWriteToDVD();
	//virtual function list
	EXITCODE SetCDParam(CDPARAM *cdParam);
	NERO_CD_INFO * GetCDParam();
	EXITCODE BurnData(const CFileList *listRecData);
	void StopBurn(){CWriteToDVD::s_bAborted = true;};
	EXITCODE InitNero();
	
	void LogError(char *action);
	int  LogError(const char * format, ...);
	//bool IsAbort(){return m_Aborted;}

	static BOOL WINAPI CtrlHandler (DWORD dwCtrlType);
	static BOOL NERO_CALLBACK_ATTR IdleCallback (void *pUserData);
	static NeroUserDlgInOut NERO_CALLBACK_ATTR UserDialog (void * pUserData, NeroUserDlgInOut type, void *data);
	static BOOL NERO_CALLBACK_ATTR ProgressCallback (void *pUserData, DWORD dwProgressInPercent);
	static BOOL NERO_CALLBACK_ATTR AbortedCallback (void *pUserData);
	static void NERO_CALLBACK_ATTR AddLogLine (void *pUserData, NERO_TEXT_TYPE type, const char *text);
	static void NERO_CALLBACK_ATTR SetPhaseCallback (void *pUserData, const char *text);
	static void NERO_CALLBACK_ATTR SetMajorPhaseCallback (void *pUserData, NERO_MAJOR_PHASE phase, void * reserved);
	static void NERO_CALLBACK_ATTR DisableAbortCallback (void *pUserData, BOOL enableAbort);
	static DWORD NERO_CALLBACK_ATTR WriteIOCallback (void *pUserData, BYTE *pBuffer, DWORD dwLen);
	static BOOL NERO_CALLBACK_ATTR EOFCallback (void *pUserData);
	static BOOL NERO_CALLBACK_ATTR ErrorCallback (void *pUserData);
	static DWORD NERO_CALLBACK_ATTR ReadIOCallback (void *pUserData, BYTE *pBuffer, DWORD dwLen);
};
void DeleteIsoItemTree (NERO_ISO_ITEM * pItem);
void FreeOurOwnResources (NERO_ISO_ITEM * pItem);
EXITCODE  CreateIsoTree (bool Recursive, LPCSTR psFilename, NERO_ISO_ITEM ** ppItem, int iLevel=0);
bool MergeIsoTrack (NERO_ISO_ITEM ** ppItemFirst, NERO_ISO_ITEM * pItemToAdd);
EXITCODE  GetIsoTrack (string s_VolumeName,const CFileList *fList,CNeroIsoTrack** ppIsoTrack, NERO_ISO_ITEM** ppItem);