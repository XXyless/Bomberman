#include "ObjectLoader.h"

ObjectLoader::ObjectLoader(){
	this->x = 5;
}

ObjectLoader::~ObjectLoader(){}

CHMAT<int> ObjectLoader::LoadObject(std::string location, CHMAT<int> m, int x, int y)
{
	UINT8* buff[2] = { nullptr, nullptr };

	UINT8* pixels = nullptr;

	BITMAPFILEHEADER* bmpHeader = nullptr;
	BITMAPINFOHEADER* bmpInfo = nullptr;

	std::ifstream file(location, std::ios::binary);
	return m;
}

