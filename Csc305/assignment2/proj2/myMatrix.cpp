#include "myMatrix.h"

MyMat4f::MyMat4f()
{
	float data[16];
}

MyMat4f::MyMat4f(const MyMat4f &copied)
{
	float data[16];
	// TODO copy
}

MyMat4f::~MyMat4f()
{ }

MyMat4f::MyMat4f operator*(const MyMat4f &M, const float &K)
{
	//TODO
	return M;
}

MyMat4f::MyMat4f operator*(const float &K, const MyMat4f &M)
{
	//TODO
	return M;
}

MyMat4f::MyMat4f operator*(const MyMat4f &lhm, const MyMat4f &rhm)
{
	//TODO
	return lhm;
}

void MyMat4f::print()
{
	//TODO
}

void MyMat4f::identity()
{
	//TODO
}

void MyMat4f::transpose()
{
	//TODO
}

void MyMat4f::zero()
{
	//TODO
}

void MyMat4f::turnLeft(float angle)
{
	//TODO
}

void MyMat4f::turnRight(float angle)
{
	//TODO
}

MyVec4f MyMat4f::getTranslation()
{
	MyVec4f newVec;
	return newVec;
}

void MyMat4f::setTranslation(const MyVec4f &v)
{
	//TODO
}

void MyMat4f::setTranslation(const float &a, const float &b)
{
	//TODO
}

float MyMat4f::operator ()(const unsigned char &i, const unsigned char &j)
{
	float ans;
	return ans;
}

MyMat4f MyMat4f::operator =(const MyMat4f &other)
{

}
