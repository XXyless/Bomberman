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
#include "Bomb.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow);
VOID CALLBACK start_game();


CHMAT<int>								GameScreen(GAME_WIDTH, GAME_HEIGHT, BG_COLOR);
HANDLE hTimerQueue = nullptr;


HANDLE									player_threads[PLAYER_COUNT];
HANDLE									bomb_threads[PLAYER_COUNT];
std::vector <Player*>					players(PLAYER_COUNT);
std::vector <Bomb*>						bombs(PLAYER_COUNT);
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

DWORD WINAPI PlayerThread(LPVOID param) {
	Player* myself = reinterpret_cast<Player*>(param);

	std::vector <std::vector<int>> next_line;
	while (GAME_STARTED) {

		if (PRESSED_KEY == VK_SPACE && myself->id == 0)
		{
			bombs[0]->bomb_coord.x = myself->coordinates.x;
			bombs[0]->bomb_coord.y = myself->coordinates.y;
			bombs[0]->bomb = true;
		}
		if (PRESSED_KEY == VK_SHIFT && myself->id == 1)
		{
			bombs[1]->bomb_coord.x = myself->coordinates.x;
			bombs[1]->bomb_coord.y = myself->coordinates.y;
			bombs[1]->bomb = true;
		}

		drawBox(PLAYER_WIDTH, PLAYER_HEIGHT, myself->coordinates.x, myself->coordinates.y, BG_COLOR);
		//if (DRAWING_DONE) {
			//MOVEMENT_DONE[my_id]	=	false;
		if (myself->moves.left) {
			if (myself->coordinates.x - myself->s > 1) { myself->coordinates.x -= myself->s; }
			else { myself->coordinates.x = 2; }
		}

		else if (myself->moves.right) {
			if (myself->coordinates.x + myself->s + myself->w < myself->area_width) { myself->coordinates.x += myself->s; }
			else { myself->coordinates.x = myself->area_width - myself->w - 1; }
		}

		if (myself->moves.up) {
			if (myself->coordinates.y + myself->s + myself->h < myself->area_height - 2) { myself->coordinates.y += myself->s; }
			else { myself->coordinates.y = myself->area_height - myself->h - 2; }
		}

		else if (myself->moves.down) {
			if (myself->coordinates.y - myself->s > 2) { myself->coordinates.y -= myself->s; }
			else { myself->coordinates.y = 2; }
		}

		myself->move();
			//MOVEMENT_DONE[my_id] = true;
		//}

		drawBox(PLAYER_WIDTH, PLAYER_HEIGHT, myself->coordinates.x, myself->coordinates.y, myself->c);

		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	return 0;
}

DWORD WINAPI BombThread(LPVOID param) {
	Bomb* myself = reinterpret_cast<Bomb*>(param);

	while (GAME_STARTED)
	{
		if (myself->bomb) {
			drawBox(myself->w, myself->h, myself->bomb_coord.x + 4, myself->bomb_coord.y + 4, myself->c);
			std::this_thread::sleep_for(std::chrono::milliseconds(200));
		}
		
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
	int block_x, block_y, current_wall = 0;
	
	while (current_wall < WALL_COUNT)
	{
		for (block_y = 50; block_y < GAME_HEIGHT; block_y += 100)
		{
			for (block_x = 50; block_x < GAME_WIDTH; block_x += 100)
			{
				walls.push_back({ block_x, block_y, false });
				drawBox(WALL_WIDTH, WALL_HEIGHT, block_x, block_y, WALL_COLOR);
				current_wall++;
			}
		}
	}

	current_wall = 0;
	while (current_wall < BRICK_COUNT )
	{
		for (block_y = 102; block_y < 450; block_y += 100)
		{
			for (block_x = 52; block_x < 650; block_x += 100)
			{
				bricks.push_back({ block_x, block_y, true });
				drawBox(BRICK_WIDTH, BRICK_HEIGHT, block_x, block_y, BRICK_COLOR);
				current_wall++;
			}
		}

		for (block_y = 52; block_y < 500; block_y += 50)
		{
			for (block_x = 102; block_x < 550; block_x += 100)
			{

				bricks.push_back({ block_x, block_y, true });
				drawBox(BRICK_WIDTH, BRICK_HEIGHT, block_x, block_y, BRICK_COLOR);
				current_wall++;
			}
		}

		for (block_y = 3; block_y < 450; block_y += 50)
		{
			bricks.push_back({ 3, block_y, true });
			drawBox(BRICK_WIDTH , BRICK_HEIGHT , 3, block_y, BRICK_COLOR);
			current_wall++;
		}
		for (block_y = 103; block_y < GAME_HEIGHT; block_y += 50)
		{
			bricks.push_back({ 603, block_y, true });
			drawBox(BRICK_WIDTH, BRICK_HEIGHT, 603, block_y, BRICK_COLOR);
			current_wall++;
		}

		for (block_x = 53; block_x < 550; block_x += 50)
		{
			bricks.push_back({ block_x, 3, true });
			drawBox(BRICK_WIDTH, BRICK_HEIGHT, block_x, 3, BRICK_COLOR);
			current_wall++;
		}
		for (block_x = 103; block_x < GAME_WIDTH - 50; block_x += 50)
		{
			bricks.push_back({ block_x, 503, true });
			drawBox(BRICK_WIDTH, BRICK_HEIGHT, block_x, 503, BRICK_COLOR);
			current_wall++;
		}
	}
	SetWindowText(hEdit, "1. player moves(GREEN): arrow keys   1. player use bombs: space button     2. player moves(WHITE): W,A,S,D       2. player use bombs: Shift button ");

	int x, y; COLOR c;
	for (size_t current_player = 0; current_player < PLAYER_COUNT; current_player++) {
		// player_moves[current_player].left = false; player_moves[current_player].right = false; player_moves[current_player].up = false; player_moves[current_player].down = false;

 		if		(current_player == 0)	{ x = 5 ;y = GAME_HEIGHT - PLAYER_HEIGHT - 5; c = GREEN; }
		else if (current_player == 1)	{ x = GAME_WIDTH - PLAYER_WIDTH - 5; y = 5; c = WHITE; }

		players[current_player] = new Player(current_player, x, y, PLAYER_WIDTH, PLAYER_HEIGHT, PLAYER_SPEED, c, GAME_WIDTH, GAME_HEIGHT);
		bombs[current_player] = new Bomb(current_player, x, y, BOMB_WIDTH, BOMB_HEIGHT, BOMB_COLOR);
		// player_threads[current_player] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)PlayerThread, NULL, 0, &ThreadID);
		player_threads[current_player] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)PlayerThread, reinterpret_cast<void*>(players[current_player]), 0, &ThreadID);
		bomb_threads[current_player] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)BombThread, reinterpret_cast<void*>(bombs[current_player]), 0, &ThreadID);
	}
	
	// draw_boxes()
	CreateTimerQueueTimer(&hTimer, hTimerQueue, (WAITORTIMERCALLBACK)Update, NULL, 0, 1000 / FRAME_COUNT, 0);
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg)
	{
	case WM_CREATE:
		hEdit = CreateWindowEx(WS_EX_CLIENTEDGE, "Edit", "", WS_CHILD | WS_VISIBLE |
			ES_AUTOVSCROLL | ES_MULTILINE | ES_WANTRETURN | WS_VSCROLL, 700, 50, 275, 100, hWnd, NULL, hInst, NULL);
		Hmainbmp = CreateWindowEx(0, "STATIC", "", WS_CHILD | WS_VISIBLE | SS_BITMAP | WS_THICKFRAME, 1, 50, GAME_WIDTH, GAME_HEIGHT, hWnd, NULL, hInst, NULL);
		start_game();
		break;

	case WM_KEYDOWN:
		PRESSED_KEY = (int)wParam;
		if		(PRESSED_KEY == VK_LEFT)	{ players[0]->moves.left	= true; }
		else if (PRESSED_KEY == VK_RIGHT)	{ players[0]->moves.right	= true; }
		else if (PRESSED_KEY == VK_UP)		{ players[0]->moves.up		= true; }
		else if (PRESSED_KEY == VK_DOWN)	{ players[0]->moves.down	= true; }

		if		(PRESSED_KEY == 'A' or PRESSED_KEY == 'a') { players[1]->moves.left		= true; }
		else if (PRESSED_KEY == 'D' or PRESSED_KEY == 'd') { players[1]->moves.right	= true; }
		else if (PRESSED_KEY == 'W' or PRESSED_KEY == 'w') { players[1]->moves.up		= true; }
		else if (PRESSED_KEY == 'S' or PRESSED_KEY == 's') { players[1]->moves.down		= true; }

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