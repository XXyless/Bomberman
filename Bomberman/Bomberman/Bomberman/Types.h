#pragma once


typedef const char* MESSAGE;
typedef const int MESSAGE_CODE;

typedef int COLOR;

typedef int _ID;

typedef int _SIZE;

typedef char _KEY;
typedef bool _BOOL;

struct coordinates {
	int x, y;
};

struct moves {
	bool left, right, up, down;
};

typedef coordinates COORDINATES;
typedef moves MOVES;
