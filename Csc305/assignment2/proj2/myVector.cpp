#include "myVector.h"

using namespace std;

MyVec4f::MyVec4f()
{
	data[0] = 0;
	data[1] = 0;
	data[2] = 0;
	data[3] = 0;
}

MyVec4f::MyVec4f(const float &a, const float &b, const float &c, const float &d)
{
	data[0] = a;
	data[1] = b;
	data[2] = c;
	data[3] = d;
}

MyVec4f::MyVec4f(const MyVec4f &copied)
{

}

MyVec4f::~MyVec4f()
{
}

// REST IS TODO @braden
