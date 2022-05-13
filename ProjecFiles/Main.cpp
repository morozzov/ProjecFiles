//#include <Windows.h>
#include "MainDefinitions.h"

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow) {
	WNDCLASS MainClass = NewWindowClass((HBRUSH)COLOR_WINDOW, LoadCursor(NULL, IDC_ARROW), hInst, LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1)), L"MainWndClass", MainProcedure);

	if (!RegisterClassW(&MainClass)) {
		return -1;
	}
	MSG MainMessage = { 0 };
	CreateWindow(L"MainWndClass", L"Explorer", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 10, 10, 850, 700, NULL, NULL, NULL, NULL);
	while (GetMessage(&MainMessage, NULL, NULL, NULL)) {
		TranslateMessage(&MainMessage);
		DispatchMessage(&MainMessage);
	}
	return 0;
}

WNDCLASS NewWindowClass(HBRUSH BGColor, HCURSOR Cursor, HINSTANCE hInst, HICON Icon, LPCWSTR Name, WNDPROC Procedure) {
	WNDCLASS NWC = { 0 };

	NWC.hCursor = Cursor;
	NWC.hIcon = Icon;
	NWC.hInstance = hInst;
	NWC.lpszClassName = Name;
	NWC.hbrBackground = BGColor;
	NWC.lpfnWndProc = Procedure;

	return NWC;
}

LRESULT CALLBACK MainProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
	switch (msg) {
	case WM_COMMAND:
		if (LOWORD(wp) == OnListboxClicked) {
			if (HIWORD(wp) == LBN_DBLCLK) {

				try {
					ItemIndex = (int)SendMessage(hListboxControl, LB_GETCURSEL, (WPARAM)0, (LPARAM)0);

					if (ItemIndex != LB_ERR) {
						ItemSize = (int)SendMessage(hListboxControl, LB_GETTEXTLEN, (WPARAM)ItemIndex, 0);
						GetCur(ItemIndex, ItemSize);

						if (IsCurDir) {

							if (wcscmp(Cur.c_str(), L".") == 0) {
								CurrentPath = CurrentDir.substr(0, 4);
							}
							else if (wcscmp(Cur.c_str(), L"..") == 0) {
								CurrentPath = CurrentDir;
								CurrentPath.erase(CurrentPath.size() - 2, 2);
								CurrentPath.erase(CurrentPath.find_last_of(L"/") + 1, CurrentPath.size() - CurrentPath.find_last_of(L"/") - 1);
							}
							else {
								CurrentPath = CurrentDir.c_str();
								CurrentPath += Cur.c_str();
								CurrentPath += L"//";
							}

							GetFiles(CurrentPath);
						}

					}
				}
				catch (const std::exception&) {
				}
			}
		}
		switch (wp) {
		case OnHelpClicked:
			MessageBoxA(hWnd, "Contact: https://t.me/moroz_zov", "Help", MB_OK);
			break;

		case OnAboutClicked:
			MessageBoxA(hWnd, "I'm a file explorer)", "About", MB_OK);
			break;

		case OnCreateFileClicked:
			GetFileName();

			CreateFile(hWnd);

			GetFiles(CurrentDir);
			break;

		case OnDeleteFileClicked:
			ItemIndex = (int)SendMessage(hListboxControl, LB_GETCURSEL, (WPARAM)0, (LPARAM)0);

			if (ItemIndex != LB_ERR) {
				ItemSize = (int)SendMessage(hListboxControl, LB_GETTEXTLEN, (WPARAM)ItemIndex, 0);
				GetCur(ItemIndex, ItemSize);

				DeleteFile(hWnd);

				GetFiles(CurrentDir);
			}
			break;

		case OnCreateDirectoryClicked:
			GetFileName();

			CreateDir(hWnd);

			GetFiles(CurrentDir);
			break;

		case OnDeleteDirectoryClicked:
			GetFileName();

			ItemIndex = (int)SendMessage(hListboxControl, LB_GETCURSEL, (WPARAM)0, (LPARAM)0);

			if (ItemIndex != LB_ERR) {
				ItemSize = (int)SendMessage(hListboxControl, LB_GETTEXTLEN, (WPARAM)ItemIndex, 0);
				GetCur(ItemIndex, ItemSize);

				DeleteDir(hWnd);

				GetFiles(CurrentDir);
			}
			break;

		case OnExitClicked:
			PostQuitMessage(0);
			break;

		case OnCClicked:
			CurrentDir = L"C://";
			GetFiles(CurrentDir);
			break;
		case OnDClicked:
			CurrentDir = L"D://";
			GetFiles(CurrentDir);
			break;
		case OnEClicked:
			CurrentDir = L"E://";
			GetFiles(CurrentDir);
			break;
		case OnFClicked:
			CurrentDir = L"F://";
			GetFiles(CurrentDir);
			break;
		case OnGClicked:
			CurrentDir = L"G://";
			GetFiles(CurrentDir);
			break;
		case OnHClicked:
			CurrentDir = L"H://";
			GetFiles(CurrentDir);
			break;
		case OnIClicked:
			CurrentDir = L"I://";
			GetFiles(CurrentDir);
			break;

		case OnButtonReadClicked:
			MainWndAddMenus(hWnd);
			GetFiles(CurrentDir);
			break;

		default:break;
		}
		break;

	case WM_CREATE:
		CurrentDir = L"D://";

		MainWndAddMenus(hWnd);
		MainWndAddWidgets(hWnd);

		GetFiles(CurrentDir);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_SIZE:
	{
		Resize(hWnd);
		break;
	}

	default:		return DefWindowProc(hWnd, msg, wp, lp);
	}
}

void MainWndAddMenus(HWND hWnd) {
	HMENU RootMenu = CreateMenu();
	HMENU SubMenu = CreateMenu();

	HMENU Root1Menu = CreateMenu();
	HMENU Sub1Menu = CreateMenu();

	int n;
	DWORD dr = GetLogicalDrives();
	UINT_PTR comm;

	for (int i = 0; i < 26; i++) {
		n = ((dr >> i) & 0x00000001);
		if (n == 1) {
			std::wstring disk = L"Open ";
			disk += char(65 + i);
			disk += L": directory";

			switch (char(65 + i)) 	{
			case 'C':
				comm = OnCClicked;
				break;
			case 'D':
				comm = OnDClicked;
				break;
			case 'E':
				comm = OnEClicked;
				break;
			case 'F':
				comm = OnFClicked;
				break;
			case 'G':
				comm = OnGClicked;
				break;
			case 'H':
				comm = OnHClicked;
				break;
			case 'I':
				comm = OnIClicked;
				break;
			default:
				comm = OnCClicked;
				break;
			}
			AppendMenu(Sub1Menu, MF_STRING, comm, disk.c_str());
		}
	}

	AppendMenu(Sub1Menu, MF_SEPARATOR, NULL, NULL);
	AppendMenu(Sub1Menu, MF_STRING, OnCreateDirectoryClicked, L"Create new directory");
	AppendMenu(Sub1Menu, MF_STRING, OnDeleteDirectoryClicked, L"Delete selected directory");
	AppendMenu(Sub1Menu, MF_SEPARATOR, NULL, NULL);
	AppendMenu(Sub1Menu, MF_STRING, OnExitClicked, L"Exit");
	AppendMenu(RootMenu, MF_POPUP, (UINT_PTR)Sub1Menu, L"Directory");

	AppendMenu(SubMenu, MF_STRING, OnCreateFileClicked, L"Create new file");
	AppendMenu(SubMenu, MF_STRING, OnDeleteFileClicked, L"Delete selected file");

	AppendMenu(RootMenu, MF_POPUP, (UINT_PTR)SubMenu, L"File");

	AppendMenu(RootMenu, MF_STRING, OnHelpClicked, L"Help");
	AppendMenu(RootMenu, MF_STRING, OnAboutClicked, L"About");

	SetMenu(hWnd, RootMenu);
}

void MainWndAddWidgets(HWND hWnd) {
	RECT rcClient;

	GetClientRect(hWnd, &rcClient);

	hStaticDirControl = CreateWindowA("static", "fff", WS_VISIBLE | WS_CHILD, 0, 0, rcClient.right - 80, 20, hWnd, NULL, NULL, NULL);

	hButton = CreateWindowA("button", "Refresh", WS_VISIBLE | WS_CHILD | ES_CENTER, rcClient.right - 80, 0, 80, 20, hWnd, (HMENU)OnButtonReadClicked, NULL, NULL);

	hListboxControl = CreateWindowA("listbox", "Edit text", WS_VISIBLE | LBS_NOTIFY | WS_CHILD | WS_VSCROLL, 0, 20, rcClient.right, rcClient.bottom - 50, hWnd, (HMENU)OnListboxClicked, NULL, NULL);

	hEditControl = CreateWindowA("edit", "file.txt", WS_VISIBLE | WS_CHILD | ES_MULTILINE | WS_VSCROLL, 5, rcClient.bottom - 25, rcClient.right, 20, hWnd, NULL, NULL, NULL);
}

void GetFiles(std::wstring path) {
	SendMessage(hListboxControl, LB_RESETCONTENT, 0, 0);
	SendMessage(hListbox1Control, LB_RESETCONTENT, 0, 0);

	SetWindowText(hStaticDirControl, (LPCWSTR)path.c_str());

	CurrentDir = path;

	WIN32_FIND_DATA findFileData{};
	auto handle = FindFirstFile((path + L"*").c_str(), &findFileData);
	std::wstring fN;

	do {
		if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			fN = L"[_] ";
			fN += findFileData.cFileName;
			SendMessage(hListboxControl, LB_ADDSTRING, 0, (LPARAM)fN.c_str());
		}
		else {
			fN = L"     ";
			fN += findFileData.cFileName;
			SendMessage(hListboxControl, LB_ADDSTRING, 0, (LPARAM)(fN.c_str()));
		}

	} while (FindNextFile(handle, &findFileData));

	FindClose(handle);
};

void CreateDir(HWND hWnd) {
	std::wstring name = CurrentDir + FileName;

	CreateDirectory(name.c_str(), NULL);

	MessageBox(hWnd, L"Directory created", L"Done", MB_ICONINFORMATION);
}

void DeleteDir(HWND hWnd) {
	std::wstring name = CurrentDir + Cur;

	RemoveDirectory(name.c_str());

	MessageBox(hWnd, L"Directory deleted", L"Done", MB_ICONINFORMATION);
}

int CreateFile(HWND hWnd) {
	std::wstring name = CurrentDir + FileName;

	HANDLE hFile = CreateFile(
		name.c_str(),
		GENERIC_WRITE,
		FILE_SHARE_READ,
		NULL,
		CREATE_NEW,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (hFile == INVALID_HANDLE_VALUE) {
		return 2;
	}

	std::string strText = "Hello World!";

	DWORD bytesWritten;

	WriteFile(
		hFile,
		strText.c_str(),
		strText.size(),
		&bytesWritten,
		nullptr);

	CloseHandle(hFile);

	MessageBox(hWnd, L"File created", L"Done", MB_ICONINFORMATION);
}

void DeleteFile(HWND hWnd) {
	std::wstring name = CurrentDir + Cur;

	DeleteFile(name.c_str());

	MessageBox(hWnd, L"File deleted", L"Done", MB_ICONINFORMATION);
}

void GetFileName() {
	int length = SendMessageW(hEditControl, WM_GETTEXTLENGTH, 0, 0);

	if (length > 0) {
		std::vector<WCHAR> buf(length + 1);
		SendMessageW(hEditControl,
			WM_GETTEXT,
			buf.size(),
			(LPARAM)reinterpret_cast<LPCWSTR>(&buf[0]));
		FileName = &buf[0];
	}
}

void GetCur(int curIndex, int length) {
	if (length > 0) {
		std::vector<WCHAR> buf(length + 1);
		SendMessageW(hListboxControl,
			LB_GETTEXT,
			(WPARAM)curIndex,
			(LPARAM)reinterpret_cast<LPCWSTR>(&buf[0]));
		Cur = &buf[0];
		if (Cur[0] == ' ') {
			Cur = Cur.erase(0, 5);
			IsCurDir = false;
		}
		else {
			Cur = Cur.erase(0, 4);
			IsCurDir = true;
		}
	}
}

void Resize(HWND hWnd) {
	RECT rcClient;

	GetClientRect(hWnd, &rcClient);

	SetWindowPos(hStaticDirControl, NULL, 0, 0, rcClient.right - 80, 20, SWP_NOZORDER);
	SetWindowPos(hButton, NULL, rcClient.right - 80, 0, 80, 20, SWP_NOZORDER);
	SetWindowPos(hListboxControl, NULL, 0, 20, rcClient.right, rcClient.bottom - 50, SWP_NOZORDER);
	SetWindowPos(hEditControl, NULL, 5, rcClient.bottom - 25, rcClient.right, 20, SWP_NOZORDER);
}
