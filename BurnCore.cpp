#include "BurnCore.h"


/* for the settings */


//----------------------------------------------------------
EXITCODE  CWriteToDVD::SetCDParam(CDPARAM *cdParam){

	if(!cdParam||cdParam->szBurnMethod!=0)
		return EXITCODE_BAD_USAGE;
	//get
	this->m_cdtitle=cdParam->szTitle;
	this->m_cd_type=cdParam->szCDType;
	this->m_IsEject=cdParam->IsEject;

	//check dvd-w
	NERO_SCSI_DEVICE_INFOS *NeroDeviceInfos=0;

	DWORD  m_set= MEDIA_NONE;//|MEDIA_CD|MEDIA_DVD_ANY;
	__try{
		NeroDeviceInfos = NeroGetAvailableDrivesEx ((NERO_MEDIA_TYPE)m_set,NULL);
		if (!NeroDeviceInfos) {
			LogError("NeroGetAvailableDrives()");
			return EXITCODE_NO_SUPPORT_FOR_CDRW;
		}

		for (int i = 0; i < NeroDeviceInfos->nsdisNumDevInfos; i++) 
		{
			char driveLetter=NeroDeviceInfos->nsdisDevInfos[i].nsdiDriveLetter ? NeroDeviceInfos->nsdisDevInfos[i].nsdiDriveLetter : '?';

			if (toupper(cdParam->szDriver)==toupper(driveLetter))
			{
				this->m_handle = NeroOpenDevice (&NeroDeviceInfos->nsdisDevInfos[i]);
				if (!this->m_handle) 
				{
					LogError("can not open dirver");
					return EXITCODE_ERROR_OPENNING_DRIVE;
				}
				break;
			}

		}
		return (this->m_handle!=0)?EXITCODE_OK:EXITCODE_DRIVE_NOT_FOUND;

	}__finally{
		if(NeroDeviceInfos)NeroFreeMem (NeroDeviceInfos);
	}
}
//------------------------------------------------------------
NERO_CD_INFO * CWriteToDVD::GetCDParam(){
	if(!this->m_handle){
		LogError("handle=0!Open dirver first");
		return NULL;
	}
	NERO_CD_INFO *NeroCDInfo=0;
	NeroCDInfo = NeroGetCDInfo(this->m_handle, NGCDI_READ_CD_TEXT|NGCDI_READ_ISRC);
	if (!NeroCDInfo) {
		LogError ("NeroGetCDInfo()");
		return NULL;
	}
	return NeroCDInfo;
}
//------------------------------------------------------------
EXITCODE CWriteToDVD::BurnData(const CFileList *listRecData){

	int size = sizeof (NERO_WRITE_CD) ;
	NERO_WRITE_CD *pWriteCD = (NERO_WRITE_CD *)malloc (size);
	NERO_ISO_ITEM * pItem = NULL;
	EXITCODE code =EXITCODE_OK;

	if (!pWriteCD) {
		return EXITCODE_OUT_OF_MEMORY;
	}
	memset (pWriteCD, 0, size);
	pWriteCD->nwcdMediaType = this->m_cd_type ? MEDIA_DVD_ANY : MEDIA_CD;
	pWriteCD->nwcdNumTracks = 0;
	pWriteCD->nwcdpCDStamp=NULL;
	pWriteCD->nwcdArtist=NULL;
	pWriteCD->nwcdTitle=this->m_cdtitle.c_str();


	//NeroEjectLoadCD(this->m_handle,true);
	try{

		code = GetIsoTrack (this->m_cdtitle, listRecData,&pWriteCD->nwcdIsoTrack, &pItem);
		if (code != EXITCODE_OK)
		{
			throw code;
		}


		CBurnContextProgress m_NeroProgress;

		DWORD b_flag=NBF_WRITE
			| (this->m_IsEject ? 0:NBF_DISABLE_EJECT)
			|NBF_DETECT_NON_EMPTY_CDRW;

		int res = NeroBurn (this->m_handle,NERO_ISO_AUDIO_MEDIA ,pWriteCD,
			b_flag,0, m_NeroProgress);

		if(pWriteCD)
		{
			delete [] (char *)pWriteCD;
			pWriteCD = NULL;
		}

		if(res!=NEROAPI_BURN_OK){
			switch(res)
			{
			case NEROAPI_BURN_UNKNOWN_CD_FORMAT:
				LogError ("BurnCD() : unknown CD format");
				throw EXITCODE_UNKNOWN_CD_FORMAT;
			case NEROAPI_BURN_INVALID_DRIVE:
				LogError ("BurnCD() : invalid drive");
				throw EXITCODE_INVALID_DRIVE;
			case NEROAPI_BURN_FAILED:
				LogError ("BurnCD() : burn failed");
				throw EXITCODE_BURN_FAILED;
			case NEROAPI_BURN_FUNCTION_NOT_ALLOWED:
				LogError ("BurnCD() : function not allowed");
				throw EXITCODE_FUNCTION_NOT_ALLOWED;
			case NEROAPI_BURN_DRIVE_NOT_ALLOWED:
				LogError ("BurnCD() : drive not allowed");
				throw EXITCODE_DRIVE_NOT_ALLOWED;
			case NEROAPI_BURN_USER_ABORT:
				LogError ("BurnCD() : aborted by the user");
				throw EXITCODE_USER_ABORTED;
			default:
				LogError ("BurnCD() : unknown error");
				throw EXITCODE_UNKNOWN;
			}
		}
		
	}
	catch(EXITCODE e)
	{
		code =e;
	}   
		if(pWriteCD && pWriteCD->nwcdIsoTrack)
		{
			NeroFreeIsoTrack (pWriteCD->nwcdIsoTrack);
			pWriteCD->nwcdIsoTrack = NULL;
		}
		if(pItem)
		{
			DeleteIsoItemTree (pItem);
			pItem = NULL;
		}

	return code;
}

//------------------------------------------------------------
EXITCODE CWriteToDVD::InitNero(){

	this->m_log=fopen(".\\Burndvd_log.txt","wt");
	if(this->m_log==NULL)return EXITCODE_ERROR_OPEN_LOGFILE;

	//init
	if (!NeroAPIGlueConnect(NULL))return EXITCODE_NEROAPI_DLL_NOT_FOUND;    

	//prepare  settings	
	/*NERO_SETTINGS nsSettings;

	string path="NeroFiles";
	string vendor="ahead";
	string pcsoft="Nero - Burning Rom";
	string language="Nero.txt";*/


	//nsSettings.nstNeroFilesPath = path.c_str();
	//nsSettings.nstVendor = vendor.c_str();
	//// set pointers to various callback functions
	////nsSettings.nstIdle.ncCallbackFunction = IdleCallback;      
	//// this pointer is required to access non-static variables from callback functions
	//nsSettings.nstIdle.ncUserData = this;
	//nsSettings.nstSoftware = pcsoft.c_str();
	//nsSettings.nstUserDialog.ncUserData = this;
	////nsSettings.nstUserDialog.ncCallbackFunction = UserDialog;
	//nsSettings.nstLanguageFile =language.c_str();


	/*npProgress.npAbortedCallback = AbortedCallback;
	npProgress.npAddLogLineCallback = NULL;
	npProgress.npDisableAbortCallback = NULL;
	npProgress.npProgressCallback = NULL;
	npProgress.npSetPhaseCallback = NULL;
	npProgress.npUserData = &nsSettings;
	npProgress.npSetMajorPhaseCallback=NULL;	
	npProgress.npSubTaskProgressCallback=NULL;*/
	s_NeroSettings.nstUserDialog.ncCallbackFunction = UserDialog;
	s_NeroSettings.nstUserDialog.ncUserData = this;

	NEROAPI_INIT_ERROR initErr=NeroInit (&s_NeroSettings,NULL);
	if(initErr!=NEROAPI_INIT_OK){
		switch (initErr) 
		{
		case NEROAPI_INIT_INVALID_ARGS:
			LogError ("NeroInit() : invalid args");
			break;
		case NEROAPI_INIT_INVALID_SERIAL_NUM:
			LogError ("NeroInit() : invalid serial number");
			break;
		case NEROAPI_INIT_DEMOVERSION_EXPIRED:
			LogError ("NeroInit() : demo version has expired");
			break;
		case NEROAPI_INIT_CANNOT_LOCK:
			LogError ("NeroInit() : cannot lock");
			break;
		default:
		case NEROAPI_INIT_UNSPECIFIED_ERROR:
			LogError ("NeroInit() : unspecified error");
		}
		return EXITCODE_INTERNAL_ERROR;
	}
	return EXITCODE_OK;
}
//------------------------------------------------------------
CWriteToDVD::~CWriteToDVD(){
	fclose(m_log);
	if(this->m_handle)
		NeroCloseDevice (this->m_handle);
	NeroClearErrors ();
	NeroDone ();
	NeroAPIGlueDone();
}
//------------------------------------------------------------
void CWriteToDVD::LogError(char *action){
	char *error = NeroGetLastError ();
	fprintf(this->m_log,"%s",action);
	fprintf(this->m_log,"%s" ,(error ? error : "failed"));
	NeroFreeMem (error);
	fflush(m_log);
}
//------------------------------------------------------------
int CWriteToDVD::LogError(const char * format, ...)
{
	va_list args;	
	va_start (args, format);
	int ret;
	if (NULL != this->m_log)
	{
		ret=vfprintf (this->m_log, format, args);
	}	
	va_end (args);
	return ret;
}


