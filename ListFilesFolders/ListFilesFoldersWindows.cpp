/*This includes functions which will help to launch windows and dialogs, that is this file contains function
 * responsible for User Interface. Mainly it has two user interfaces
 *1. Main Dialog box - This will get the information regarding the search criteria for searching the files.
 *2. Resultant Dialog box - This will show the results based on the search criteia which was selected in Main Dialog box.
 *
 *Developer = Harsha Kadekar
 */


#include <windows.h>
#include <stdio.h>
#include <commctrl.h>
#include <ShlObj.h>
#include <Shlwapi.h>
#include "resource.h"
#include "ListFilesFoldersApp.h"
#include "ListFilesFoldersWindows.h"
#include "SearchCriteria.h"


bool bFileSizeSelected = false;
bool bMaxFileSize = false;
bool bMinFileSize = false;
bool bReadOnlySelected = false;
bool bHiddenSelected = false;
bool bCreationDateSelected = false;
bool bCreationGreater = false;
bool bCreationLesser = false;
bool bModificationDateSelected = false;
bool bModificationGreater = false;
bool bModificationLesser = false;
HWND hTree = NULL;
HINSTANCE hInst;

SEARCHFILEFOLDER *sffSearchCriteria = NULL;
FILEINFO* SearchFolderInfo = NULL;

void BrowseForFolder(HWND hDlg)
{

	TCHAR path[MAX_PATH];
	BROWSEINFO bi = {0};
	bi.lpszTitle = L"Select the Folder to start search operation";
	LPITEMIDLIST pidl = SHBrowseForFolder(&bi);

	if(pidl != NULL)
	{
		SHGetPathFromIDList(pidl, path);
		//SendMessage(GetDlgItem(hDlg, IDC_EDIT_MINSIZE), 
		SetDlgItemText(hDlg, IDC_EDIT_SEARLOC, path);


	}
}

int RecursiveFormChain(FILEINFO* ptrCurrentFolder)
{
	WIN32_FIND_DATA FindFileData;
	TCHAR szFullPattern[MAX_PATH];
	TCHAR szLocation[MAX_PATH];
	HANDLE hFind;
	__int64 nFileSize = 0;
	__int64 nTempSize = 0;
	LSTFILEINSUBFOLDER* lstOfSubFolder = NULL;
	LSTFILEINSUBFOLDER* ptrCurrentSubFile = NULL;
	FILEINFO* ptrNewFile = NULL;
	LARGE_INTEGER sz;
	WCHAR* strLocation = NULL;
	WCHAR* strFileName = NULL;

	memset(szLocation, L'\0', MAX_PATH);
	PathCombine(szFullPattern, ptrCurrentFolder->location, ptrCurrentFolder->strFileName);
	PathCombine(szLocation, ptrCurrentFolder->location, ptrCurrentFolder->strFileName);
	PathCombine(szFullPattern, szFullPattern, L"*");
	hFind = FindFirstFile(szFullPattern, &FindFileData);
	if(hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			if(wcscmp(FindFileData.cFileName, L".") != 0 && wcscmp(FindFileData.cFileName, L"..") != 0)
			{
				if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					if(!((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) && (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) && (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM)))
					{
						ptrNewFile = (FILEINFO*)malloc(sizeof(FILEINFO));
						strLocation = (WCHAR*)malloc((wcslen(szLocation)+1)*sizeof(WCHAR));
						memset(strLocation, L'\0', (wcslen(szLocation)+1));
						wcscpy(strLocation, szLocation);
						ptrNewFile->location = strLocation;
						ptrNewFile->ParentFolder = ptrCurrentFolder;
						ptrNewFile->bIsFile = false;
						ptrNewFile->actualData = &FindFileData;
						ptrNewFile->strFileName = FindFileData.cFileName;
						PathStripPath(ptrNewFile->strFileName);
						strFileName = (WCHAR*)malloc((wcslen(ptrNewFile->strFileName)+1)*sizeof(WCHAR));
						memset(strFileName, L'\0', (wcslen(ptrNewFile->strFileName)+1));
						wcscpy(strFileName, ptrNewFile->strFileName);
						ptrNewFile->strFileName = strFileName;

						if((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN))
							ptrNewFile->bHidden = true;
						else
							ptrNewFile->bHidden = false;

						if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_READONLY)
							ptrNewFile->bReadonly = true;
						else
							ptrNewFile->bReadonly = false;


						ptrNewFile->dwFileAttributes = FindFileData.dwFileAttributes;
						ptrNewFile->ftCreationTime = FindFileData.ftCreationTime;
						ptrNewFile->ftModificationTime = FindFileData.ftLastWriteTime;

						RecursiveFormChain(ptrNewFile);

						nFileSize += ptrNewFile->dwFileSize;

						lstOfSubFolder = (LSTFILEINSUBFOLDER*)malloc(sizeof(LSTFILEINSUBFOLDER));
						lstOfSubFolder->nextFileFolder = NULL;
						lstOfSubFolder->fileFolder = ptrNewFile;

						if(ptrCurrentSubFile == NULL)
						{
							ptrCurrentFolder->ChildElements = lstOfSubFolder;
							ptrCurrentSubFile = lstOfSubFolder;

						}
						else
						{
							ptrCurrentSubFile->nextFileFolder = lstOfSubFolder;
							ptrCurrentSubFile = lstOfSubFolder;
						}





					}
					else
					{
						ptrNewFile = (FILEINFO*)malloc(sizeof(FILEINFO));
						strLocation = (WCHAR*)malloc((wcslen(szLocation)+1)*sizeof(WCHAR));
						memset(strLocation, L'\0', (wcslen(szLocation)+1));
						wcscpy(strLocation, szLocation);
						ptrNewFile->location = strLocation;
						ptrNewFile->ParentFolder = ptrCurrentFolder;
						ptrNewFile->bIsFile = false;
						ptrNewFile->actualData = &FindFileData;
						ptrNewFile->strFileName = FindFileData.cFileName;
						PathStripPath(ptrNewFile->strFileName);
						strFileName = (WCHAR*)malloc((wcslen(ptrNewFile->strFileName)+1)*sizeof(WCHAR));
						memset(strFileName, L'\0', (wcslen(ptrNewFile->strFileName)+1));
						wcscpy(strFileName, ptrNewFile->strFileName);
						ptrNewFile->strFileName = strFileName;

						if((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN))
							ptrNewFile->bHidden = true;
						else
							ptrNewFile->bHidden = false;

						if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_READONLY)
							ptrNewFile->bReadonly = true;
						else
							ptrNewFile->bReadonly = false;


						ptrNewFile->dwFileAttributes = FindFileData.dwFileAttributes;
						ptrNewFile->ftCreationTime = FindFileData.ftCreationTime;
						ptrNewFile->ftModificationTime = FindFileData.ftLastWriteTime;

						//RecursiveFormChain(ptrNewFile);
						ptrNewFile->dwFileSize = 0;
						ptrNewFile->ChildElements = NULL;

						ptrNewFile->bError = true;

						nFileSize += ptrNewFile->dwFileSize;

						lstOfSubFolder = (LSTFILEINSUBFOLDER*)malloc(sizeof(LSTFILEINSUBFOLDER));
						lstOfSubFolder->nextFileFolder = NULL;
						lstOfSubFolder->fileFolder = ptrNewFile;

						if(ptrCurrentSubFile == NULL)
						{
							ptrCurrentFolder->ChildElements = lstOfSubFolder;
							ptrCurrentSubFile = lstOfSubFolder;

						}
						else
						{
							ptrCurrentSubFile->nextFileFolder = lstOfSubFolder;
							ptrCurrentSubFile = lstOfSubFolder;
						}


					}

				}
				else
				{

					ptrNewFile = (FILEINFO*)malloc(sizeof(FILEINFO));
					strLocation = (WCHAR*)malloc((wcslen(szLocation)+1)*sizeof(WCHAR));
					memset(strLocation, L'\0', (wcslen(szLocation)+1));
					wcscpy(strLocation, szLocation);
					ptrNewFile->location = strLocation;
					ptrNewFile->ParentFolder = ptrCurrentFolder;
					ptrNewFile->bIsFile = true;
					ptrNewFile->actualData = &FindFileData;
					ptrNewFile->strFileName = FindFileData.cFileName;//ptrNewFile->location;
					PathStripPath(ptrNewFile->strFileName);
					strFileName = (WCHAR*)malloc((wcslen(ptrNewFile->strFileName)+1)*sizeof(WCHAR));
					memset(strFileName, L'\0', (wcslen(ptrNewFile->strFileName)+1));
					wcscpy(strFileName, ptrNewFile->strFileName);
					ptrNewFile->strFileName = strFileName;
					if((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN))
						ptrNewFile->bHidden = true;
					else
						ptrNewFile->bHidden = false;

					if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_READONLY)
						ptrNewFile->bReadonly = true;
					else
						ptrNewFile->bReadonly = false;


					ptrNewFile->dwFileAttributes = FindFileData.dwFileAttributes;
					ptrNewFile->ftCreationTime = FindFileData.ftCreationTime;
					ptrNewFile->ftModificationTime = FindFileData.ftLastWriteTime;

					ptrNewFile->ChildElements = NULL;

							
					sz.LowPart = FindFileData.nFileSizeLow;
					sz.HighPart = FindFileData.nFileSizeHigh;
					ptrNewFile->dwFileSize = sz.QuadPart;

					nFileSize += ptrNewFile->dwFileSize;

					lstOfSubFolder = (LSTFILEINSUBFOLDER*)malloc(sizeof(LSTFILEINSUBFOLDER));
					lstOfSubFolder->nextFileFolder = NULL;
					lstOfSubFolder->fileFolder = ptrNewFile;

					if(ptrCurrentSubFile == NULL)
					{
						ptrCurrentFolder->ChildElements = lstOfSubFolder;
						ptrCurrentSubFile = lstOfSubFolder;

					}
					else
					{
						ptrCurrentSubFile->nextFileFolder = lstOfSubFolder;
						ptrCurrentSubFile = lstOfSubFolder;
					}

				}

			}

		}while(FindNextFile(hFind, &FindFileData));



		ptrCurrentFolder->dwFileSize = nFileSize;
	}
	
	return 0;

}

int FormChain()
{
	WIN32_FIND_DATA FindFileData;
	TCHAR szFullPattern[MAX_PATH];
	HANDLE hFind;
	__int64 nFileSize = 0;
	__int64 nTempSize = 0;
	LSTFILEINSUBFOLDER* lstOfSubFolder = NULL;
	LSTFILEINSUBFOLDER* ptrCurrentSubFile = NULL;
	FILEINFO* ptrNewFile = NULL;
	WCHAR* strLocation = NULL;
	WCHAR* strFileName = NULL;
	LARGE_INTEGER sz;
	int nLastError = 0;
	

	if(sffSearchCriteria != NULL)
	{
		hFind = FindFirstFile(sffSearchCriteria->location, &FindFileData);
		if(hFind == INVALID_HANDLE_VALUE)
		{
			nLastError = GetLastError();

			return -1;
		}

		if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			SearchFolderInfo = (FILEINFO*)malloc(sizeof(FILEINFO));
			
			strLocation = (WCHAR*)malloc((wcslen(sffSearchCriteria->location)+1)*sizeof(WCHAR));
			memset(strLocation, L'\0', (wcslen(sffSearchCriteria->location)+1));
			wcscpy(strLocation, sffSearchCriteria->location);
			SearchFolderInfo->location = strLocation;
			SearchFolderInfo->ParentFolder = NULL;
			SearchFolderInfo->bIsFile = false;
			SearchFolderInfo->actualData = &FindFileData;
			SearchFolderInfo->strFileName = FindFileData.cFileName;
			PathStripPath(SearchFolderInfo->strFileName);
			strFileName = (WCHAR*)malloc((wcslen(SearchFolderInfo->strFileName)+1)*sizeof(WCHAR));
			memset(strFileName, L'\0', (wcslen(SearchFolderInfo->strFileName)+1));
			wcscpy(strFileName, SearchFolderInfo->strFileName);
			SearchFolderInfo->strFileName = strFileName;

			if((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN))
				SearchFolderInfo->bHidden = true;
			else
				SearchFolderInfo->bHidden = false;

			if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_READONLY)
				SearchFolderInfo->bReadonly = true;
			else
				SearchFolderInfo->bReadonly = false;


			SearchFolderInfo->dwFileAttributes = FindFileData.dwFileAttributes;
			SearchFolderInfo->ftCreationTime = FindFileData.ftCreationTime;
			SearchFolderInfo->ftModificationTime = FindFileData.ftLastWriteTime;

			PathCombine(szFullPattern, sffSearchCriteria->location, L"*");
			hFind = FindFirstFile(szFullPattern, &FindFileData);
			if(hFind != INVALID_HANDLE_VALUE)
			{
				do
				{
					if(wcscmp(FindFileData.cFileName, L".") != 0 && wcscmp(FindFileData.cFileName, L"..") != 0)
					{
						if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
						{
							if(!((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) && (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) && (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM)))
							{
								ptrNewFile = (FILEINFO*)malloc(sizeof(FILEINFO));
								//ptrNewFile->location = sffSearchCriteria->location;
								strLocation = (WCHAR*)malloc((wcslen(sffSearchCriteria->location)+1)*sizeof(WCHAR));
								memset(strLocation, L'\0', (wcslen(sffSearchCriteria->location)+1));
								wcscpy(strLocation, sffSearchCriteria->location);
								ptrNewFile->location = strLocation;
								ptrNewFile->ParentFolder = SearchFolderInfo;
								ptrNewFile->bIsFile = false;
								ptrNewFile->actualData = &FindFileData;
								ptrNewFile->strFileName = FindFileData.cFileName;
								PathStripPath(ptrNewFile->strFileName);
								strFileName = (WCHAR*)malloc((wcslen(ptrNewFile->strFileName)+1)*sizeof(WCHAR));
								memset(strFileName, L'\0', (wcslen(ptrNewFile->strFileName)+1));
								wcscpy(strFileName, ptrNewFile->strFileName);
								ptrNewFile->strFileName = strFileName;
								if((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN))
									ptrNewFile->bHidden = true;
								else
									ptrNewFile->bHidden = false;

								if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_READONLY)
									ptrNewFile->bReadonly = true;
								else
									ptrNewFile->bReadonly = false;


								ptrNewFile->dwFileAttributes = FindFileData.dwFileAttributes;
								ptrNewFile->ftCreationTime = FindFileData.ftCreationTime;
								ptrNewFile->ftModificationTime = FindFileData.ftLastWriteTime;

								RecursiveFormChain(ptrNewFile);

								nFileSize += ptrNewFile->dwFileSize;

								lstOfSubFolder = (LSTFILEINSUBFOLDER*)malloc(sizeof(LSTFILEINSUBFOLDER));
								lstOfSubFolder->nextFileFolder = NULL;
								lstOfSubFolder->fileFolder = ptrNewFile;

								if(ptrCurrentSubFile == NULL)
								{
									SearchFolderInfo->ChildElements = lstOfSubFolder;
									ptrCurrentSubFile = lstOfSubFolder;

								}
								else
								{
									ptrCurrentSubFile->nextFileFolder = lstOfSubFolder;
									ptrCurrentSubFile = lstOfSubFolder;
								}







							}
							else
							{

								ptrNewFile = (FILEINFO*)malloc(sizeof(FILEINFO));
								strLocation = (WCHAR*)malloc((wcslen(sffSearchCriteria->location)+1)*sizeof(WCHAR));
								memset(strLocation, L'\0', (wcslen(sffSearchCriteria->location)+1));
								wcscpy(strLocation, sffSearchCriteria->location);
								ptrNewFile->location = strLocation;
								ptrNewFile->ParentFolder = SearchFolderInfo;
								ptrNewFile->bIsFile = false;
								ptrNewFile->actualData = &FindFileData;
								ptrNewFile->strFileName = FindFileData.cFileName;
								PathStripPath(ptrNewFile->strFileName);
								strFileName = (WCHAR*)malloc((wcslen(ptrNewFile->strFileName)+1)*sizeof(WCHAR));
								memset(strFileName, L'\0', (wcslen(ptrNewFile->strFileName)+1));
								wcscpy(strFileName, ptrNewFile->strFileName);
								ptrNewFile->strFileName = strFileName;
								if((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN))
								ptrNewFile->bHidden = true;
								else
									ptrNewFile->bHidden = false;

								if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_READONLY)
									ptrNewFile->bReadonly = true;
								else
									ptrNewFile->bReadonly = false;


								ptrNewFile->dwFileAttributes = FindFileData.dwFileAttributes;
								ptrNewFile->ftCreationTime = FindFileData.ftCreationTime;
								ptrNewFile->ftModificationTime = FindFileData.ftLastWriteTime;

								//RecursiveFormChain(ptrNewFile);
								ptrNewFile->dwFileSize = 0;
								ptrNewFile->ChildElements = NULL;

								ptrNewFile->bError = true;

								nFileSize += ptrNewFile->dwFileSize;

								lstOfSubFolder = (LSTFILEINSUBFOLDER*)malloc(sizeof(LSTFILEINSUBFOLDER));
								lstOfSubFolder->nextFileFolder = NULL;
								lstOfSubFolder->fileFolder = ptrNewFile;

								if(ptrCurrentSubFile == NULL)
								{
									SearchFolderInfo->ChildElements = lstOfSubFolder;
									ptrCurrentSubFile = lstOfSubFolder;

								}
								else
								{
									ptrCurrentSubFile->nextFileFolder = lstOfSubFolder;
									ptrCurrentSubFile = lstOfSubFolder;
								}

							}
						}
						else
						{
							ptrNewFile = (FILEINFO*)malloc(sizeof(FILEINFO));
							ptrNewFile->location = sffSearchCriteria->location;//FindFileData.cFileName;
							strLocation = (WCHAR*)malloc((wcslen(sffSearchCriteria->location)+1)*sizeof(WCHAR));
							memset(strLocation, L'\0', (wcslen(sffSearchCriteria->location)+1));
							wcscpy(strLocation, sffSearchCriteria->location);
							ptrNewFile->location = strLocation;
							ptrNewFile->ParentFolder = SearchFolderInfo;
							ptrNewFile->bIsFile = true;
							ptrNewFile->actualData = &FindFileData;
							ptrNewFile->strFileName = FindFileData.cFileName;//ptrNewFile->location;
							PathStripPath(ptrNewFile->strFileName);
							strFileName = (WCHAR*)malloc((wcslen(ptrNewFile->strFileName)+1)*sizeof(WCHAR));
							memset(strFileName, L'\0', (wcslen(ptrNewFile->strFileName)+1));
							wcscpy(strFileName, ptrNewFile->strFileName);
							ptrNewFile->strFileName = strFileName;
							if((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN))
								ptrNewFile->bHidden = true;
							else
								ptrNewFile->bHidden = false;

							if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_READONLY)
								ptrNewFile->bReadonly = true;
							else
								ptrNewFile->bReadonly = false;


							ptrNewFile->dwFileAttributes = FindFileData.dwFileAttributes;
							ptrNewFile->ftCreationTime = FindFileData.ftCreationTime;
							ptrNewFile->ftModificationTime = FindFileData.ftLastWriteTime;

							ptrNewFile->ChildElements = NULL;

							
							sz.LowPart = FindFileData.nFileSizeLow;
							sz.HighPart = FindFileData.nFileSizeHigh;
							ptrNewFile->dwFileSize = sz.QuadPart;

							nFileSize += ptrNewFile->dwFileSize;

							lstOfSubFolder = (LSTFILEINSUBFOLDER*)malloc(sizeof(LSTFILEINSUBFOLDER));
							lstOfSubFolder->nextFileFolder = NULL;
							lstOfSubFolder->fileFolder = ptrNewFile;

							if(ptrCurrentSubFile == NULL)
							{
								SearchFolderInfo->ChildElements = lstOfSubFolder;
								ptrCurrentSubFile = lstOfSubFolder;

							}
							else
							{
								ptrCurrentSubFile->nextFileFolder = lstOfSubFolder;
								ptrCurrentSubFile = lstOfSubFolder;
							}

							



						}
					}

				}while(FindNextFile(hFind, &FindFileData));

				SearchFolderInfo->dwFileSize = nFileSize;

			}
		}
	}
}

/*Function = Dialogproc
 *Description = this is the dialog procedure which will handle all the commands which are given to the Main Dialog ie the dialog having
 *				search criteria
 *Parameters = 
 *Return Value = 
 */
INT_PTR CALLBACK DialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	bool bReturnValue = FALSE;
	int i = 0;

	switch(uMsg)
	{
	case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
			case IDC_CHECK_FILESIZE:
				{
					if(bFileSizeSelected)
					{
						bFileSizeSelected = false;
						
						EnableWindow( GetDlgItem(hDlg, IDC_RADIO_MAXSIZE), false);
						EnableWindow( GetDlgItem(hDlg, IDC_EDIT_MAXSIZE), false);
						EnableWindow( GetDlgItem(hDlg, IDC_RADIO_MINSIZE), false);
						EnableWindow( GetDlgItem(hDlg, IDC_EDIT_MINSIZE), false);

						bMaxFileSize = false;
						bMinFileSize = false;

						
					}
					else
					{
						bFileSizeSelected = true;

						EnableWindow( GetDlgItem(hDlg, IDC_RADIO_MAXSIZE), true);
						if(IsDlgButtonChecked(hDlg, IDC_RADIO_MAXSIZE))
						{
							EnableWindow( GetDlgItem(hDlg, IDC_EDIT_MAXSIZE), true);
							bMaxFileSize = true;
						}
						
						EnableWindow( GetDlgItem(hDlg, IDC_RADIO_MINSIZE), true);
						if(IsDlgButtonChecked(hDlg, IDC_RADIO_MINSIZE))
						{
							EnableWindow( GetDlgItem(hDlg, IDC_EDIT_MINSIZE), true);
							bMinFileSize = true;
						}

					}
					break;
				}
			case IDC_CHECK_CREATION:
				{
					if(bCreationDateSelected)
					{
						bCreationDateSelected = false;

						EnableWindow(GetDlgItem(hDlg, IDC_RADIO_GRE_CD), false);
						EnableWindow(GetDlgItem(hDlg, IDC_RADIO_LES_CD), false);
						EnableWindow(GetDlgItem(hDlg, IDC_DATETIMEPICKER_CD), false);

						bCreationGreater = false;
						bCreationLesser = false;
					}
					else
					{
						bCreationDateSelected = true;

						EnableWindow(GetDlgItem(hDlg, IDC_RADIO_GRE_CD), true);
						EnableWindow(GetDlgItem(hDlg, IDC_RADIO_LES_CD), true);
						EnableWindow(GetDlgItem(hDlg, IDC_DATETIMEPICKER_CD), true);

						bCreationGreater = true;
						bCreationLesser = true;

					}
					break;
				}
			case IDC_CHECK_MDDATE:
				{
					if(bModificationDateSelected)
					{
						bModificationDateSelected = false;

						EnableWindow(GetDlgItem(hDlg, IDC_RADIO_GRE_MD), false);
						EnableWindow(GetDlgItem(hDlg, IDC_RADIO_LES_MD), false);
						EnableWindow(GetDlgItem(hDlg, IDC_DATETIMEPICKER_MD), false);
					}
					else
					{
						bModificationDateSelected = true;

						EnableWindow(GetDlgItem(hDlg, IDC_RADIO_GRE_MD), true);
						EnableWindow(GetDlgItem(hDlg, IDC_RADIO_LES_MD), true);
						EnableWindow(GetDlgItem(hDlg, IDC_DATETIMEPICKER_MD), true);

					}
					break;
				}
			case IDC_RADIO_MAXSIZE:
				{
					if(bMaxFileSize)
					{
						bMaxFileSize = false;

						EnableWindow( GetDlgItem(hDlg, IDC_EDIT_MAXSIZE), false);
					}
					else
					{
						bMaxFileSize = true;
						bMinFileSize = false;
						EnableWindow( GetDlgItem(hDlg, IDC_EDIT_MAXSIZE), true);
						EnableWindow( GetDlgItem(hDlg, IDC_EDIT_MINSIZE), false);
					}
					break;
				}
			case IDC_RADIO_MINSIZE:
				{
					if(bMinFileSize)
					{
						bMinFileSize = false;
						EnableWindow( GetDlgItem(hDlg, IDC_EDIT_MINSIZE), false);

						//DateTime_
					}
					else
					{
						bMinFileSize = true;
						bMaxFileSize = false;
						EnableWindow( GetDlgItem(hDlg, IDC_EDIT_MINSIZE), true);
						EnableWindow( GetDlgItem(hDlg, IDC_EDIT_MAXSIZE), false);
					}
					break;
				}
			
			case IDCANCEL:
				{
					SendMessage(hDlg, WM_CLOSE, 0, 0);
					bReturnValue = TRUE;
					break;
				}
			case IDOK:
				{
					bSetCriteria = true;

					sffSearchCriteria = (SEARCHFILEFOLDER*)malloc(sizeof(SEARCHFILEFOLDER));

					sffSearchCriteria->nMaxSize = sffSearchCriteria->nMinSize = 0;
					sffSearchCriteria->bCreationDateGreater = sffSearchCriteria->bCreationDateLesser = sffSearchCriteria->bHiddenFiles = sffSearchCriteria->bModificationGreater = sffSearchCriteria->bModificationLesser = sffSearchCriteria->bReadOnlyFiles = false;
					/*sffSearchCriteria->strCreationDate = NULL;
					sffSearchCriteria->strModificationDate = NULL;*/

					if(bFileSizeSelected)
					{
						if(bMaxFileSize)
						{
							int len = SendMessage(GetDlgItem(hDlg, IDC_EDIT_MAXSIZE), WM_GETTEXTLENGTH, 0, 0);
							if(len > 0)
							{
								WCHAR* buffer = (WCHAR*)malloc((len+1)*sizeof(WCHAR));
								memset(buffer, L'\0', len+1);
								//SendMessage(GetDlgItem(hDlg, IDC_EDIT_MAXSIZE), WM_GETTEXT, (WPARAM)len, (LPARAM)buffer);
								GetWindowText(GetDlgItem(hDlg, IDC_EDIT_MAXSIZE), buffer, len+1);
								swscanf(buffer, L"%ld", &sffSearchCriteria->nMaxSize);
								free(buffer);
								buffer = NULL;
								sffSearchCriteria->bSearchForMaxSize = true;
								sffSearchCriteria->bSearchForMinSize = false;
							}
						}

						if(bMinFileSize)
						{
							int len = SendMessage(GetDlgItem(hDlg, IDC_EDIT_MINSIZE), WM_GETTEXTLENGTH, 0, 0);
							if(len > 0)
							{
								WCHAR* buffer = (WCHAR*)malloc((len+1)*sizeof(WCHAR));
								memset(buffer, L'\0', len+1);
								//SendMessage(GetDlgItem(hDlg, IDC_EDIT_MAXSIZE), WM_GETTEXT, (WPARAM)len, (LPARAM)buffer);
								GetWindowText(GetDlgItem(hDlg, IDC_EDIT_MINSIZE), buffer, len+1);
								swscanf(buffer, L"%ld", &sffSearchCriteria->nMinSize);
								free(buffer);
								buffer = NULL;
								sffSearchCriteria->bSearchForMinSize = true;
								sffSearchCriteria->bSearchForMaxSize = false;
							}
						}
						
					}

					if(bCreationDateSelected)
					{
						DateTime_GetSystemtime(GetDlgItem(hDlg, IDC_DATETIMEPICKER_CD), &sffSearchCriteria->systimeCreationDate);
						/*int len = SendMessage(GetDlgItem(hDlg, IDC_DATETIMEPICKER_CD), WM_GETTEXTLENGTH, 0, 0);
						sffSearchCriteria->strCreationDate = (WCHAR*)malloc((len+1)*sizeof(WCHAR));
						memset(sffSearchCriteria->strCreationDate, L'\0', len+1);
						GetWindowText(GetDlgItem(hDlg, IDC_DATETIMEPICKER_CD), sffSearchCriteria->strCreationDate, len+1);*/

						if(IsDlgButtonChecked(hDlg, IDC_RADIO_GRE_CD))
						{
							sffSearchCriteria->bCreationDateGreater = true;
						}
						else
						{
							sffSearchCriteria->bCreationDateLesser = true;
						}

						
												

					}

					if(bModificationDateSelected)
					{
						DateTime_GetSystemtime(GetDlgItem(hDlg, IDC_DATETIMEPICKER_MD), &sffSearchCriteria->systimeModificationDate);
						/*int len = SendMessage(GetDlgItem(hDlg, IDC_DATETIMEPICKER_MD), WM_GETTEXTLENGTH, 0, 0);
						sffSearchCriteria->strModificationDate = (WCHAR*)malloc((len+1)*sizeof(WCHAR));
						memset(sffSearchCriteria->strModificationDate, L'\0', len+1);
						GetWindowText(GetDlgItem(hDlg, IDC_DATETIMEPICKER_MD), sffSearchCriteria->strModificationDate, len+1);*/

						if(IsDlgButtonChecked(hDlg, IDC_RADIO_GRE_MD))
						{
							sffSearchCriteria->bModificationGreater = true;
						}
						else
						{
							sffSearchCriteria->bModificationLesser = true;
						}

					}



					if(IsDlgButtonChecked(hDlg, IDC_CHECK_RD))
					{
						sffSearchCriteria->bReadOnlyFiles = true;
					}

					if(IsDlgButtonChecked(hDlg, IDC_CHECK_HID))
					{
						sffSearchCriteria->bHiddenFiles = true;
					}

					int len = SendMessage(GetDlgItem(hDlg, IDC_EDIT_SEARLOC), WM_GETTEXTLENGTH, 0, 0);
					if(len > 0)
					{
						sffSearchCriteria->location = (WCHAR*)malloc((len+1)*sizeof(WCHAR));
						memset(sffSearchCriteria->location, L'\0', len+1);
						//SendMessage(GetDlgItem(hDlg, IDC_EDIT_MAXSIZE), WM_GETTEXT, (WPARAM)len, (LPARAM)buffer);
						GetWindowText(GetDlgItem(hDlg, IDC_EDIT_SEARLOC), sffSearchCriteria->location, len+1);
						//swscanf(buffer, L"%s", &);
						//free(buffer);
						//buffer = NULL;
					}

					FormChain();


					SendMessage(hDlg, WM_CLOSE, 0, 0);
					bReturnValue = TRUE;
					break;
				}
			}

			if (HIWORD(wParam) == BN_CLICKED)
			{
				if (LOWORD(wParam) == IDC_BTN_SL)
					BrowseForFolder(hDlg);
			}

			break;
		}
	case WM_CLOSE:
		{
			DestroyWindow(hDlg);
			bReturnValue = TRUE;
			break;
		}
	case WM_DESTROY:
		{
			PostQuitMessage(0);
			bReturnValue = TRUE;
			break;
		}
	}

  return bReturnValue;
}


int CheckForSearchCondition(FILEINFO* fileFolder)
{
	int nReturnValue = 1;
	if(sffSearchCriteria->bSearchForMaxSize)
	{
		if(fileFolder->dwFileSize > sffSearchCriteria->nMaxSize)
		{
			nReturnValue = 0;
			return nReturnValue;
		}
	}
	else if(sffSearchCriteria->bSearchForMinSize)
	{
		if(fileFolder->dwFileSize < sffSearchCriteria->nMinSize)
		{
			nReturnValue = 0;
			return nReturnValue;
		}
	}
	
	if(sffSearchCriteria->bHiddenFiles)
	{
		if(!(fileFolder->bHidden))
		{
			nReturnValue = 0;
			return nReturnValue;
		}
	}

	if(sffSearchCriteria->bReadOnlyFiles)
	{
		if(!(fileFolder->bReadonly))
		{
			nReturnValue = 0;
			return nReturnValue;
		}
	}

	if(sffSearchCriteria->bCreationDateGreater)
	{
		FILETIME tempTime;
		SystemTimeToFileTime(&sffSearchCriteria->systimeCreationDate, &tempTime);
		if(CompareFileTime(&fileFolder->ftCreationTime, &tempTime) == -1)
		{
			nReturnValue = 0;
			return nReturnValue;
		}
		//FileTimeToSystemTime(&fileFolder->ftCreationTime, &tempTime);

		//if(fileFolder->ftCreationTime
	}
	else if(sffSearchCriteria->bCreationDateLesser)
	{
		FILETIME tempTime;
		SystemTimeToFileTime(&sffSearchCriteria->systimeCreationDate, &tempTime);
		if(CompareFileTime(&fileFolder->ftCreationTime, &tempTime) == 1)
		{
			nReturnValue = 0;
			return nReturnValue;
		}
	}

	if(sffSearchCriteria->bModificationGreater)
	{
		FILETIME tempTime;
		SystemTimeToFileTime(&sffSearchCriteria->systimeCreationDate, &tempTime);
		if(CompareFileTime(&fileFolder->ftCreationTime, &tempTime) == -1)
		{
			nReturnValue = 0;
			return nReturnValue;
		}
	}
	else if(sffSearchCriteria->bModificationLesser)
	{
		FILETIME tempTime;
		SystemTimeToFileTime(&sffSearchCriteria->systimeModificationDate, &tempTime);
		if(CompareFileTime(&fileFolder->ftModificationTime, &tempTime) == 1)
		{
			nReturnValue = 0;
			return nReturnValue;
		}
	}


	return nReturnValue;
}

void FormTreeView(TV_INSERTSTRUCT* tvinsert, HTREEITEM Before, HWND hDlg, LSTFILEINSUBFOLDER* lstChildElements)
{
	HTREEITEM Parent;

	while(lstChildElements != NULL)
	{
		if(CheckForSearchCondition(lstChildElements->fileFolder) == 0)
		{
			lstChildElements = lstChildElements->nextFileFolder;

						continue;
		}
					        // handle of the before root
		tvinsert->hParent=Before;         // handle of the above data
		tvinsert->hInsertAfter=TVI_LAST;  // below parent
		tvinsert->item.pszText=lstChildElements->fileFolder->strFileName;
			 
		Parent=(HTREEITEM)SendDlgItemMessage(hDlg,IDC_TREE_RESULT,TVM_INSERTITEM,0,(LPARAM)tvinsert);

					

		if(!(lstChildElements->fileFolder->bIsFile))
		{
			//tvinsert->item.iImage=0;
			//tvinsert->item.iSelectedImage=1;
			FormTreeView(tvinsert, Parent, hDlg, lstChildElements->fileFolder->ChildElements);

		}
		else
		{
			//tvinsert->item.iImage=2;
			//tvinsert->item.iSelectedImage=2;
		}
					



		lstChildElements = lstChildElements->nextFileFolder;
	}


}

void FreeFileFolderInfo(FILEINFO* fileFolder)
{
	if(fileFolder != NULL)
	{
		free(fileFolder->location);
		free(fileFolder->strFileName);
		
		if(fileFolder->ChildElements != NULL)
		{
			FreeChildElements(fileFolder->ChildElements);
		}

	}
}

void FreeChildElements(LSTFILEINSUBFOLDER* ChildElements)
{
	//LSTFILEINSUBFOLDER* nextElement = NULL;
	if(ChildElements != NULL)
	{
		FreeFileFolderInfo(ChildElements->fileFolder);
		FreeChildElements(ChildElements->nextFileFolder);
		free(ChildElements);
	}

}

void FreeMemory()
{
	/*	SEARCHFILEFOLDER *sffSearchCriteria = NULL;
	FILEINFO* SearchFolderInfo = NULL;*/

	if(sffSearchCriteria != NULL)
	{
		if(sffSearchCriteria->location != NULL)
		{
			free(sffSearchCriteria->location);
			sffSearchCriteria->location = NULL;
		}

		free(sffSearchCriteria);
		sffSearchCriteria = NULL;


	}

	if(SearchFolderInfo != NULL)
	{
		free(SearchFolderInfo->location);
		free(SearchFolderInfo->strFileName);
		
		if(SearchFolderInfo->ChildElements != NULL)
		{
			FreeChildElements(SearchFolderInfo->ChildElements);
		}

	}
}




/*Function = ResultDialogproc
 *Description = this is the dialog procedure which will handle all the commands which are given to the Resultant Dialog ie the dialog having
 *				result of the serach.
 *Parameters = 
 *Return Value = 
 */
INT_PTR CALLBACK ResultDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	bool bReturnValue = FALSE;
	HIMAGELIST hImageList1, hImageList2; 
	HBITMAP hBitMap;
	TV_INSERTSTRUCT tvinsert;
	HTREEITEM Parent;           
	HTREEITEM Before;           
	HTREEITEM Root;  

	switch(uMsg)
	{
	case WM_INITDIALOG:
		{
			InitCommonControls();
			hTree=GetDlgItem(hDlg,IDC_TREE_RESULT);
			hImageList1=ImageList_Create(16,16,ILC_COLOR16,3,10);
			hBitMap=LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP2));
			ImageList_Add(hImageList1,hBitMap,NULL);
			DeleteObject(hBitMap);
			hImageList2=ImageList_Create(16,16,ILC_COLOR16,2,10);
			hBitMap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));
			ImageList_Add(hImageList2, hBitMap, NULL);
			DeleteObject(hBitMap);
			SendDlgItemMessage(hDlg,IDC_TREE_RESULT,TVM_SETIMAGELIST,0,(LPARAM)hImageList1);
			//SendDlgItemMessage(hDlg,IDC_TREE_RESULT,TVM_SETIMAGELIST,0,(LPARAM)hImageList2);

			
			tvinsert.hParent=NULL;
			tvinsert.hInsertAfter=TVI_ROOT;
			tvinsert.item.mask=TVIF_TEXT|TVIF_IMAGE|TVIF_SELECTEDIMAGE;
			//tvinsert.item.pszText=L"Parent";
			tvinsert.item.pszText=SearchFolderInfo->strFileName;
			tvinsert.item.iImage=0;
			tvinsert.item.iSelectedImage=1;
			//tvinsert.item.
			Parent=(HTREEITEM)SendDlgItemMessage(hDlg,IDC_TREE_RESULT,TVM_INSERTITEM,0,(LPARAM)&tvinsert);
			Root=Parent;
			Before=Parent; 

			if(SearchFolderInfo->ChildElements != NULL)
			{
				//FILEINFO* currentFile = NULL;
				LSTFILEINSUBFOLDER* lstChildElements = SearchFolderInfo->ChildElements;

				while(lstChildElements != NULL)
				{
					if(CheckForSearchCondition(lstChildElements->fileFolder) == 0)
					{
						lstChildElements = lstChildElements->nextFileFolder;
						continue;
					}

					                  // handle of the before root
					tvinsert.hParent=Before;         // handle of the above data
					tvinsert.hInsertAfter=TVI_LAST;  // below parent
					tvinsert.item.pszText=lstChildElements->fileFolder->strFileName;
			 
					Parent=(HTREEITEM)SendDlgItemMessage(hDlg,IDC_TREE_RESULT,TVM_INSERTITEM,0,(LPARAM)&tvinsert);

					

					if(!(lstChildElements->fileFolder->bIsFile))
					{
						//tvinsert.item.iImage=0;
						//tvinsert.item.iSelectedImage=1;
						FormTreeView(&tvinsert, Parent, hDlg, lstChildElements->fileFolder->ChildElements);

					}
					else
					{
						//tvinsert.item.iImage=2;
						//tvinsert.item.iSelectedImage=2;
					}
					



					lstChildElements = lstChildElements->nextFileFolder;
				}

			}




			

			break;
		}
	
	case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
			case IDOK:
				{
					FreeMemory();
					//if(sffSearchCriteria != NULL)
					//{
					//	/*if(sffSearchCriteria-> != NULL)
					//	{
					//		free(sffSearchCriteria->strCreationDate);
					//		sffSearchCriteria->strCreationDate = NULL;
					//	}

					//	if(sffSearchCriteria->strModificationDate != NULL)
					//	{
					//		free(sffSearchCriteria->strModificationDate);
					//		sffSearchCriteria->strModificationDate = NULL;
					//	}*/

					//	if(sffSearchCriteria->location != NULL)
					//	{
					//		free(sffSearchCriteria->location);
					//		sffSearchCriteria->location = NULL;
					//	}

					//	free(sffSearchCriteria);
					//	sffSearchCriteria = NULL;
					//}
					SendMessage(hDlg, WM_CLOSE, 0, 0);
					bReturnValue = TRUE;
					break;
				}
			}
			break;
		}
	case WM_CLOSE:
		{
			FreeMemory();
			DestroyWindow(hDlg);
			bReturnValue = TRUE;
			break;
		}
	case WM_DESTROY:
		{
			PostQuitMessage(0);
			bReturnValue = TRUE;
			break;
		}
	}

	return bReturnValue;

}

/*Function = CreateMainDialog
 *Description = This function will create the main dialog which will get the search criteria from the user.
 *Parameters = Instance for which this dialog is being created
 *Return Value = Handle of the dialog
 *
 */
HWND CreateMainDialog(HINSTANCE hInstance)
{
	HWND hDlg = NULL;
	hInst = hInstance;

	hDlg =  CreateDialogParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG_MAIN), 0, DialogProc, 0);
	//SetWindowText(hDlg, L"Search Criteria");
	//SetWindowPos(
	ShowWindow(hDlg, SW_NORMAL);

	return hDlg;
}

/*Function = CreateResultantDialog
 *Description = This function will create the resultant dialog which will show the searched files.
 *Parameters = Instance for which this dialog is being created
 *Return Value = Handle of the dialog
 *
 */
HWND CreateResultantDialog(HINSTANCE hInstance)
{

	HWND hDlg = NULL;
	hInst = hInstance;
	hDlg = CreateDialogParam(hInstance, MAKEINTRESOURCE(IDD_RESULT_DIALOG), 0, ResultDialogProc, 0);
	ShowWindow(hDlg, SW_NORMAL);


	return hDlg;

}

/*Function = RunApplicationForMainDialog
 *Description = This function will have the message loop for the main dialog
 *Parameter = -
 *Return Value = 
 *
 */
int RunApplicationForMainDialog()
{
	BOOL ret;
	MSG	 msg;

	while((ret = GetMessage(&msg, 0, 0, 0)) != 0 )
	{
		if(ret == -1)
			return -1;

		if(!IsDialogMessage(hMainDlg, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}


/*Function = RunApplicationForResultantDialog
 *Description = This function will have the message loop for the resultant dialog
 *Parameter = -
 *Return Value = 
 *
 */
int RunApplicationForResultantDialog()
{
	BOOL ret;
	MSG msg;

	while((ret = GetMessage(&msg, 0, 0, 0)) != 0)
	{
		if(-1 == ret)
		{
			return -1;
		}

		if(!IsDialogMessage(hResultDlg, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}




//int InitApplication(HINSTANCE hInstance)
//{
//	ATOM atRetVal = 0;
//
//	WndClass.style = CS_DBLCLKS;
//	WndClass.hInstance = hInstance;
//	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
//	WndClass.cbClsExtra = 0;
//	WndClass.cbWndExtra = 0;
//	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
//	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
//	WndClass.lpszClassName = strClassName;
//	WndClass.lpszMenuName = NULL;
//	WndClass.lpfnWndProc = WndProc;
//
//	atRetVal = RegisterClass(&WndClass);
//
//	if( NULL == atRetVal )
//	{
//		MessageBox(NULL, L"Error in registering the window class", L"Error", MB_OK|MB_ICONWARNING);
//		return -1;
//	}
//
//	return 0;
//}
//
//int InitInstance(HINSTANCE hInstance)
//{
//	
//	hResultWnd = CreateWindow(strClassName, L"Simple", WS_OVERLAPPEDWINDOW, 100, 100, 300,300,NULL, NULL, hInstance, NULL);
//
//	if( NULL == hResultWnd)
//	{
//		MessageBox(NULL, L"Window not created", L"Error", MB_OK|MB_ICONWARNING);
//		return -1;
//	}
//
//	ShowWindow(hResultWnd, SW_NORMAL);
//	UpdateWindow(hResultWnd);
//
//	return 0;
//}
//
//int RunApplication()
//{
//	MSG	 Msg;
//	while(GetMessage(&Msg, NULL, 0, 0))
//	{
//		DispatchMessage(&Msg);
//		TranslateMessage(&Msg);
//	}
//
//	return Msg.wParam;
//}
//
//LRESULT CALLBACK WndProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
//{
//
//
//	switch(uMessage)
//	{
//	
//
//
//	
//	
//			
//
//	case WM_DESTROY:
//		{
//			PostQuitMessage(0);
//			break;
//		}
//	default:
//		{
//			return DefWindowProc(hWnd, uMessage, wParam, lParam);
//		}
//
//	}
//
//	return 0;
//}


