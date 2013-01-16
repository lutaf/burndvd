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
|* PROGRAM: UserDialog.cpp
|*
|* PURPOSE: Interaction with the user
******************************************************************************/


#include "stdafx.h"
#include "BurnCore.h"
//#include "Response.h"

// IMPORTANT: Unimplemented DLG_ROBO_xxx messages !!!
// 


NeroUserDlgInOut NERO_CALLBACK_ATTR CWriteToDVD::UserDialog (void* pUserData, NeroUserDlgInOut type, void *data)
{
	//_ASSERTE (pUserData != NULL);
	//CBurnContext * pBurnContext = (CBurnContext*)pUserData;
	//const bool bUserInteraction = pBurnContext->m_params->GetUserInteraction ();

	//printf ("\r                                                                                \r");

	//switch (type)
	//{
	//	case DLG_AUTO_INSERT:
	//		{
	//			static const CResponsePairs mapping[] = 
	//			{ 
	//				"Install drivers & restart", DLG_RETURN_INSTALL_DRIVER,
	//				"Turn off & restart", DLG_RETURN_OFF_RESTART,
	//				"Exit", DLG_RETURN_EXIT,
	//				"Continue at own risk", DLG_RETURN_CONTINUE,
	//				NULL
	//			};

	//			puts ("Auto Insert Notification is turned on in the system configuration.\n"
	//				"This may cause serious problems while burning: your CD might be damaged,\n"
	//				"or the system might hang up.\n"
	//				"\n"
	//				"Nero is able to burn CDs with Auto Insert Notification turned on if all\n"
	//				"necessary drivers are installed.\n"
	//				"\n"
	//				"You can do the following:\n"
	//				"Install drivers and restart Windows\n"
	//				"Turn off Auto Insert Notification and restart Windows\n"
	//				"Exit Nero\n"
	//				"Continue at your own risk\n");

	//			CResponse response (mapping, 2);

	//			return response;
	//		}
	//	case DLG_DISCONNECT_RESTART:
	//	case DLG_DISCONNECT:
	//		{
	//			puts ("Disconnect is turned off in the system configuration.\n"
	//				"This may cause serious problems while burning: your CD\n"
	//				"might be damaged, or the system might hang up.\n");
	//			
	//			if (DLG_DISCONNECT_RESTART == type)
	//			{
	//				static const CResponsePairs mapping[] = 
	//				{ 
	//					"Turn on & restart", DLG_RETURN_ON_RESTART,
	//					"Don't turn on & restart\n", DLG_RETURN_RESTART,
	//					NULL
	//				};

	//				CResponse response (mapping, 0);

	//				return response;
	//			}
	//			else
	//			{
	//				static const CResponsePairs mapping[] = 
	//				{ 
	//					"Turn on & restart", DLG_RETURN_ON_RESTART,
	//					"Don't turn on & restart", DLG_RETURN_RESTART,
	//					"Continue at own risk", DLG_RETURN_CONTINUE,
	//					NULL
	//				};

	//				CResponse response (mapping, 0);

	//				return response;
	//			}
	//		}

	//	case DLG_AUTO_INSERT_RESTART:
	//		{
	//			puts ("Auto Insert Notification is now OFF. You should restart Windows.\n");

	//			CResponseOkay response;

	//			return response;
	//		}

	//	case DLG_RESTART:
	//		{
	//			puts ("Please restart Windows now.\n");

	//			CResponseOkay response;
	//			
	//			return response;
	//		}

	//	case DLG_SETTINGS_RESTART:
	//		{
	//			static const CResponsePairs mapping[] = 
	//			{ 
	//				"Restart", DLG_RETURN_RESTART,
	//				"Continue at own risk", DLG_RETURN_CONTINUE,
	//				NULL
	//			};
	//			
	//			puts ("Nero detected some modifications of your PC system configuration\n"
	//				"and needs to modify some settings. Please restart your PC to make\n"
	//				"the changes become effective.\n");

	//			CResponse response (mapping, 0);
	//			
	//			return response;
	//		}

	//	case DLG_OVERBURN:
	//		{
	//			puts ("Sorry, this compilation contains too much data to fit on the CD\n"
	//				"with respect to the normal CD capacity. Do you want to try\n"
	//				"overburn writing at your own risk (this might cause read\n"
	//				"errors at the end of the CD or might even damage your recorder)?\n\n"
	//				"Note: It is also possible, that SCSI/Atapi errors occur at the end\n"
	//				"of the simulation or burning. Even in this case there is a certain\n"
	//				"chance, that the CD is readable.\n");

	//			const DLG_OVERBURN_INFO * pInfo = (const DLG_OVERBURN_INFO *) data;
	//			_ASSERTE (pInfo != NULL);

	//			printf ("Total blocks to be written: %d\n", pInfo->dwTotalBlocksOnCD);
	//			printf ("Free capacity in blocks: %d\n", pInfo->dwTotalCapacity);

	//			CResponseYesNoDefaultIsYes response;
	//			
	//			return response;
	//		}

	//	case DLG_COPY_QUALITY_LOSS:
	//		{
	//			puts( "Disc must be written as 'track at once' ('disc at once' not\n"
	//				"supported or not able to write such a disc). This means\n"
	//				"there might be some minor quality loss (e.g. lost audio index\n"
	//				"or different pause between audio tracks). Do you want to\n"
	//				"proceed anyway?\n");

	//			CResponseYesNoDefaultIsYes response;
	//			
	//			return response;
	//		}
	//		
	//	case DLG_COPY_FULLRISK:
	//		{
	//			puts( "Disc must be written as 'track at once' ('disc at once' not\n"
	//				"supported or not able to write such a disc).  Unfortunately\n"
	//				"your image has a special format which can most likely only\n"
	//				"be written correctly with 'disc at once'. But you may\n"
	//				"PROCEED AT YOUR OWN RISK. Do you want to continue\n"
	//				"burning?\n");

	//			CResponseYesNoDefaultIsYes response;
	//			
	//			return response;
	//		}

	//	case DLG_AUDIO_PROBLEMS:
	//		{
	//			if (((DWORD)data) & AUP_NOTRACK_FOUND)
	//			{
	//				puts ("No tracks given.");
	//				return DLG_RETURN_EXIT;
	//			}
	//			if (((DWORD)data) == AUP_MEGA_FATAL)
	//			{
	//				puts ("fatal internal problem");
	//				return DLG_RETURN_EXIT;
	//			}
	//			
	//			puts ("Nero has detected that the following audio properties are not\n"
	//				"supported by your recorder in the current write mode and need\n"
	//				"to be modified:");

	//			if (((DWORD)data) & AUP_FIRST_TR_PAUSE) puts ("- CD specification allows only 2-3 Seconds pause for the first track");
	//			if (((DWORD)data) & AUP_PAUSE_SETTINGS) puts ("- Pause length between the tracks not supported");
	//			if (((DWORD)data) & AUP_INDEX_SETTINGS) puts ("- Additional index positions not supported");
	//			if (((DWORD)data) & AUP_ISRC_SETTINGS) puts ("- ISRC codes not supported");
	//			if (((DWORD)data) & AUP_COPYPROT_SETTINGS) puts ("- Copy protection bit not supported");
	//			
	//			puts ("Nero can change the audio track settings for you to fit the\n"
	//				"capabilities of your recorder. This might cause some audio\n"
	//				"properties to be lost. Do you want Nero to change the track\n"
	//				"settings for you?\n");

	//			CResponseYesNoDefaultIsYes response;
	//			
	//			return response;
	//		}
	//		
	//	case DLG_WAITCD:
	//		{
	//			NERO_WAITCD_TYPE waitcdType = (NERO_WAITCD_TYPE) (int)data;

	//			LPCSTR psText = NeroGetLocalizedWaitCDTexts (waitcdType);
	//			if (psText != NULL)
	//			{
	//				printf("\n%s\n", psText);
	//				NeroFreeMem ((void *) psText);
	//			}

	//			if (!bUserInteraction &&
	//				waitcdType != NERO_WAITCD_AUTOEJECTLOAD &&
	//				waitcdType != NERO_WAITCD_WAITING)
	//			{
	//				// If there is supposed to be no user interaction,
	//				// wait cd dialog would never be dismissed. Abort the
	//				// burning process immediatelly (but only if waitcd
	//				// type actually designates some user interaction).
	//				// 
	//				pBurnContext->s_bAborted = true;
	//				pBurnContext->m_bPrintUserInteractionErrorMessage = true;
	//			}				
	//		}
	//		break;

	//	case DLG_WAITCD_REMINDER:
	//		printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\b");
	//		break;

	//	case DLG_WAITCD_DONE:
	//		// nothing to be done in the text interface,
	//		// should close dialog box in a GUI
	//		break;

	//	case DLG_FILESEL_IMAGE:
	//		{
	//			printf ("Enter file name to save image to: ");

	//			LPCSTR psImageFilename = pBurnContext->m_params->GetOutputImageFileName ();

	//			// Check if the image filename was supplied at command
	//			// line.
	//			// 
	//			if (NULL == psImageFilename)
	//			{
	//				if (!bUserInteraction)
	//				{
	//					// If there should be no user interaction, fail! We need
	//					// this filename and this can't be supplied automatically.
	//					// 
	//					printf ("<nothing specified>\n");
	//					pBurnContext->m_bPrintUserInteractionErrorMessage = true;
	//					return DLG_RETURN_EXIT;
	//				}
	//				else
	//				{
	//					fgets ((char *) data, 255, stdin);
	//					// Remove the carriage return
	//					((char *)data)[strlen((char *)data)-1]=0; 
	//				}
	//			}
	//			else
	//			{
	//				// First clear the supplied buffer.
	//				// 
	//				memset (data, 0, 256);
	//				
	//				// Then copy at most 255 characters.
	//				// 
	//				strncpy ((char *) data, psImageFilename, 255);
	//				puts ((char *) data);
	//			}

	//			return DLG_RETURN_TRUE;
	//		}

	//	case DLG_BURNIMAGE_CANCEL:
	//		printf("There is not enough space available to burn the image.\n");
	//		break;

	//	case DLG_NON_EMPTY_CDRW:
	//	{
	//		static const CResponsePairs mapping[] = 
	//		{ 
	//			"Erase disc", DLG_RETURN_CONTINUE,
	//			"Eject disc", DLG_RETURN_RESTART,
	//			"Cancel", DLG_RETURN_EXIT,
	//			NULL
	//		};

	//		NeroUserDlgInOut res;

	//		puts ("This disc is not empty.");

	//		// Do not ask the user if the --force_erase_cdrw
	//		// command line parameter has been provided

	//		// pUserData holds a pointer to CBurnContext which we set 
	//		// when initializing NeroAPI.
	//		// We need this pointer to obtain settings and handles
	//		// from the CBurnContext class.

	//		if (((CBurnContext*)pUserData)->m_params->GetForceEraseCDRW())
	//		{
	//			res = DLG_RETURN_CONTINUE;
	//		}
	//		else
	//		{
	//			CResponse response (mapping, 2);

	//			res = response;
	//		}

	//		if (DLG_RETURN_CONTINUE == res)
	//		{
	//			// Erase the disc

	//			NEROAPI_CDRW_ERASE_MODE mode = NEROAPI_ERASE_QUICK;

	//			int time = NeroGetCDRWErasingTime(((CBurnContext*)pUserData)->m_NeroDeviceHandle,mode);

	//			// If time is negative, it means error.
	//			//
	//			switch (time)
	//			{
	//			case -1:
	//				puts("No disc inserted!");
	//				res = DLG_RETURN_RESTART;
	//				break;

	//			case -2:
	//				puts("This recorder does not support rewritable discs!");
	//				res = DLG_RETURN_EXIT;
	//				break;

	//			case -3:
	//				puts ("The inserted disc is NOT rewritable!");
	//				res = DLG_RETURN_RESTART;
	//				break;

	//			default:
	//				if (time < 0)
	//				{
	//					// If this is some unknown error (negative value)
	//					// we should print generic error message and not
	//					// even attepmt erasing.
	//					// 
	//					puts ("Unknown error while trying to erase disc!");
	//				}
	//				else
	//				{
	//					// If the value is positive, it is the approximate
	//					// second count the process will take.
	//					// 
	//					printf("Erasing disc. This will take %d seconds.\n",time);
	//					
	//					int err = NeroEraseDisc (((CBurnContext*)pUserData)->m_NeroDeviceHandle, mode, 0, NULL);
	//					if (err)
	//					{
	//						puts("Error erasing the disc");

	//						// There was an error erasing the disc so quit.
	//						// 
	//						res = DLG_RETURN_EXIT;
	//					}
	//				}
	//			}
	//		}

	//		if (res == DLG_RETURN_CONTINUE ||
	//			res == DLG_RETURN_RESTART)
	//		{
	//			// If aborted flag was set in DLG_WAITCD, reset it here
	//			// because we have successfully erased/ejected a rewritable
	//			// disc and the process can now continue. Idle callback
	//			// obviously hasn't yet been called at this point so
	//			// playing with the aborted flag had no influence on NeroAPI.
	//			// 
	//			pBurnContext->s_bAborted = false;
	//			pBurnContext->m_bPrintUserInteractionErrorMessage = false;
	//		}

	//		return res;
	//	}

	//	case DLG_WAITCD_MEDIA_INFO:
	//		{
	//			const NERO_DLG_WAITCD_MEDIA_INFO * pMediaInfo = (const NERO_DLG_WAITCD_MEDIA_INFO *) data;

	//			printf ("Last detected media: %s\n", pMediaInfo->ndwmiLastDetectedMediaName);
	//			printf ("Requested media: %s\n", pMediaInfo->ndwmiRequestedMediaName);

	//			// The return value is ignored.
	//			// 
	//		}
	//		break;

	//	case DLG_COMP_REC_CONFLICT:
	//		puts ("The compilation cannot be written on this recorder! "
	//				"You should modify your compilation settings or burn "
	//				"the disc on another recorder, that supports the required medium type.");
	//		break;

	//	case DLG_WRONG_MEDIUM:
	//		puts ("Another type of medium must be used to burn this compilation!");
	//		break;

	//	case DLG_MESSAGEBOX:
	//		{
	//			const NERODLG_MESSAGEBOX * pMsgBox = (const NERODLG_MESSAGEBOX *) data;
	//			_ASSERTE (pMsgBox != NULL);

	//			if (pMsgBox != NULL)
	//			{
	//				// Construct a generic reponse according to the parameters
	//				// passed.
	//				// 
	//				CResponseGeneric response (pMsgBox->type, pMsgBox->defaultReturn);

	//				// If data was interpreted correctly, proceed. If not, the
	//				// default handling will take place.
	//				// 
	//				if (response.IsOk ())
	//				{
	//					// Translate the icon type into a character.
	//					// 
	//					char chIcon;
	//					switch (pMsgBox->icon)
	//					{
	//					case NDIT_INFO:
	//						chIcon = 'i';
	//						break;
	//					case NDIT_WARNING:
	//						chIcon = '!';
	//						break;
	//					case NDIT_QUESTION:
	//						chIcon = '?';
	//						break;
	//					case NDIT_ERROR:
	//					default:
	//						chIcon = 'E';
	//					}

	//					// Print the message and ask for a response.
	//					// 
	//					printf ("[%c] %s\n", chIcon, pMsgBox->message);

	//					return response;
	//				}
	//			}
	//		}
	//		break;
	//}

	return DLG_RETURN_NOT_HANDLED;
}
