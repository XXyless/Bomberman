#include <windows.h>  
#include "chmat.h"
#include <vector>
#include <string>

#define IDI_ICON1		101
#define IDC_OPEN		3000
#define IDC_BUTTON		3001
#define IDC_COLORTEXT	3002
#define IDC_BGCOLORTEXT 3003
#define IDC_NUMBERTEXT	3004
#define IDC_VALUEX		3005
#define IDC_VALUEY		3006

HINSTANCE hInst;   // current instance
HWND     hWnd, hEdit, HButton, HNColorText, HBgColorText, HNumberText, HValueX, HValueY, Hmainbmp;    //parent window

LPCTSTR lpszAppName = "GEN32";
LPCTSTR lpszTitle = "HW2 - IZZET - OGUZ - BERKE";
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) {
	MSG      msg;
	WNDCLASSEX wc;
	HMENU MainMenu, FileMenu;
	MainMenu = CreateMenu();
	FileMenu = CreatePopupMenu();
	AppendMenu(FileMenu, MF_STRING, IDC_OPEN, "Open");
	AppendMenu(MainMenu, MF_POPUP, (UINT_PTR)FileMenu, "FILE");

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	wc.lpszMenuName = lpszAppName;
	wc.lpszClassName = lpszAppName;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hIconSm = (HICON)LoadImage(hInstance, lpszAppName, IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);

	if (!RegisterClassEx(&wc)) return(FALSE);

	hInst = hInstance;
	hWnd = CreateWindowEx(0, lpszAppName,
		lpszTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		700, 450,
		NULL,
		MainMenu,
		hInstance,
		NULL
	);


	if (!hWnd) return(FALSE);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return(msg.wParam);
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{

	case WM_CREATE:
	{
		hEdit = CreateWindowEx(WS_EX_CLIENTEDGE, "Edit", "", WS_CHILD | WS_VISIBLE |
			ES_AUTOVSCROLL | ES_MULTILINE | ES_WANTRETURN | WS_VSCROLL, 350, 50, 275, 100, hWnd, NULL, hInst, NULL);

		Hmainbmp = CreateWindowEx(NULL, "STATIC", "", WS_CHILD | WS_VISIBLE | SS_BITMAP | WS_THICKFRAME, 20, 42, 300, 300, hWnd, NULL, hInst, NULL);

		HButton = CreateWindowEx(NULL, "BUTTON", "GO", WS_CHILD | WS_VISIBLE | SS_CENTER, 550, 233, 80, 30, hWnd, (HMENU)IDC_BUTTON, hInst, NULL);

		CreateWindow(TEXT("Static"), TEXT("Num Color: "), WS_VISIBLE | WS_CHILD, 350, 200, 100, 20, hWnd, NULL, NULL, NULL);
		CreateWindow(TEXT("Static"), TEXT("BG Color: "), WS_VISIBLE | WS_CHILD, 350, 225, 100, 20, hWnd, NULL, NULL, NULL);
		CreateWindow(TEXT("Static"), TEXT("Number: "), WS_VISIBLE | WS_CHILD, 350, 250, 100, 20, hWnd, NULL, NULL, NULL);
		CreateWindow(TEXT("Static"), TEXT("Value X: "), WS_VISIBLE | WS_CHILD, 350, 275, 100, 20, hWnd, NULL, NULL, NULL);
		CreateWindow(TEXT("Static"), TEXT("Value Y: "), WS_VISIBLE | WS_CHILD, 350, 300, 100, 20, hWnd, NULL, NULL, NULL);

		HNColorText = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("Edit"), TEXT(""), WS_CHILD | WS_VISIBLE, 430, 200, 100,
			20, hWnd, (HMENU)IDC_COLORTEXT, NULL, NULL);
		HBgColorText = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("Edit"), TEXT(""), WS_CHILD | WS_VISIBLE, 430, 225, 100,
			20, hWnd, (HMENU)IDC_BGCOLORTEXT, NULL, NULL);
		HNumberText = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("Edit"), TEXT(""), WS_CHILD | WS_VISIBLE, 430, 250, 100,
			20, hWnd, (HMENU)IDC_NUMBERTEXT, NULL, NULL);
		HValueX = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("Edit"), TEXT(""), WS_CHILD | WS_VISIBLE, 430, 275, 100,
			20, hWnd, (HMENU)IDC_VALUEX, NULL, NULL);
		HValueY = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("Edit"), TEXT(""), WS_CHILD | WS_VISIBLE, 430, 300, 100,
			20, hWnd, (HMENU)IDC_VALUEY, NULL, NULL);

	}
	break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_OPEN:
		{
			OPENFILENAME ofn;
			char szFile[260];
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hWnd;
			ofn.lpstrFile = szFile;
			//
			// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
			// use the contents of szFile to initialize itself.
			//
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = sizeof(szFile);
			ofn.lpstrFilter = "All\0*.*\0Text\0*.TXT\0";
			ofn.nFilterIndex = 1;
			ofn.lpstrFileTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			GetOpenFileName(&ofn);
		}
		break;
		case IDC_BUTTON:
		{

			int x, y, color, bgcolor, number, i;
			char control[30];

			GetWindowText(HNColorText, control, 30);
			if (strlen(control) == 0)
			{
				SetWindowText(hEdit, "Error!! Please enter number color.");
				return NULL;
			}
			color = std::strtoul(control, nullptr, 16);

			GetWindowText(HBgColorText, control, 30);
			if (strlen(control) == 0)
			{
				SetWindowText(hEdit, "Error!! Please enter background color.");
				return NULL;
			}
			bgcolor = std::strtoul(control, nullptr, 16);

			GetWindowText(HNumberText, control, 30);

			if (strlen(control) == 0)
			{
				SetWindowText(hEdit, "Error!! Please enter number.");
				return NULL;
			}

			for (i = 0; i < strlen(control); i++)
			{
				if (isalpha(control[i]))
				{
					SetWindowText(hEdit, "Error!! Wrong type.");
					return NULL;
				}
			}

			number = std::strtoul(control, nullptr, 10);
			if (number > 9 || number < 0)
			{
				SetWindowText(hEdit, "Error!! Please enter 0-9 number.");
				return NULL;
			}

			GetWindowText(HValueX, control, 30);

			if (strlen(control) == 0)
			{
				SetWindowText(hEdit, "Error!! Please enter value of X.");
				return NULL;
			}

			for (i = 0; i < strlen(control); i++)
			{
				if (isalpha(control[i]))
				{
					SetWindowText(hEdit, "Error!! Wrong type.");
					return NULL;
				}
			}

			x = std::strtoul(control, nullptr, 10);

			GetWindowText(HValueY, control, 30);

			if (strlen(control) == 0)
			{
				SetWindowText(hEdit, "Error!! Please enter value of Y.");
				return NULL;
			}

			for (i = 0; i < strlen(control); i++)
			{
				if (isalpha(control[i]))
				{
					SetWindowText(hEdit, "Error!! Wrong type.");
					return NULL;
				}
			}

			y = std::strtoul(control, nullptr, 10);

			CHMAT<int> m(300, 300);

			if ((x < 1) || (x > m.x - 12) || (y < 1) || (y > m.y - 12))
			{
				SetWindowText(hEdit, "Error!! Out of Boundaries.");
				return NULL;
			}
			m = bgcolor;
			m.printnumber(x, y, number, color, bgcolor);
			SendMessage(Hmainbmp, STM_SETIMAGE, 0, (LPARAM)m.HBitmap);
			SetWindowText(hEdit, "\tNumber generated !!");

		}
		break;

		default:
			break;
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return(DefWindowProc(hWnd, uMsg, wParam, lParam));
	}

	return(0L);
}