#ifndef LISTFILESFOLDERSWINDOWS_H
#define LISTFILESFOLDERSWINDOWS_H

INT_PTR CALLBACK DialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
HWND CreateMainDialog(HINSTANCE hInstance);
int RunApplicationForMainDialog();

INT_PTR CALLBACK ResultDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
HWND CreateResultantDialog(HINSTANCE hInstance);
int RunApplicationForResultantDialog();

//LRESULT CALLBACK WndProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam);
//int InitApplication(HINSTANCE hInstance);
//int InitInstance(HINSTANCE hInstance);
//int RunApplication();


#endif