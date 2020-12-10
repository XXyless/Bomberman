#pragma once

#include <string>
#include <wtypes.h>
#include <vector>
#include "Properties.h"


class ObjectLoader{
	public:
		ObjectLoader(std::string object_paths);
		~ObjectLoader();

	public:
		bool copyObject(HBITMAP target, int object_id);
		int& getObject(int object_id);

	private:
		std::vector <int&> objectBag;

	private:

};

