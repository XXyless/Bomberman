#pragma once
#include <string>
#include "Chmat.h"
#include <fstream>

class ObjectLoader{
	public:
		ObjectLoader();
		~ObjectLoader();
	public:
		CHMAT<int> LoadObject(std::string location, CHMAT<int> m, int x, int y);
	public:
		int x;
};