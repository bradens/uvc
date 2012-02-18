#include "Vec.h"

using namespace std;

Vec::Vec()
{
	data[0] = 0;
	data[1] = 0;
	data[2] = 0;
	data[3] = 0;
}

Vec::Vec(const float &a, const float &b, const float &c, const float &d)
{
	data[0] = a;
	data[1] = b;
	data[2] = c;
	data[3] = d;
}

Vec::Vec(const Vec &copied)
{

}

Vec::~Vec()
{
}

// REST IS TODO @braden
