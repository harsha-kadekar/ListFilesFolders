/*This is the main entry point of the application.
 *Application will get the serach criteria for the files and based on that finds the files and then displays that to the user
 *Search criteria may be hidden files, read only files, file size, etc.
 *
 *Developer = Harsha kadekar
 *
 */

#include <Windows.h>

#include "ListFilesFoldersApp.h"
#include "ListFilesFoldersWindows.h"
#include "resource.h"


HWND hMainDlg = NULL;							//Handle for the main dialog where it asks for search criteria from the user.
bool bSetCriteria = false;						//To know whether user pressed OK or Cancel in main dialog, If OK then resultant dialog, else close application

HWND hResultDlg = NULL;							//Handle for the dialog which will show the resultant files information.

//HWND hResultWnd = NULL;
//WNDCLASS WndClass;
//LPWSTR strClassName = L"windclass";

/*FunctionName = WinMain
 *Description = This is the main entry of the application. This is where it all begins.
 *Parameters = 
 *ReturnValue = true if success else false
 */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstace, LPSTR lpCmdLine, int nShowCmd)
{
	int nRetVal = 0;

	hMainDlg = CreateMainDialog(hInstance);
	
	
	nRetVal = RunApplicationForMainDialog();
	
	if(bSetCriteria)
	{
		hResultDlg = CreateResultantDialog(hInstance);
		nRetVal = RunApplicationForResultantDialog();

		/*nRetVal = InitApplication(hInstance);
		nRetVal = InitInstance(hInstance);
		nRetVal = RunApplication();*/
	}

	return nRetVal;
}

