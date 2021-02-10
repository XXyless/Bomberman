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
std::vector <block>						bricks;
std::vector <block>						walls;

bool MOVEMENT_DONE[] = { true, true, true, true, true, true, true, true, true, true };
bool DRAWING_DONE = true;

void drawBox(int box_width, int box_height, int box_x, int box_y, int color) {
	for (auto x = 0; x < box_width; ++x)
		for (auto y = 0; y < box_height; ++y)
			if (x < GAME_WIDTH and y < GAME_HEIGHT)
				GameScreen(box_x + x, box_y + y) = color;
}

bool road_open(COORDINATES current_point, int direction) {
	bool in_range;
	int x = current_point.x, y = current_point.y; 
	if (direction == LEFT) { 

	}
	else if (direction == RIGHT){
		
	}
	else if (direction == UP){

	}
	else{

	}
}	

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
	for (int j = 50; j < GAME_HEIGHT; j += 100)
	{
		for (int i = 50; i < GAME_WIDTH; i += 100)
		{
			drawBox(50, 50, i, j, GREY);
		}
	}
	
	for (int j = 102; j < 449; j += 100)
	{
		for (int i = 52; i < 649; i += 100)
		{
			drawBox(46, 46, i, j, BROWN);
		}
	} 

	for (int j = 52; j < 499; j += 50)
	{
		for (int i = 102; i < 549; i += 100)
		{
			drawBox(46, 46, i, j, BROWN);
		}
	}

	SendMessage(Hmainbmp, STM_SETIMAGE, 0, (LPARAM)GameScreen.HBitmap);
	// DRAWING_DONE	=	true;
	return 0;
}


DWORD WINAPI PlayerThread(LPVOID param) {
	Player* myself = reinterpret_cast<Player*>(param);

	while (GAME_STARTED) {
		drawBox(PLAYER_WIDTH, PLAYER_HEIGHT, myself->coordinates.x, myself->coordinates.y, BG_COLOR);
		//if (DRAWING_DONE) {
			//MOVEMENT_DONE[my_id]	=	false;
		myself->move();
			//MOVEMENT_DONE[my_id] = true;
		//}

		drawBox(PLAYER_WIDTH, PLAYER_HEIGHT, myself->coordinates.x, myself->coordinates.y, myself->c);
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
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
	int block_x, block_y;
	for (size_t current_wall = 0; current_wall < WALL_COUNT; current_wall++){
		walls.push_back({ {}, false });
	}

	int x, y; COLOR c;
	for (size_t current_player = 0; current_player < PLAYER_COUNT; current_player++) {
		// player_moves[current_player].left = false; player_moves[current_player].right = false; player_moves[current_player].up = false; player_moves[current_player].down = false;

 		if		(current_player == 0)	{ x = 5 ;y = GAME_HEIGHT - PLAYER_HEIGHT - 5; c = GREEN; }
		else if (current_player == 1)	{ x = GAME_WIDTH - PLAYER_WIDTH - 5; y = 5; c = WHITE; }

		players[current_player] = new Player(current_player, x, y, PLAYER_WIDTH, PLAYER_HEIGHT, PLAYER_SPEED, c, GAME_WIDTH, GAME_HEIGHT);
		// player_threads[current_player] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)PlayerThread, NULL, 0, &ThreadID);
		player_threads[current_player] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)PlayerThread, reinterpret_cast<void*>(players[current_player]), 0, &ThreadID);
	}
	
	// draw_boxes()
	CreateTimerQueueTimer(&hTimer, hTimerQueue, (WAITORTIMERCALLBACK)Update, NULL, 0, 1000 / FRAME_COUNT, 0);
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg)
	{
	case WM_CREATE:
		Hmainbmp = CreateWindowEx(0, "STATIC", "", WS_CHILD | WS_VISIBLE | SS_BITMAP | WS_THICKFRAME, 1, 50, GAME_WIDTH, GAME_HEIGHT, hWnd, NULL, hInst, NULL);
		start_game();
		break;

	case WM_KEYDOWN:
		PRESSED_KEY = (int)wParam;
		if		(PRESSED_KEY == VK_LEFT)	{ if (road_open(players[0]->coordinates, LEFT))		{ players[0]->moves.left = true; } }
		else if (PRESSED_KEY == VK_RIGHT)	{ if (road_open(players[0]->coordinates, RIGHT))	{ players[0]->moves.right = true; } }
		else if (PRESSED_KEY == VK_UP)		{ if (road_open(players[0]->coordinates, UP))		{ players[0]->moves.up = true; } }
		else if (PRESSED_KEY == VK_DOWN)	{ if (road_open(players[0]->coordinates, DOWN ))	{ players[0]->moves.down = true; } }

		if		(PRESSED_KEY == 'A' or PRESSED_KEY == 'a') { if (road_open(players[1]->coordinates, LEFT))	{ players[1]->moves.left = true; } }
		else if (PRESSED_KEY == 'D' or PRESSED_KEY == 'd') { if (road_open(players[1]->coordinates, RIGHT)) { players[1]->moves.right = true; } }
		else if (PRESSED_KEY == 'W' or PRESSED_KEY == 'w') { if (road_open(players[1]->coordinates, UP))	{ players[1]->moves.up = true; } }
		else if (PRESSED_KEY == 'S' or PRESSED_KEY == 's') { if (road_open(players[1]->coordinates, DOWN))	{ players[1]->moves.down = true; } }
		break;
	case WM_KEYUP:
		RELEASED_KEY = (int)wParam;
		if		(RELEASED_KEY == VK_LEFT)	{ players[0]->moves.left = false; }
		else if (RELEASED_KEY == VK_RIGHT)	{ players[0]->moves.right = false; }
		else if (RELEASED_KEY == VK_UP)		{ players[0]->moves.up = false; }
		else if (RELEASED_KEY == VK_DOWN)	{ players[0]->moves.down = false; }

		if		(RELEASED_KEY == 'A' or RELEASED_KEY == 'a') { players[1]->moves.left = false; }
		else if (RELEASED_KEY == 'D' or RELEASED_KEY == 'd') { players[1]->moves.right = false; }
		else if (RELEASED_KEY == 'W' or RELEASED_KEY == 'w') { players[1]->moves.up = false; }
		else if (RELEASED_KEY == 'S' or RELEASED_KEY == 's') { players[1]->moves.down = false; }
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