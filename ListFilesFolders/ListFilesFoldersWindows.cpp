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

SEARCHFILEFOLDER *sffSearchCriteria = NULL;

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

					sffSearchCriteria->nMaxSize = sffSearchCriteria->nMinSize = -1;
					sffSearchCriteria->bCreationDateGreater = sffSearchCriteria->bCreationDateLesser = sffSearchCriteria->bHiddenFiles = sffSearchCriteria->bModificationGreater = sffSearchCriteria->bModificationLesser = sffSearchCriteria->bReadOnlyFiles = false;
					sffSearchCriteria->strCreationDate = NULL;
					sffSearchCriteria->strModificationDate = NULL;

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
								swscanf(buffer, L"%d", &sffSearchCriteria->nMaxSize);
								free(buffer);
								buffer = NULL;
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
								swscanf(buffer, L"%d", &sffSearchCriteria->nMinSize);
								free(buffer);
								buffer = NULL;
							}
						}
						
					}

					if(bCreationDateSelected)
					{
						int len = SendMessage(GetDlgItem(hDlg, IDC_DATETIMEPICKER_CD), WM_GETTEXTLENGTH, 0, 0);
						sffSearchCriteria->strCreationDate = (WCHAR*)malloc((len+1)*sizeof(WCHAR));
						memset(sffSearchCriteria->strCreationDate, L'\0', len+1);
						GetWindowText(GetDlgItem(hDlg, IDC_DATETIMEPICKER_CD), sffSearchCriteria->strCreationDate, len+1);

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
						int len = SendMessage(GetDlgItem(hDlg, IDC_DATETIMEPICKER_MD), WM_GETTEXTLENGTH, 0, 0);
						sffSearchCriteria->strModificationDate = (WCHAR*)malloc((len+1)*sizeof(WCHAR));
						memset(sffSearchCriteria->strModificationDate, L'\0', len+1);
						GetWindowText(GetDlgItem(hDlg, IDC_DATETIMEPICKER_MD), sffSearchCriteria->strModificationDate, len+1);

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

					SendMessage(hDlg, WM_CLOSE, 0, 0);
					bReturnValue = TRUE;
					break;
				}
			}

			if (HIWORD(wParam) == BN_CLICKED)
			{
				if (LOWORD(wParam) == IDC_BTN_SL)
					BrowseForFolder();
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


void BrowseForFolder()
{

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

	switch(uMsg)
	{
	//case WM_INITDIALOG:
	//	{
	//		InitCommonControls();
	//		/*HWND hTree = GetDlgItem(hDlg, IDC_TREE_RESULT);
	//		HIMAGELIST hImageList=ImageList_Create(16,16,ILC_COLOR16,2,10);
	//		HBITMAP hBitMap=LoadBitmap(GetWindowLong(hDlg, GWL_HINSTANCE) ,MAKEINTRESOURCE(IDC_TREE_RESULT));
 //  ImageList_Add(hImageList,hBitMap,NULL);
 //  DeleteObject(hBitMap);
 //  SendDlgItemMessage(hDlg,IDC_TREE_RESULT,
 //      TVM_SETIMAGELIST,0,(LPARAM)hImageList); 
 //  tvinsert.hParent=NULL;
 //  tvinsert.hInsertAfter=TVI_ROOT;
 //  tvinsert.item.mask=TVIF_TEXT|TVIF_IMAGE|TVIF_SELECTEDIMAGE;
 //  tvinsert.item.pszText="Parent";
 //  tvinsert.item.iImage=0;
 //  tvinsert.item.iSelectedImage=1;*/
	//		break;
	//	}
	case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
			case IDOK:
				{
					if(sffSearchCriteria != NULL)
					{
						if(sffSearchCriteria->strCreationDate != NULL)
						{
							free(sffSearchCriteria->strCreationDate);
							sffSearchCriteria->strCreationDate = NULL;
						}

						if(sffSearchCriteria->strModificationDate != NULL)
						{
							free(sffSearchCriteria->strModificationDate);
							sffSearchCriteria->strModificationDate = NULL;
						}

						free(sffSearchCriteria);
						sffSearchCriteria = NULL;
					}
					SendMessage(hDlg, WM_CLOSE, 0, 0);
					bReturnValue = TRUE;
					break;
				}
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

/*Function = CreateMainDialog
 *Description = This function will create the main dialog which will get the search criteria from the user.
 *Parameters = Instance for which this dialog is being created
 *Return Value = Handle of the dialog
 *
 */
HWND CreateMainDialog(HINSTANCE hInstance)
{
	HWND hDlg = NULL;

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


