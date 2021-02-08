#include "Player.h"


Player::Player(int id, int x, int y, int w, int h, int s, int c, int bg, CHMAT<int>* screen) {
	this->id = id;
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
	this->c = c;
	this->s = s;
	this->moving = false;
	this->GameScreen = screen;



	this->draw(c);
}

Player::~Player() {
	delete this->GameScreen;
}

void Player::draw(int c) {
	for (auto x = 0; x < this->w; ++x)
		for (auto y = 0; y < this->h; ++y)
			this->GameScreen->set(this->x + x, this->y + y, c);
}

COORDINATES Player::move() {
	MOVES movement = this->random_move();

	return this->move(movement.left, movement.right, movement.up, movement.down);
}

COORDINATES Player::move(bool left, bool right, bool up, bool down) {
	this->draw(this->bg);

	if (left) {
		if (this->x - this->s > 1) {
			this->x -= this->s;
		}
		else {
			this->x = 1;
		}
	}
	else if (right) {
		if (this->x + this->s + this->w < this->GameScreen->x) {
			this->x += this->s;
		}
		else {
			this->x = this->GameScreen->x - this->w - 1;
		}
	}

	if (up) {
		if (this->y + this->s + this->h < this->GameScreen->y) {
			this->y += this->s;
		}
		else {
			this->y = this->GameScreen->y - this->h - 1;
		}
	}
	else if (down) {
		if (this->y - this->s > 1) {
			this->y -= this->s;
		}
		else {
			this->y = 1;
		}
	}

	this->draw(this->c);

	return { this->x, this->y };
}

MOVES Player::random_move()
{
	std::mt19937 gen(this->rd());
	std::uniform_real_distribution<double> dist(0.0, 1.0);
	bool left = dist(gen) < 0.5, right = dist(gen) < 0.5, up = dist(gen) < 0.5, down = dist(gen) < 0.5;

	return { left, right, up, down };
}