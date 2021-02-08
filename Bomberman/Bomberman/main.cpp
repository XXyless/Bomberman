#include <windows.h>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <thread>
#include <chrono>
#include <vector>
#include <string>
#include <memory>
#include <random>
#include <array>
#include "CHMAT.h"
#include "Properties.h"
#include "Player.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow);
VOID CALLBACK start_game();


CHMAT<int>								GameScreen(GAME_WIDTH, GAME_HEIGHT, BG_COLOR);
HANDLE hTimerQueue = nullptr;


HANDLE									player_threads[PLAYER_COUNT];
std::vector <Player*>					players(PLAYER_COUNT);
// std::vector <MOVES>						player_moves(PLAYER_COUNT);

bool MOVEMENT_DONE[] = { true, true, true, true, true, true, true, true, true, true };
bool DRAWING_DONE = true;

VOID* Update(LPVOID param) {
	/*
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
	*/
	// KUTULAR MUTULAR DRAW
	SendMessage(Hmainbmp, STM_SETIMAGE, 0, (LPARAM)GameScreen.HBitmap);
	// DRAWING_DONE	=	true;
	return 0;
}

void drawBox(int box_width, int box_height, int box_x, int box_y, int color) {
	for (auto x = 0; x < box_width; ++x)
		for (auto y = 0; y < box_height; ++y)
			GameScreen(box_x + x, box_y + y) = color;
}



DWORD WINAPI PlayerThread(LPVOID param) {
	Player* myself = reinterpret_cast<Player*>(param);
	
	while (GAME_STARTED) {
		if (DRAWING_DONE) {
			drawBox(PLAYER_WIDTH, PLAYER_HEIGHT, myself->coordinates.x, myself->coordinates.y, myself->c);
			//MOVEMENT_DONE[my_id]	=	false;
			myself->move();
			//MOVEMENT_DONE[my_id] = true;
		}
		drawBox(PLAYER_WIDTH, PLAYER_HEIGHT, myself->coordinates.x, myself->coordinates.y, BG_COLOR);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	return 0;
}


VOID CALLBACK start_game() {
	DWORD ThreadID;
	HANDLE hTimer = NULL;
	hTimerQueue = CreateTimerQueue();
	if (NULL == hTimerQueue) { return; }

	GAME_STARTED = true;

	// SendMessage(Hmainbmp, STM_SETIMAGE, 0, (LPARAM)GameScreen.HBitmap);

	int x, y;
	COLOR c;
	for (size_t current_player = 0; current_player < PLAYER_COUNT; current_player++) {
		// player_moves[current_player].left = false; player_moves[current_player].right = false; player_moves[current_player].up = false; player_moves[current_player].down = false;

		if		(current_player == 0)	{ x = 10 ;y = GAME_HEIGHT - PLAYER_HEIGHT - 10; c = GREEN; }
		else if (current_player == 1)	{ x = GAME_WIDTH - PLAYER_WIDTH - 10; y = 10; c = WHITE; }

		players.push_back(new Player(current_player, x, y, PLAYER_WIDTH, PLAYER_HEIGHT, PLAYER_SPEED, c, GAME_WIDTH, GAME_HEIGHT));
		// player_threads[current_player] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)PlayerThread, NULL, 0, &ThreadID);
		player_threads[current_player] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)PlayerThread, reinterpret_cast<void*>(&players[current_player]), 0, &ThreadID);
		break;
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
		if		(PRESSED_KEY == VK_LEFT)	{ players[0]->moves.left	= true; }
		else if (PRESSED_KEY == VK_RIGHT)	{ players[0]->moves.right	= true; }
		else if (PRESSED_KEY == VK_UP)		{ players[0]->moves.up		= true; }
		else if (PRESSED_KEY == VK_DOWN)	{ players[0]->moves.down	= true; }

		break;
	case WM_KEYUP:
		RELEASED_KEY = (int)wParam;
		if		(RELEASED_KEY == VK_LEFT)	{ players[0]->moves.left	= false; }
		else if (RELEASED_KEY == VK_RIGHT)	{ players[0]->moves.right	= false; }
		else if (RELEASED_KEY == VK_UP)		{ players[0]->moves.up		= false; }
		else if (RELEASED_KEY == VK_DOWN)	{ players[0]->moves.down	= false; }
		break;
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

*/


/*
* 
bool BOX_DIRECTION_LEFT = false, BOX_DIRECTION_RIGHT = false, BOX_DIRECTION_UP = false, BOX_DIRECTION_DOWN = false;
int BOX_SPEED = 1;
int BOX_WIDTH = 20;
int BOX_HEIGHT = 20;
int BOX_LOCATION_X = 150, BOX_LOCATION_Y = 150;
COLOR BOX_COLOR = ORANGE;
*/