#ifndef PLAYER_H
#define PLAYER_H
#pragma once

#include "CHMAT.h"
#include <random>
#include "Types.h"

class Player
{
public:
	int id;
	COORDINATES coordinates;
	_SIZE w, h;
	int s;
	COLOR c;
	bool moving;
	int area_width, area_height;
	MOVES moves;
public:
	Player(_ID id, int x, int y, _SIZE w, _SIZE h, int s, COLOR c, _SIZE area_width, _SIZE area_height);
	~Player();

public:
	
	COORDINATES move();
	// COORDINATES move(bool left, bool right, bool up, bool down);

private:
	MOVES random_move();
	std::random_device rd;
};



#endif //!PLAYER_H

