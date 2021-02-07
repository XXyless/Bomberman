#pragma once
#include "Types.h"
#include "Colors.h"
#include "Messages.h"


_ID			IDI_ICON_1			= 1;
_ID			START_BUTTON		= 3000;
_ID			PAUSE_BUTTON		= 3001;

HINSTANCE	hInst;													//	current instance
HWND		hWnd, hEdit, hInfo, HButton1, HButton3, Hmainbmp;		//	parent window

LPCTSTR		lpszAppName			= "PARALLEL PROGRAMMING";
LPCTSTR		lpszTitle			= "BOMBERMAN";


_SIZE		WINDOW_WIDTH		= 800;
_SIZE		WINDOW_HEIGHT		= 600;

_KEY		PRESSED_KEY;

_BOOL		GAME_STARTED		= false;

constexpr	_SIZE		PLAYER_COUNT		= 4;

constexpr int			FRAME_COUNT = 60;


auto PLAYER_DIRECTION_LEFT		= false;
auto PLAYER_DIRECTION_RIGHT		= false;
auto PLAYER_DIRECTION_UP		= false;
auto PLAYER_DIRECTION_DOWN		= false;

int PLAYER_SPEED				=	1;

_SIZE PLAYER_WIDTH				=	3;
_SIZE PLAYER_WIDTH				=	3;