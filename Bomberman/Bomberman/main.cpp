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
CHMAT<int>								a(GAME_WIDTH, GAME_HEIGHT, BG_COLOR);
HANDLE hTimerQueue = nullptr;

HANDLE									player_threads[PLAYER_COUNT];
HANDLE									bomb_threads;
std::vector <Player*>					players(PLAYER_COUNT);
COORDINATES								bombs[PLAYER_COUNT];
std::vector <block>						walls;
_BOOL									BOMB_COUNT[PLAYER_COUNT] = { true, true };

void drawBox(int box_width, int box_height, int box_x, int box_y, int color) {
	for (auto x = 0; x < box_width; ++x)
		for (auto y = 0; y < box_height; ++y)
			if (x < GAME_WIDTH and y < GAME_HEIGHT)
				GameScreen(box_x + x, box_y + y) = color;
}

void deleteBlock(int x, int y) {
	int current_pixel = GameScreen.data[y * GameScreen.x + x];

	if (current_pixel != BG_COLOR and current_pixel != WALL_COLOR) {
		GameScreen(x, y) = BG_COLOR;
		if (x < GameScreen.x) { deleteBlock(x + 1, y); }
		if (x > 1) { deleteBlock(x - 1, y); }
		if (y < GameScreen.y) { deleteBlock(x, y + 1); }
		if (y > 1) { deleteBlock(x, y - 1); }
	}
}

VOID* Update(LPVOID param) {

	SendMessage(Hmainbmp, STM_SETIMAGE, 0, (LPARAM)GameScreen.HBitmap);
	return 0;
}

DWORD WINAPI PlayerThread(LPVOID param) {
	Player* myself = reinterpret_cast<Player*>(param);

	std::vector <std::vector<int>> next_line;
	while (GAME_STARTED) {
		
		for (int i = 0; i < PLAYER_WIDTH; i++)
		{
			//Player 1
			if (GameScreen.data[(myself->coordinates.y + i) * GameScreen.x + (myself->coordinates.x + 31)] != BG_COLOR && myself->id == 0)
			{
				myself->moves.right = false;
			}
			else if (GameScreen.data[(myself->coordinates.y + i) * GameScreen.x + (myself->coordinates.x - 2)] != BG_COLOR && myself->id == 0)
			{
				myself->moves.left = false;
			}
			else if (GameScreen.data[(myself->coordinates.y + 31) * GameScreen.x + (myself->coordinates.x + i)] != BG_COLOR && myself->id == 0)
			{
				myself->moves.up = false;
			}
			else if (GameScreen.data[(myself->coordinates.y - 2) * GameScreen.x + (myself->coordinates.x + i)] != BG_COLOR && myself->id == 0)
			{
				myself->moves.down = false;
			}
			//Player 2
			else if (GameScreen.data[(myself->coordinates.y + i) * GameScreen.x + (myself->coordinates.x + 31)] != BG_COLOR && myself->id == 1)
			{
				myself->moves.right = false;
			}
			else if (GameScreen.data[(myself->coordinates.y + i) * GameScreen.x + (myself->coordinates.x - 3)] != BG_COLOR && myself->id == 1)
			{
				myself->moves.left = false;
			}
			else if (GameScreen.data[(myself->coordinates.y + 31) * GameScreen.x + (myself->coordinates.x + i)] != BG_COLOR && myself->id == 1)
			{
				myself->moves.up = false;
			}
			else if (GameScreen.data[(myself->coordinates.y - 2) * GameScreen.x + (myself->coordinates.x + i)] != BG_COLOR && myself->id == 1)
			{
				myself->moves.down = false;
			}

		}
		
		drawBox(PLAYER_WIDTH, PLAYER_HEIGHT, myself->coordinates.x, myself->coordinates.y, BG_COLOR);

		myself->move();

		drawBox(PLAYER_WIDTH, PLAYER_HEIGHT, myself->coordinates.x, myself->coordinates.y, myself->c);

		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	return 0;
}

DWORD WINAPI BombThread(LPVOID param) {
	
	int	x, y, second = 0;
	COORDINATES* my_cord = (COORDINATES*)param;
	x = my_cord->x ;
	y = my_cord->y ;

	while (GAME_STARTED && second < 3)
	{

		drawBox(BOMB_WIDTH, BOMB_HEIGHT,my_cord->x + 5, my_cord->y + 5, BOMB_COLOR);
		second ++;
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		//deleteBlock(my_cord->x + 30, my_cord->y );
		drawBox(BOMB_WIDTH, BOMB_HEIGHT, my_cord->x + 5, my_cord->y + 5, BG_COLOR);

	}
	if (my_cord->x + 41 < GAME_WIDTH && my_cord->y + 41 < GAME_HEIGHT && my_cord->x - 41 > 1 && my_cord->y - 41 > 1)
	{

		for (int i = 0; i < BOMB_WIDTH + 30; i++)
		{
			//Player 1
			if (GameScreen.data[(my_cord->y + i) * GameScreen.x + (my_cord->x + 40)] == WHITE)
			{
				SetWindowText(hEdit, "PLAYER 1 (GREEN) WIN !!!");
				return 0;
			}
			else if (GameScreen.data[(my_cord->y + i) * GameScreen.x + (my_cord->x - 40)] == WHITE)
			{
				SetWindowText(hEdit, "PLAYER 1 (GREEN) WIN !!!");
				return 0;
			}
			else if (GameScreen.data[(my_cord->y + 40) * GameScreen.x + (my_cord->x + i)] == WHITE)
			{
				SetWindowText(hEdit, "PLAYER 1 (GREEN) WIN !!!");
				return 0;
			}
			else if (GameScreen.data[(my_cord->y - 40) * GameScreen.x + (my_cord->x + i)] == WHITE)
			{
				SetWindowText(hEdit, "PLAYER 1 (GREEN) WIN !!!");
				return 0;
			}
			else if (GameScreen.data[(my_cord->y + i) * GameScreen.x + (my_cord->x + 40)] == GREEN)
			{
				SetWindowText(hEdit, "PLAYER 2 (WHITE) WIN !!!");
				return 0;
			}
			else if (GameScreen.data[(my_cord->y + i) * GameScreen.x + (my_cord->x - 40)] == GREEN)
			{
				SetWindowText(hEdit, "PLAYER 2 (WHITE) WIN !!!");
				return 0;
			}
			else if (GameScreen.data[(my_cord->y + 40) * GameScreen.x + (my_cord->x + i)] == GREEN)
			{
				SetWindowText(hEdit, "PLAYER 2 (WHITE) WIN !!!");
				return 0;
			}
			else if (GameScreen.data[(my_cord->y - 40) * GameScreen.x + (my_cord->x + i)] == GREEN)
			{
				SetWindowText(hEdit, "PLAYER 2 (WHITE) WIN !!!");
				return 0;
			}

		}
		drawBox(BOMB_WIDTH + 20, BOMB_HEIGHT + 20, my_cord->x, my_cord->y, BOMB_COLOR);
		drawBox(BOMB_WIDTH + 20, BOMB_HEIGHT + 20, my_cord->x - BOMB_WIDTH, my_cord->y - BOMB_HEIGHT, BOMB_COLOR);
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		drawBox(BOMB_WIDTH + 20, BOMB_HEIGHT + 20, my_cord->x, my_cord->y, BG_COLOR);
		drawBox(BOMB_WIDTH + 20, BOMB_HEIGHT + 20, my_cord->x - BOMB_WIDTH, my_cord->y - BOMB_HEIGHT, BG_COLOR);
	}
	
	BOMB_COUNT[0] = true;
	BOMB_COUNT[1] = true;
	
	return 0;
}

VOID CALLBACK start_game() {
	DWORD ThreadID;
	HANDLE hTimer = NULL;
	hTimerQueue = CreateTimerQueue();
	if (NULL == hTimerQueue) { return; }
	
	GAME_STARTED = true;

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
	
		for (block_y = 102; block_y < 450; block_y += 100)
		{
			for (block_x = 52; block_x < 650; block_x += 100)
			{
				walls.push_back({ block_x, block_y, true });
				drawBox(BRICK_WIDTH, BRICK_HEIGHT, block_x, block_y, BRICK_COLOR);
				current_wall++;
			}
		}

		for (block_y = 52; block_y < 500; block_y += 50)
		{
			for (block_x = 102; block_x < 550; block_x += 100)
			{

				walls.push_back({ block_x, block_y, true });
				drawBox(BRICK_WIDTH, BRICK_HEIGHT, block_x, block_y, BRICK_COLOR);
				current_wall++;
			}
		}

		for (block_y = 3; block_y < 450; block_y += 50)
		{
			walls.push_back({ 3, block_y, true });
			drawBox(BRICK_WIDTH , BRICK_HEIGHT , 3, block_y, BRICK_COLOR);
			current_wall++;
		}
		for (block_y = 103; block_y < GAME_HEIGHT; block_y += 50)
		{
			walls.push_back({ 603, block_y, true });
			drawBox(BRICK_WIDTH, BRICK_HEIGHT, 603, block_y, BRICK_COLOR);
			current_wall++;
		}

		for (block_x = 53; block_x < 550; block_x += 50)
		{
			walls.push_back({ block_x, 3, true });
			drawBox(BRICK_WIDTH, BRICK_HEIGHT, block_x, 3, BRICK_COLOR);
			current_wall++;
		}
		for (block_x = 103; block_x < GAME_WIDTH - 50; block_x += 50)
		{
			walls.push_back({ block_x, 503, true });
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
		player_threads[current_player] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)PlayerThread, reinterpret_cast<void*>(players[current_player]), 0, &ThreadID);
		
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

		if (PRESSED_KEY == VK_SPACE && BOMB_COUNT[0])
		{
			bombs[0] = { players[0]->coordinates.x, players[0]->coordinates.y };
			BOMB_COUNT[0] = false;
			bomb_threads = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)BombThread, (LPVOID)&bombs[0], 0, 0);
			
		}
		if (PRESSED_KEY == VK_SHIFT && BOMB_COUNT[1])
		{
			BOMB_COUNT[1] = false;
			bombs[1] = { players[1]->coordinates.x, players[1]->coordinates.y };
			bomb_threads = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)BombThread, (LPVOID)&bombs[1], 0, 0);
		}

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
	 * ---------
	  #-#-#-#-#-
	 -#-#-#-#-#-
	 -#-#-#-#-#-
	 -#-#-#-#-#-
	 -#-#-#-#-#-
	 -#-#-#-#-#-
	 -#-#-#-#-#
	 --------- *

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