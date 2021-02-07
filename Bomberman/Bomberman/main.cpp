#include <windows.h>
#include <algorithm>
#include <iostream>
#include <numeric>
#include "chmat.h"
#include <thread>
#include <chrono>
#include <vector>
#include <string>
#include <memory>
#include <random>
#include <array>
#include "Properties.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow);
VOID CALLBACK start_game();


CHMAT<int>								GameScreen(WINDOW_WIDTH - 10, WINDOW_HEIGHT - 10, GREEN);
HANDLE hTimerQueue = nullptr;


HANDLE									player_threads[PLAYER_COUNT];
std::vector <std::vector <int>>			player_coordinates(PLAYER_COUNT, std::vector <int>(2, 0));
int										player_id[PLAYER_COUNT] = { 0, 1, 2, 3};

bool MOVEMENT_DONE[]	= { true, true, true, true, true, true, true, true, true, true };
bool DRAWING_DONE		= false;

VOID* Update(LPVOID param) {
	DRAWING_DONE	= false;
	bool wait		= true;
	while (wait) {
		for (size_t current_player = 0; current_player < PLAYER_COUNT; current_player++) {
			if (not MOVEMENT_DONE[current_player]) {
				wait = true;
				break;
			}
			wait = false;
		}
	}

	// KUTULAR MUTULAR DRAW
	SendMessage(Hmainbmp, STM_SETIMAGE, 0, (LPARAM)GameScreen.HBitmap);
	DRAWING_DONE	=	true;
}


DWORD WINAPI PlayerThread(LPVOID param) {
	int* tm = (int*)param;
	int my_id = *tm;

	int x, y, speed = PLAYER_SPEED;
	if (my_id == 0) {
		x = 1;
		y = 1;
	}
	else if(my_id == 1){

	}
	else if (my_id == 2) {

	}
	else {

	}

	while (GAME_STARTED) {
		if (PLAYER_DIRECTION_LEFT) {
			if (x - PLAYER_SPEED > 1) x -= PLAYER_SPEED;
			else x = 1;
		}
		else if (PLAYER_DIRECTION_RIGHT) {
			if (x + PLAYER_SPEED < GameScreen.x - PLAYER_WIDTH) x += PLAYER_SPEED;
			else x = GameScreen.x - PLAYER_WIDTH;
		}

		if (PLAYER_DIRECTION_UP) {
			if (y+ PLAYER_SPEED < m.y - PLAYER_HEIGHT) y+= PLAYER_SPEED;
			else y= m.y - PLAYER_HEIGHT;
		}
		else if (PLAYER_DIRECTION_DOWN) {
			if (y- PLAYER_SPEED > 0) y-= PLAYER_SPEED;
			else y= 1;
		}
	}
}


VOID CALLBACK start_game() {
	DWORD ThreadID;
	HANDLE hTimer = NULL;
	hTimerQueue = CreateTimerQueue();
	if (NULL == hTimerQueue)
	{
		return;
	}
	// GAME_STARTED = true;
	
	// SendMessage(Hmainbmp, STM_SETIMAGE, 0, (LPARAM)GameScreen.HBitmap);

	
	for (size_t current_player = 0; current_player < PLAYER_COUNT; current_player++){
		player_threads[current_player] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)PlayerThread, &player_id[current_player], 0, &ThreadID);
	}


	// draw_boxes()
	CreateTimerQueueTimer(&hTimer, hTimerQueue, (WAITORTIMERCALLBACK)Update, NULL, 0, 1000 / FRAME_COUNT, 0);
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg)
	{
	case WM_CREATE:
		Hmainbmp = CreateWindowEx(0, "STATIC", "", WS_CHILD | WS_VISIBLE | SS_BITMAP | WS_THICKFRAME, 1, 23, WINDOW_WIDTH, WINDOW_HEIGHT, hWnd, nullptr, hInst, nullptr);
		start_game();
		break;
	case WM_KEYDOWN:
		PRESSED_KEY = (int)wParam;
		if (PRESSED_KEY == 'p' or PRESSED_KEY == 'P') {
			
		}
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		
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


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) {
	MSG      msg;
	WNDCLASSEX wc;

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON_1));
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
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
		WINDOW_WIDTH, WINDOW_HEIGHT,
		nullptr,
		nullptr,
		hInstance,
		nullptr
	);

	if (!hWnd) return(FALSE);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}


/*
	 * ------- *
	  #-#-#-#-# 
	 -#-#-#-#-#-
	 -#-#-#-#-#-
	 -#-#-#-#-#- 
	 -#-#-#-#-#-
	 -#-#-#-#-#-
	  #-#-#-#-#
	 * ------- *
	 GİTHUBA PUSHLUYORUM BU AKŞAMLIK SALALIM MI? OLUR REİS YARIN DEVAM EDERİZ - TAMAMDIR
	 
*/