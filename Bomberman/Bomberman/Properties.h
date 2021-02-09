#ifndef PROPERTIES_H
#define PROPERTIES_H
#pragma once
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
#include "Types.h"
#include "Colors.h"

_ID				IDI_ICON_1					=	1;
_ID				START_BUTTON				=	3000;
_ID				PAUSE_BUTTON				=	3001;

HINSTANCE		hInst;													//	current instance
HWND			hWnd, hEdit, hInfo, HButton1, HButton3, Hmainbmp;		//	parent window

LPCTSTR			lpszAppName					=	"PARALLEL PROGRAMMING";
LPCTSTR			lpszTitle					=	"BOMBERMAN";


_SIZE			WINDOW_WIDTH				=	800;
_SIZE			WINDOW_HEIGHT				=	600;

_SIZE			GAME_WIDTH					=	WINDOW_WIDTH - 50;
_SIZE			GAME_HEIGHT					=	WINDOW_HEIGHT - 100;


_KEY			PRESSED_KEY					=	0;
_KEY			RELEASED_KEY				=	0;


_BOOL			GAME_STARTED				=	false;

constexpr _SIZE	PLAYER_COUNT				=	2;

constexpr int	FRAME_COUNT					=	30;



_BOOL			PLAYER_DIRECTION_LEFT		=	false;
_BOOL			PLAYER_DIRECTION_RIGHT		=	false;
_BOOL			PLAYER_DIRECTION_UP			=	false;
_BOOL			PLAYER_DIRECTION_DOWN		=	false;

int				PLAYER_SPEED				=	2;

_SIZE			PLAYER_WIDTH				=	20;
_SIZE			PLAYER_HEIGHT				=	20;

COLOR			BG_COLOR					=	BLACK;

int FIRST_LEFT	= VK_LEFT;
int FIRST_RIGHT = VK_RIGHT;
int FIRST_UP	= VK_UP;
int FIRST_DOWN	= VK_DOWN;

#endif //!PROPERTIES_H