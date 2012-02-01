/*****************************
   4 x 4 Matrix class
   Implemented by Amy Gooch
   Copyright 2003
******************************/


#ifndef _myMatrix_H
#define _myMatrix_H

#include "myVector.h"
#include <iostream>
#include <math.h>

class MyMat4f {

 public:
  float data[16];  //opengl style matrix
  int rc;
  int cc;
  MyMat4f();
  MyMat4f(const MyMat4f &copied);
  ~MyMat4f();

  void print();
  void identity();
  void transpose();
  void zero();

  void turnLeft(float angle);
  void turnRight(float angle);

  MyVec4f getTranslation();
  void setTranslation(const MyVec4f &v);
  void setTranslation(const float &a, const float &b,
		      const float &c, const float&d);

                		
  // operator overloading
  
  float operator() (const unsigned int &i, const unsigned int &j) const;
  float& operator() (const unsigned int &i, const unsigned int &j);

  MyMat4f operator=(const MyMat4f &other);
  
  bool operator==(const MyMat4f &other) const;
  bool operator!=(const MyMat4f &other) const;
  
  MyMat4f operator*=(const float &k);
  
  friend MyMat4f operator*(const MyMat4f &M, const float &K);
  friend MyMat4f operator*(const float &K, const MyMat4f &M);
  friend MyMat4f operator*(const MyMat4f &lhm, const MyMat4f &rhm);
  
}; //end of class MyMat4f


#endif //_myMatrix_H
