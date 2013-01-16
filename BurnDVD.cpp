// BurnDVD.cpp : 定义 DLL 应用程序的入口点。
//

#include "stdafx.h"
#include "BurnDVD.h"
BOOL APIENTRY DllMain( HANDLE hModule, 
					  DWORD  ul_reason_for_call, 
					  LPVOID lpReserved
					  )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
//---------------------------------------------------------
extern "C" void __declspec(dllexport) CreateWriteCDClass(IWriteCD *&fp){
	fp=  new CWriteToDVD();
}
//-------------------------------------------------------------
extern "C" void __declspec(dllexport) RemoveWriteCDClass(IWriteCD *fp){
	delete fp;
}
//----------------------------------------------------------------
extern "C" void __declspec(dllexport) Free_NeroData(void *ptr){
	NeroFreeMem(ptr);
}

