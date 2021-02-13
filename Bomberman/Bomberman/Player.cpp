#include "Player.h"




Player::Player(_ID id, int x, int y, _SIZE w, _SIZE h, int s, COLOR c, _SIZE area_width, _SIZE area_height) {
	this->id = id;

	this->coordinates.x = x, this->coordinates.y = y;
	this->w = w, this->h = h;
	this->c = c;
	this->s = s;
	this->area_width = area_width, this->area_height = area_height;
	this->moves.left = false; this->moves.right = false; this->moves.up = false; this->moves.down = false;
}

Player::~Player() {}

/*
COORDINATES Player::move() {
	this->moving = true;
	MOVES movement = this->random_move();
	return this->move(movement.left, movement.right, movement.up, movement.down);
}
*/

COORDINATES Player::move() {
	
	if (this->moves.left) {
		if (this->coordinates.x - this->s > 1 )	{ this->coordinates.x -= this->s; }
		else	{ this->coordinates.x = 2; }
	}

	else if (this->moves.right) {
		if (this->coordinates.x + this->s + this->w < this->area_width )	{ this->coordinates.x += this->s; }
		else	{ this->coordinates.x = this->area_width - this->w - 1; }
	}

	if (this->moves.up) {
		if	(this->coordinates.y + this->s + this->h < this->area_height - 2)	{ this->coordinates.y += this->s; }
		else	{ this->coordinates.y = this->area_height - this->h - 2; }
	}

	else if (this->moves.down) {
		if	(this->coordinates.y - this->s > 2)	{ this->coordinates.y -= this->s; }
		else	{ this->coordinates.y = 2; }
	}

	// this->moves.left = false, this->moves.right = false, this->moves.up = false, this->moves.down = false;
	return	{ this->coordinates.x, this->coordinates.y };
}

MOVES Player::random_move()
{
	std::mt19937 gen(this->rd());
	std::uniform_real_distribution<double> dist(0.0, 1.0);
	bool left = dist(gen) < 0.5, right = dist(gen) < 0.5, up = dist(gen) < 0.5, down = dist(gen) < 0.5;

	return { left, right, up, down };
}