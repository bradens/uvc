#include "myMatrix.h"

using namespace std;

MyMat4f::MyMat4f()
{
	float data[16];
	this->rc = 4;
	this->cc = 4;
	this->identity();
}

MyMat4f::MyMat4f(const MyMat4f &copied)
{
	float data[16];
	this->rc = copied.rc;
	this->cc = copied.cc;
	for (int i = 0;i < this->cc;i++)
	{
		for (int j = 0;j < this->rc;i++)
		{
			this->data[i + j*rc] = copied.data[i + j*rc];
		}
	}
}

MyMat4f::~MyMat4f()
{

}

MyMat4f::MyMat4f operator*(const MyMat4f &M, const float &K)
{
	MyMat4f newMat = MyMat4f(M);

	for (int i = 0;i < newMat.cc;i++)
	{
		for (int j = 0;j < newMat.rc;j++)
		{
			newMat.data[i + j*newMat.rc] = newMat.data[i + j*newMat.rc] * K;
		}
	}
	return newMat;
}

MyMat4f::MyMat4f operator*(const float &K, const MyMat4f &M)
{
	MyMat4f newMat = MyMat4f(M);

	for (int i = 0;i < newMat.cc;i++)
	{
		for (int j = 0;j < newMat.rc;j++)
		{
			newMat.data[i + j*newMat.rc] = newMat.data[i + j*newMat.rc] * K;
		}
	}
	return newMat;
}

MyMat4f::MyMat4f operator*(const MyMat4f &lhm, const MyMat4f &rhm)
{
	MyMat4f newMat = MyMat4f();
	float tmp = 0;
	for (int i = 0;i < lhm.rc;i++)
	{
		tmp = 0;
		for (int j = 0;j < lhm.cc;j++)
		{
			for (int z = 0;z < rhm.rc;z++)
			{
				tmp += (rhm.data[j + z*4] * lhm.data[i + z*4]);
			}
			newMat.data[i + j*4];
		}
	}
	return newMat;
}

void MyMat4f::print()
{
	for (int i = 0;i < 4;i++)
	{
		cout << "[ ";
		for (int j = 0;j < 4;j++)
			cout << this->data[i  + j*4];
		cout << " ]\n";
	}
}

void MyMat4f::identity()
{
	for (int i = 0;i < 4;i++)
	{
		for (int j = 0;j < 4;j++)
		{
			if (i == j)
				this->data[i + j*4] = 1;
			else
				this->data[i + j*4] = 0;
		}
	}
}

void MyMat4f::transpose()
{
	//TODO
}

void MyMat4f::zero()
{
	for (int i = 0;i < this->rc;i++)
		for (int j = 0;j < this->cc;j++)
			this->data[i+j*this->rc] = 0;
}

void MyMat4f::turnLeft(float angle)
{
	this->data[0] = cos(angle);
	this->data[1] = -sin(angle);
	this->data[4] = sin(angle);
	this->data[5] = cos(angle);
}

void MyMat4f::turnRight(float angle)
{
	this->turnLeft(-angle);
}

MyVec4f MyMat4f::getTranslation()
{
	MyVec4f newVec;
	return newVec;
}

void MyMat4f::setTranslation(const MyVec4f &v)
{
	for (int i = 0;i < this->rc;i++)
	{
		for (int j = 0;j < (this->cc - 1);j++)
		{
			this->data[i + j*rc] *= v.data[j];
		}
	}
}

void MyMat4f::setTranslation(const float &a, const float &b,
		const float &c, const float &d)
{
	this->setTranslation(MyVec4f(a, b, c, d));
}

float MyMat4f::operator ()(const unsigned int &i, const unsigned int &j) const
{
	return data[i+j*4];
}

float& MyMat4f::operator ()(const unsigned int &i, const unsigned int &j)
{
	return data[i + j*4];
}

MyMat4f MyMat4f::operator =(const MyMat4f &other)
{
	for (int i = 0;i < this->rc;i++)
		for (int j = 0;j < this->cc;j++)
			this->data[i + j*this->rc] = other.data[i + j*other.rc];
}

bool MyMat4f::operator ==(const MyMat4f &other) const
{
	for (int i = 0;i < this->rc;i++)
		for (int j = 0;j < this->cc;j++)
			if (this->data[i + j*this->rc] != other.data[i + j*other.rc])
				return false;
	return true;
}

bool MyMat4f::operator !=(const MyMat4f &other) const
{
	for (int i = 0;i < this->rc;i++)
		for (int j = 0;j < this->cc;j++)
			if (this->data[i + j*this->rc] != other.data[i + j*other.rc])
				return false;
	return true;
}

MyMat4f MyMat4f::operator*=(const float &k)
{
	return *this;
}

