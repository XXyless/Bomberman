#ifndef _CHMAT_H_GUARD_
#define _CHMAT_H_GUARD_
#pragma once

#include <iostream>
#include <windows.h>
#include <vector>

constexpr auto CHMAT_ERR_UNEXPECTED_TYPE = "Error, unexpected type!";
constexpr auto CHMAT_ERR_CODE_UNEXPECTED_TYPE = 0;

constexpr auto CHMAT_ERR_OUT_OF_BOUND = "Error, out of bound!";
constexpr auto CHMAT_ERR_CODE_OUT_OF_BOUND = 1;

int FillBMPInfo(BITMAPINFO* info, int x, int y, int bits);

template <typename T>
class CHMAT {
public:
	T* data;
	int x, y, bg_color;
	HBITMAP HBitmap;
public:
	CHMAT(int x, int y, int bg_color);
	~CHMAT();

	template <typename C>
	void set(int x, int y, C val);

	template<typename C>
	bool checkType(C val) const;

	void print_mat();
	void printnumber(int x0, int y0, int number, int c, int b);

	bool inRange(int x, int y);

public:
	void operator = (T a);

	void operator += (T a);

	void operator -= (T a);

	void operator *= (T a);

	void operator /= (T a);

	T& operator ()(int x, int y);
};
#endif // !_CHMAT_H_GUARD_

template<typename T>
inline CHMAT<T>::CHMAT(int x, int y, int bg_color) {
	int iType;
	unsigned char ucType;

	if (typeid(iType) == typeid(*this->data)) {
		BITMAPINFO binfo;
		FillBMPInfo(&binfo, x, y, 32);
		this->HBitmap = CreateDIBSection(NULL, &binfo, DIB_RGB_COLORS, (void**)&data, NULL, NULL);
	}
	else if (typeid(ucType) == typeid(*this->data)) {
		this->data = new T[x * y]();
	}

	this->x = x;
	this->y = y;
	this->bg_color = bg_color;

	int a, b;
	for (b = 0; b < this->y; b++)
	{
		for (a = 0; a < this->x; a++)
		{
			this->data[b * this->x + a] = this->bg_color;
		}
	}
}

template<typename T>
inline CHMAT<T>::~CHMAT() {
	try {
		DeleteObject(HBitmap);
	}
	catch (...) {
		delete this->data;
	}
}

template<typename T>
inline void CHMAT<T>::print_mat() {
	int x, y;
	for (y = 0; y < this->y; y++)
	{
		for (x = 0; x < this->x; x++)
		{
			std::cout << this->data[y * this->x + x] << ' ';
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

template<typename T>
inline bool CHMAT<T>::inRange(int x, int y) {
	return (x > 0) and (x < this->x) and (y > 0) and (y < this->y);
}

template<typename T>
template<typename C>
inline void CHMAT<T>::set(int x, int y, C val) {
	if (this->inRange(x, y)) {
		this->data[(y - 1) * this->x + (x - 1)] = val;
	}
	else {
		std::cout << CHMAT_ERR_OUT_OF_BOUND;
		exit(CHMAT_ERR_CODE_OUT_OF_BOUND);
	}
}

template<typename T>
inline void CHMAT<T>::operator=(T a) {
	int x, y;
	for (y = 0; y < this->y; y++)
	{
		for (x = 0; x < this->x; x++)
		{
			this->data[y * this->x + x] = a;
		}
	}
}

template<typename T>
inline void CHMAT<T>::operator+=(T a) {
	int x, y;
	for (y = 0; y < this->y; y++)
	{
		for (x = 0; x < this->x; x++)
		{
			this->data[y * this->x + x] += a;
		}
	}
}

template<typename T>
inline void CHMAT<T>::operator-=(T a) {
	int x, y;
	for (y = 0; y < this->y; y++)
	{
		for (x = 0; x < this->x; x++)
		{
			this->data[y * this->x + x] -= a;
		}
	}
}

template<typename T>
inline void CHMAT<T>::operator*=(T a) {
	int x, y;
	for (y = 0; y < this->y; y++)
	{
		for (x = 0; x < this->x; x++)
		{
			this->data[y * this->x + x] *= a;
		}
	}
}

template<typename T>
inline void CHMAT<T>::operator/=(T a) {
	int x, y;
	for (y = 0; y < this->y; y++)
	{
		for (x = 0; x < this->x; x++)
		{
			this->data[y * this->x + x] /= a;
		}
	}
}

template<typename T>
inline T& CHMAT<T>::operator()(int x, int y) {
	return this->data[this->x * (y - 1) + (x - 1)];
}

template<typename T>
template<typename C>
inline bool CHMAT<T>::checkType(C val) const {
	return std::is_same_v<T, C>;
}

template<typename T>
inline void CHMAT<T>::printnumber(int x0, int y0, int number, int c, int b) {
	std::vector<std::vector<int>> mergeM;
	switch (number)
	{
	case 0:
		mergeM = {
		{	b, b, c, c, c, c, c, c, c, c, b, b  },
		{	b, c, c, c, c, c, c, c, c, c, c, b  },
		{	c, c, c, c, b, b, b, b, c, c, c, c  },
		{	c, c, c, c, b, b, b, b, c, c, c, c  },
		{	c, c, c, b, b, b, b, b, b, c, c, c  },
		{	c, c, c, b, b, b, b, b, b, c, c, c  },
		{	c, c, c, b, b, b, b, b, b, c, c, c  },
		{	c, c, c, b, b, b, b, b, b, c, c, c  },
		{	c, c, c, c, b, b, b, b, c, c, c, c  },
		{	c, c, c, c, b, b, b, b, c, c, c, c  },
		{	b, c, c, c, c, c, c, c, c, c, c, b  },
		{	b, b, c, c, c, c, c, c, c, c, b, b  },
		};
		break;
	case 1:
		mergeM = {
		{	b, b, b, b, c, c, c, c, b, b, b, b  },
		{	b, b, b, c, c, c, c, c, b, b, b, b  },
		{	b, b, c, c, c, c, c, c, b, b, b, b  },
		{	b, c, c, b, c, c, c, c, b, b, b, b  },
		{	c, c, c, b, c, c, c, c, b, b, b, b  },
		{	b, b, b, b, c, c, c, c, b, b, b, b  },
		{	b, b, b, b, c, c, c, c, b, b, b, b  },
		{	b, b, b, b, c, c, c, c, b, b, b, b  },
		{	b, b, b, b, c, c, c, c, b, b, b, b  },
		{	b, b, b, b, c, c, c, c, b, b, b, b  },
		{	c, c, c, c, c, c, c, c, c, c, c, c  },
		{	c, c, c, c, c, c, c, c, c, c, c, c  },
		};
		break;
	case 2:
		mergeM = {
		{	b, c, c, c, c, c, c, c, c, c, b, b  },
		{	c, c, c, c, c, c, c, c, c, c, c, b  },
		{	c, b, b, b, b, b, b, b, b, c, c, c  },
		{	b, b, b, b, b, b, b, b, b, c, c, c  },
		{	b, b, b, b, b, b, b, b, b, c, c, c  },
		{	b, c, c, c, c, c, c, c, c, c, c, b  },
		{	b, c, c, c, c, c, c, c, c, c, b, b  },
		{	c, c, c, b, b, b, b, b, b, b, b, b  },
		{	c, c, c, b, b, b, b, b, b, b, b, b  },
		{	c, c, c, b, b, b, b, b, b, b, b, b  },
		{	c, c, c, c, c, c, c, c, c, c, c, c  },
		{	c, c, c, c, c, c, c, c, c, c, c, c  },
		};
		break;
	case 3:
		mergeM = {
		{	b, c, c, c, c, c, c, c, c, c, b, b  },
		{	c, c, c, c, c, c, c, c, c, c, c, b  },
		{	c, b, b, b, b, b, b, b, c, c, c, c  },
		{	b, b, b, b, b, b, b, b, b, c, c, c  },
		{	b, b, b, b, b, b, b, b, b, c, c, b  },
		{	b, b, c, c, c, c, c, c, c, c, b, b  },
		{	b, b, c, c, c, c, c, c, c, c, b, b  },
		{	b, b, b, b, b, b, b, b, b, c, c, b  },
		{	b, b, b, b, b, b, b, b, b, c, c, c  },
		{	c, b, b, b, b, b, b, b, c, c, c, c  },
		{	c, c, c, c, c, c, c, c, c, c, c, b  },
		{	b, c, c, c, c, c, c, c, c, c, b, b  },
		};
		break;
	case 4:
		mergeM = {
		{	c, c, c, b, b, b, b, b, b, c, c, c  },
		{	c, c, c, b, b, b, b, b, b, c, c, c  },
		{	c, c, c, b, b, b, b, b, b, c, c, c  },
		{	c, c, c, b, b, b, b, b, b, c, c, c  },
		{	c, c, c, b, b, b, b, b, b, c, c, c  },
		{	c, c, c, c, c, c, c, c, c, c, c, c  },
		{	c, c, c, c, c, c, c, c, c, c, c, c  },
		{	b, b, b, b, b, b, b, b, b, c, c, c  },
		{	b, b, b, b, b, b, b, b, b, c, c, c  },
		{	b, b, b, b, b, b, b, b, b, c, c, c  },
		{	b, b, b, b, b, b, b, b, b, c, c, c  },
		{	b, b, b, b, b, b, b, b, b, c, c, c  },
		};
		break;
	case 5:
		mergeM = {
		{	c, c, c, c, c, c, c, c, c, c, c, c  },
		{	c, c, c, c, c, c, c, c, c, c, c, c  },
		{	c, c, c, b, b, b, b, b, b, b, b, b  },
		{	c, c, c, b, b, b, b, b, b, b, b, b  },
		{	c, c, c, b, b, b, b, b, b, b, b, b  },
		{	c, c, c, c, c, c, c, c, c, c, b, b  },
		{	c, c, c, c, c, c, c, c, c, c, c, b  },
		{	b, b, b, b, b, b, b, b, b, c, c, c  },
		{	b, b, b, b, b, b, b, b, b, c, c, c  },
		{	c, b, b, b, b, b, b, b, b, c, c, c  },
		{	c, c, c, c, c, c, c, c, c, c, c, b  },
		{	b, c, c, c, c, c, c, c, c, c, b, b  },
		};
		break;
	case 6:
		mergeM = {
		{	b, b, c, c, c, c, c, c, c, c, c, b  },
		{	b, c, c, c, c, c, c, c, c, c, c, b  },
		{	c, c, c, b, b, b, b, b, b, b, b, b  },
		{	c, c, b, b, b, b, b, b, b, b, b, b  },
		{	c, c, b, b, b, b, b, b, b, b, b, b  },
		{	c, c, b, b, c, c, c, c, c, c, b, b  },
		{	c, c, c, c, c, c, c, c, c, c, c, b  },
		{	c, c, c, b, b, b, b, b, b, c, c, c  },
		{	c, c, c, b, b, b, b, b, b, c, c, c  },
		{	c, c, c, b, b, b, b, b, b, c, c, c  },
		{	b, c, c, c, c, c, c, c, c, c, c, b  },
		{	b, b, c, c, c, c, c, c, c, c, b, b  },
		};
		break;
	case 7:
		mergeM = {
		{	c, c, c, c, c, c, c, c, c, c, c, c  },
		{	c, c, c, c, c, c, c, c, c, c, c, c  },
		{	c, c, c, b, b, b, b, b, b, c, c, c  },
		{	c, c, c, b, b, b, b, b, c, c, c, b  },
		{	b, b, b, b, b, b, b, c, c, c, b, b  },
		{	b, b, b, b, b, b, b, c, c, c, b, b  },
		{	b, b, b, b, b, b, c, c, c, b, b, b  },
		{	b, b, b, b, b, b, c, c, c, b, b, b  },
		{	b, b, b, b, b, c, c, c, b, b, b, b  },
		{	b, b, b, b, b, c, c, c, b, b, b, b  },
		{	b, b, b, b, c, c, c, b, b, b, b, b  },
		{	b, b, b, b, c, c, c, b, b, b, b, b  },
		};
		break;
	case 8:
		mergeM = {
		{	b, b, b, c, c, c, c, c, c, b, b, b  },
		{	b, c, c, c, c, c, c, c, c, c, c, b  },
		{	b, c, c, b, b, b, b, b, b, c, c, b  },
		{	c, c, b, b, b, b, b, b, b, b, c, c  },
		{	b, c, c, b, b, b, b, b, b, c, c, b  },
		{	b, b, c, c, c, c, c, c, c, c, b, b  },
		{	b, b, c, c, c, c, c, c, c, c, b, b  },
		{	b, c, c, b, b, b, b, b, b, c, c, b  },
		{	c, c, b, b, b, b, b, b, b, b, c, c  },
		{	b, c, c, b, b, b, b, b, b, c, c, b  },
		{	b, c, c, c, c, c, c, c, c, c, c, b  },
		{	b, b, b, c, c, c, c, c, c, b, b, b  },
		};
		break;
	case 9:
		mergeM = {
		{	b, b, c, c, c, c, c, c, c, c, b, b  },
		{	b, c, c, c, c, c, c, c, c, c, c, b  },
		{	c, c, c, b, b, b, b, b, b, c, c, c  },
		{	c, c, b, b, b, b, b, b, b, b, c, c  },
		{	c, c, c, b, b, b, b, b, b, c, c, c  },
		{	b, c, c, c, c, c, c, c, c, c, c, c  },
		{	b, b, c, c, c, c, c, c, c, c, c, c  },
		{	b, b, b, b, b, b, b, b, b, c, c, c  },
		{	b, b, b, b, b, b, b, b, b, c, c, c  },
		{	c, c, b, b, b, b, b, b, c, c, c, c  },
		{	b, c, c, c, c, c, c, c, c, c, c, b  },
		{	b, c, c, c, c, c, c, c, c, c, c, b  },
		};
		break;
	default:
		break;
	}

	std::reverse(std::begin(mergeM), std::end(mergeM));
	int x = x0, y = y0;
	for (auto& row : mergeM) {
		for (auto& col : row) {
			this->set(x, y, col);
			++x;
		}
		x = x0;
		++y;
	}
}

int FillBMPInfo(BITMAPINFO* info, int x, int y, int bits) {
	info->bmiHeader.biBitCount = bits;
	info->bmiHeader.biClrImportant = 0;
	info->bmiHeader.biClrUsed = 0;
	info->bmiHeader.biCompression = BI_RGB;
	info->bmiHeader.biHeight = y;
	info->bmiHeader.biPlanes = 1;
	info->bmiHeader.biSize = sizeof(BITMAPINFO);
	info->bmiHeader.biSizeImage = x * y * bits / 8;
	info->bmiHeader.biWidth = x;
	info->bmiHeader.biXPelsPerMeter = 0;
	info->bmiHeader.biYPelsPerMeter = 0;
	return(0);
}

