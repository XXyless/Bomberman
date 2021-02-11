#ifndef BOMB_H
#define BOMB_H
#pragma once

#include "Types.h"
#include "CHMAT.h"

class Bomb
{
public:
	_BOOL bomb = false;
	int id;
	COORDINATES bomb_coord;
	_SIZE w, h;
	COLOR c;
public:
	Bomb(_ID id,int x, int y, _SIZE w, _SIZE h, COLOR c);

};

#endif //!BOMB_H