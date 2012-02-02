#include "Mat.h"

using namespace std;

Mat::Mat()
{
	float data[16];
	this->rc = 4;
	this->cc = 4;
	this->identity();
}

Mat::Mat(const Mat &copied)
{
	float data[16];
	this->rc = copied.rc;
	this->cc = copied.cc;
	for (int i = 0;i < this->cc;i++)
	{
		for (int j = 0;j < this->rc;j++)
		{
			this->data[j + i*this->rc] = copied.data[j + i*copied.rc];
		}
	}
}

Mat::~Mat()
{

}

Mat::Mat operator*(const Mat &M, const float &K)
{
	Mat newMat = Mat(M);

	for (int i = 0;i < newMat.cc;i++)
	{
		for (int j = 0;j < newMat.rc;j++)
		{
			newMat.data[j + i*newMat.rc] = newMat.data[j + i*newMat.rc] * K;
		}
	}
	return newMat;
}

Mat::Mat operator*(const float &K, const Mat &M)
{
	Mat newMat = Mat(M);

	for (int i = 0;i < newMat.cc;i++)
	{
		for (int j = 0;j < newMat.rc;j++)
		{
			newMat.data[j + i*newMat.rc] = newMat.data[j + i*newMat.rc] * K;
		}
	}
	return newMat;
}

Mat::Mat operator*(const Mat &lhm, const Mat &rhm)
{
	Mat newMat = Mat();
	float tmp = 0;
	for (int i = 0;i < lhm.rc;i++)
	{
		for (int j = 0;j < lhm.cc;j++)
		{
			tmp = 0;
			for (int z = 0;z < rhm.rc;z++)
			{
				tmp += (rhm.data[j + z*4] * lhm.data[z + i*4]);
			}
			newMat.data[j + i*4] = tmp;
		}
	}
	return newMat;
}

void Mat::print()
{
	for (int i = 0;i < 4;i++)
	{
		cout << "[ ";
		for (int j = 0;j < 4;j++)
			cout << this->data[j  + i*4];
		cout << " ]\n";
	}
}

void Mat::identity()
{
	for (int i = 0;i < 4;i++)
	{
		for (int j = 0;j < 4;j++)
		{
			if (i == j)
				this->data[j + i*4] = 1;
			else
				this->data[j + i*4] = 0;
		}
	}
}

void Mat::transpose()
{
	//TODO
}

void Mat::zero()
{
	for (int i = 0;i < this->rc;i++)
		for (int j = 0;j < this->cc;j++)
			this->data[j+i*this->rc] = 0;
}

void Mat::turnLeft(float angle)
{
	this->data[0] = cos(angle);
	this->data[1] = -sin(angle);
	this->data[4] = sin(angle);
	this->data[5] = cos(angle);
}

void Mat::turnRight(float angle)
{
	this->turnLeft(-angle);
}

Vec Mat::getTranslation()
{
	Vec newVec;
	return newVec;
}

void Mat::setTranslation(const Vec &v)
{
	this->data[3] = v.data[0];
	this->data[7] = v.data[1];
	this->data[11] = v.data[2];
}

void Mat::setTranslation(const float &a, const float &b,
		const float &c, const float &d)
{
	this->setTranslation(Vec(a, b, c, d));
}

float Mat::operator ()(const unsigned int &i, const unsigned int &j) const
{
	return data[j+i*4];
}

float& Mat::operator ()(const unsigned int &i, const unsigned int &j)
{
	return data[j + i*4];
}

Mat Mat::operator =(const Mat &other)
{
	for (int i = 0;i < this->rc;i++)
		for (int j = 0;j < this->cc;j++)
			this->data[j + i*this->rc] = other.data[j + i*other.rc];
}

bool Mat::operator ==(const Mat &other) const
{
	for (int i = 0;i < this->rc;i++)
		for (int j = 0;j < this->cc;j++)
			if (this->data[j + i*this->rc] != other.data[j + i*other.rc])
				return false;
	return true;
}

bool Mat::operator !=(const Mat &other) const
{
	for (int i = 0;i < this->rc;i++)
		for (int j = 0;j < this->cc;j++)
			if (this->data[j + i*this->rc] != other.data[j + i*other.rc])
				return false;
	return true;
}

Mat Mat::operator*=(const float &k)
{
	for (int i = 0;i < this->cc;i++)
	{
		for (int j = 0;j < this->rc;j++)
		{
			this->data[j + i*this->rc] = this->data[j + i*this->rc] * k;
		}
	}
	return *this;
}

Mat Mat::operator*=(const Mat &other)
{
	float tmp = 0;
	for (int i = 0;i < this->rc;i++)
	{
		tmp = 0;
		for (int j = 0;j < this->cc;j++)
		{
			for (int z = 0;z < other.rc;z++)
			{
				tmp += (other.data[j + z*4] * this->data[i + z*4]);
			}
			this->data[i + j*4] = tmp;
		}
	}
	return *this;
}
