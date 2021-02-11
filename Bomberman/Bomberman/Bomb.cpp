#include "Bomb.h"

Bomb::Bomb(_ID id,int x, int y, _SIZE w, _SIZE h, COLOR c) {

	this->id = id;		this->bomb_coord.x = x;		this->bomb_coord.y = y;
	this->w = w;		this->h = h;				this->c = c;
	this->bomb = false;

}
