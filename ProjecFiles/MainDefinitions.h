#pragma once

#define OnHelpClicked	1
#define OnAboutClicked	2
#define OnCreateFileClicked	3
#define OnDeleteFileClicked	4
#define OnExitClicked	5
#define OnButtonReadClicked	7
#define OnCreateDirectoryClicked	8
#define OnDeleteDirectoryClicked	9
#define OnCClicked	10
#define OnDClicked	11
#define OnListboxClicked 12

int ItemIndex;
int ItemSize;

bool IsCurDir;

std::wstring CurrentDir;
std::wstring CurrentPath;
std::wstring Cur;
std::wstring FileName;

HWND hEditControl;
HWND hStaticDirControl;
HWND hListboxControl;
HWND hListbox1Control;

LRESULT CALLBACK MainProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
WNDCLASS NewWindowClass(HBRUSH BGColor, HCURSOR Cursor, HINSTANCE hInst, HICON Icon, LPCWSTR Name, WNDPROC Procedure);

void MainWndAddMenus(HWND hWnd);
void MainWndAddWidgets(HWND hWnd);
void GetFiles(std::wstring path);

int CreateFile(HWND hWnd);
void DeleteFile(HWND hWnd);
void CreateDir(HWND hWnd);
void DeleteDir(HWND hWnd);

void GetFileName();
void GetCur(int curIndex, int length);
