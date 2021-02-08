#ifndef PLAYER_H
#define PLAYER_H
#pragma once

#include "CHMAT.h"
#include <random>


class Player
{
public:
	int id;
	int x, y;
	int w, h;
	int s;
	int c, bg;
	bool moving;
	CHMAT <int>* GameScreen;
public:
	Player(int id, int w, int h, int x, int y, int s, int c, int bg, CHMAT<int>* screen);
	~Player();

public:
	void draw(int c);
	COORDINATES move();
	COORDINATES move(bool left, bool right, bool up, bool down);

private:
	MOVES random_move();
	std::random_device rd;
};



#endif //!PLAYER_H

