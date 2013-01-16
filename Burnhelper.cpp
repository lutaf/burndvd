#include "BurnCore.h"
#include <assert.h>


NERO_SETTINGS CWriteToDVD::s_NeroSettings =
{
	NULL,
	"ahead", "Nero - Burning Rom",
	"Nero.txt",
	{IdleCallback, &s_NeroSettings},
	{UserDialog, &s_NeroSettings}
};


CBurnContextProgress::CBurnContextProgress ()
{
	if (m_pNeroProgress != NULL)
	{
		m_pNeroProgress->npAbortedCallback = CWriteToDVD::AbortedCallback;
		m_pNeroProgress->npAddLogLineCallback = CWriteToDVD::AddLogLine;
		m_pNeroProgress->npDisableAbortCallback = CWriteToDVD::DisableAbortCallback;
		m_pNeroProgress->npProgressCallback = CWriteToDVD::ProgressCallback;
		m_pNeroProgress->npSetMajorPhaseCallback = CWriteToDVD::SetMajorPhaseCallback;
		m_pNeroProgress->npSetPhaseCallback = CWriteToDVD::SetPhaseCallback;
		m_pNeroProgress->npSubTaskProgressCallback = NULL;	// Write buffer fill level callback (we don't use this as it is complicated to visualize)
		m_pNeroProgress->npUserData = &CWriteToDVD::s_NeroSettings;
	}
}
void NERO_CALLBACK_ATTR CWriteToDVD::DisableAbortCallback (void *pUserData, BOOL enableAbort)
{
	// Just print out one or the other string.
	// 
	puts ((!enableAbort)? "[i] The current process cannot be interrupted": "[i] The process can be interrupted again");
}
DWORD NERO_CALLBACK_ATTR CWriteToDVD::WriteIOCallback (void *pUserData, BYTE *pBuffer, DWORD dwLen)
{
	// NeroAPI provides a file handle and a buffer of data, containing dwLen bytes

	return fwrite (pBuffer, 1, dwLen, (FILE *)pUserData);
}


BOOL NERO_CALLBACK_ATTR CWriteToDVD::EOFCallback (void *pUserData)
{
	return feof ((FILE *)pUserData);
}


BOOL NERO_CALLBACK_ATTR CWriteToDVD::ErrorCallback (void *pUserData)
{
	return ferror ((FILE *)pUserData);
}

// ReadIOCallback will be used when PCM is written to CD

DWORD NERO_CALLBACK_ATTR CWriteToDVD::ReadIOCallback (void *pUserData, BYTE *pBuffer, DWORD dwLen)
{
	// Read dwLen number of bytes from the file into pBuffer

	return fread (pBuffer, 1, dwLen, (FILE *)pUserData);
}
void NERO_CALLBACK_ATTR CWriteToDVD::AddLogLine (void *pUserData, NERO_TEXT_TYPE type, const char *text)
{
	const char *header;
	const char *start;

	// Evaluate the type of log entry that should be added
	// and assign the line header accordingly.

	switch (type)
	{
		case NERO_TEXT_INFO:        // informative text
			header = "[i]";
			break;
		case NERO_TEXT_STOP:        // some operation stopped prematurely
			header = "[#]";
			break;
		case NERO_TEXT_EXCLAMATION: // important information 
			header = "[!]";
			break;
		case NERO_TEXT_QUESTION:    // a question which requires an answer
			header = "[?]";
			break;
		case NERO_TEXT_DRIVE:		// a message concerning a CD-ROM drive or recorder
			header = "[-]";
			break;
		default:
			header = "";
	}

	// Step through the message text, considering newline characters
	// and inserting a line break every 76 characters if the line is longer

	start = text;
	while (*start)
	{
		// search for newline NL(LF) and set a pointer to the 
		// next newline character. If no newline is found end becomes NULL.

		char *end = (char*)strchr (start, '\n');

		// Determine the length of the string part to be printed.
		// If a newline character was found the length is the difference between end and start
		// Otherwise there is no newline between the current position of start in the string
		// and the end of the string. So the length can be determined by a simple call to strlen.

		int len;
		if (NULL != end)
		{
			len = end - start;
			
			// replace newline character with 0 to not print parts of the following line
			*end = 0;
		}
		else
		{
			len = strlen (start);
		}

		// We also make sure that no more than 76 characters are printed
		// no matter how long the current string part really is.

		if (len > 76)
		{
			len = 76;
		}

		// The formatted output:

		printf ("%-4.4s%-76.76s", header, start);
		header = ""; // we print the header only in the first line


		// Shift the start pointer right by the amount of bytes just printed.

		start += len;

		// If newline characters were found start has to be set to the next character
		// If end contains NULL this means that either no newlines were found or
		// the end of the string has been reached.


		if (NULL != end)
		{
			++start;
		}
	}

	printf("\n");
}
BOOL NERO_CALLBACK_ATTR CWriteToDVD::ProgressCallback (void *pUserData, DWORD dwProgressInPercent)
{
	// print the numerical value

	printf ("%03d%% ", dwProgressInPercent);

	// print the progress meter

	int w = (74*dwProgressInPercent)/100;
	for (int i = w; i > 0; i --)
	{
		printf ("#");
	}
	for (i = 74 - w; i > 0; i --)
	{
		printf (".");
	}

	// carriage return

	printf ("\r");

	fflush (stdout);

	// We simply return the aborted flag

	return s_bAborted;
}
struct PHASE_MAPPING
{
	NERO_MAJOR_PHASE m_phase;
	LPCSTR m_psDescription;
};

static PHASE_MAPPING s_PhaseMapping[] = {
{NERO_PHASE_UNSPECIFIED					, "Unspecified"},
{NERO_PHASE_START_CACHE					, "Start cache"},
{NERO_PHASE_DONE_CACHE					, "Done cache"},
{NERO_PHASE_FAIL_CACHE					, "Fail cache"},
{NERO_PHASE_ABORT_CACHE					, "Abort cache"},
{NERO_PHASE_START_TEST					, "Start test"},
{NERO_PHASE_DONE_TEST					, "Done test"},
{NERO_PHASE_FAIL_TEST					, "Fail test"},
{NERO_PHASE_ABORT_TEST					, "Abort test"},
{NERO_PHASE_START_SIMULATE				, "Start simulate"},
{NERO_PHASE_DONE_SIMULATE				, "Done simulate"},
{NERO_PHASE_FAIL_SIMULATE				, "Fail simulate"},
{NERO_PHASE_ABORT_SIMULATE				, "Abort simulate"},
{NERO_PHASE_START_WRITE					, "Start write"},
{NERO_PHASE_DONE_WRITE					, "Done write"},
{NERO_PHASE_FAIL_WRITE					, "Fail write"},
{NERO_PHASE_ABORT_WRITE					, "Abort write"},
{NERO_PHASE_START_SIMULATE_NOSPD		, "Start simulate nospd"},
{NERO_PHASE_DONE_SIMULATE_NOSPD			, "Done simulate nospd"},
{NERO_PHASE_FAIL_SIMULATE_NOSPD			, "Fail simulate nospd"},
{NERO_PHASE_ABORT_SIMULATE_NOSPD		, "Abort simulate nospd"},
{NERO_PHASE_START_WRITE_NOSPD			, "Start write nospd"},
{NERO_PHASE_DONE_WRITE_NOSPD			, "Done write nospd"},
{NERO_PHASE_FAIL_WRITE_NOSPD			, "Fail write nospd"},
{NERO_PHASE_ABORT_WRITE_NOSPD			, "Abort write nospd"},
{NERO_PHASE_PREPARE_ITEMS				, "Prepare items"},
{NERO_PHASE_VERIFY_COMPILATION			, "Verify compilation"},
{NERO_PHASE_VERIFY_ABORTED				, "Verify aborted"},
{NERO_PHASE_VERIFY_END_OK				, "Verify end ok"},
{NERO_PHASE_VERIFY_END_FAIL				, "Verify end fail"},
{NERO_PHASE_ENCODE_VIDEO				, "Encode video"},
{NERO_PHASE_SEAMLESSLINK_ACTIVATED      , "Seamlesslink activated"},
{NERO_PHASE_BUP_ACTIVATED				, "BUP activated"},
{NERO_PHASE_START_FORMATTING			, "Start formatting"},
{NERO_PHASE_CONTINUE_FORMATTING			, "Continue formatting"},
{NERO_PHASE_FORMATTING_SUCCESSFUL		, "Formatting successful"},
{NERO_PHASE_FORMATTING_FAILED			, "Formatting failed"},
{NERO_PHASE_PREPARE_CD					, "Prepare CD"},
{NERO_PHASE_DONE_PREPARE_CD				, "Done prepare CD"},
{NERO_PHASE_FAIL_PREPARE_CD				, "Fail prepare CD"},
{NERO_PHASE_ABORT_PREPARE_CD			, "Abort prepare CD"},
{NERO_PHASE_DVDVIDEO_DETECTED			, "Dvdvideo detected"},
{NERO_PHASE_DVDVIDEO_REALLOC_STARTED	, "Dvdvideo realloc started"},
{NERO_PHASE_DVDVIDEO_REALLOC_COMPLETED	, "Dvdvideo realloc completed"},
{NERO_PHASE_DVDVIDEO_REALLOC_NOTNEEDED	, "Dvdvideo realloc not needed"},
{NERO_PHASE_DVDVIDEO_REALLOC_FAILED		, "Dvdvideo realloc failed"},
{NERO_PHASE_DRM_CHECK_FAILURE			, "DRM check failure"},
};

void NERO_CALLBACK_ATTR CWriteToDVD::SetMajorPhaseCallback (void *pUserData, NERO_MAJOR_PHASE phase, void * reserved)
{
	// Declare the buffer variable and set it to a default message.
	//
	char sBuffer[100] = "PHASE: unknown";

	// Search for the appropriate phase mapping description string.
	//
	for (int i = 0; i < sizeof (s_PhaseMapping)/sizeof (s_PhaseMapping[0]); i ++)
	{
		if (phase == s_PhaseMapping[i].m_phase)
		{
			// If the phase was found in the phase mapping table then
			// use it and break the for loop. We don't need to check
			// for buffer overflow as we "know" all our messages are short
			// enough to fit in the buffer.
			//
			sprintf (sBuffer, "PHASE: %s", s_PhaseMapping[i].m_psDescription);
			break;
		}
	}

	printf ("    %-76s\n", sBuffer);
}
void NERO_CALLBACK_ATTR CWriteToDVD::SetPhaseCallback (void *pUserData, const char *text)
{
	printf ("    %-76s\n", text);
}
BOOL NERO_CALLBACK_ATTR CWriteToDVD::IdleCallback (void* pUserData)
{
	assert(pUserData!=0);
	//CWriteToDVD* ptr=(CWriteToDVD*)pUserData;
	//block here!
	/*static MSG msg;
	if(ptr->IsBurning()){
		while (PeekMessage(&msg,NULL,NULL,NULL,PM_NOREMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}*/
	return s_bAborted;
}
BOOL NERO_CALLBACK_ATTR CWriteToDVD::AbortedCallback(void *pUserData)
{
	// do not ask the user if he really wants to abort
	// just return the aborted flag

	return s_bAborted;
}
