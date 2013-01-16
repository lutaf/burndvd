burndvd
=======

Pure C++ project for  CD/DVD burning use Nero Api  
基于Nero SDK开发的CD/DVD刻录程序


##说明

- 用Microsoft Visual C++ 2003开发，c++代码，容器只使用了STL
- 编程生成为win32 dll
- 项目背景:<http://lutaf.com>
- Nero SDK 下载地址：<ftp://ftp6.nero.com/>


##Dll接口文档
in  `FileInterface.h `

		typedef struct tagCNode
		{
		 string szLocalPath;
		 bool Recursive;
		
		}CNode;
		typedef list<CNode*> CFileList;
				
		class IWriteCD
		{
		public:
		 virtual EXITCODE SetCDParam(CDPARAM *cdParam)=0;
		 virtual NERO_CD_INFO * GetCDParam()=0;//return 0 when failed
		 virtual EXITCODE BurnData(const CFileList *listRecData)=0;
		 virtual void StopBurn()=0;
		 virtual EXITCODE InitNero()=0;
		};




