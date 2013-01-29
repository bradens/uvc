/*****************************
   4 x 4 Matrix class
   Implemented by Amy Gooch
   Copyright 2003
******************************/


#ifndef _myMatrix_H
#define _myMatrix_H

#include "Vec.h"
#include <iostream>
#include <math.h>

class Mat {

 public:
  float data[16];  //opengl style matrix
  int rc;
  int cc;
  Mat();
  Mat(const Mat &copied);
  ~Mat();

  void print();
  void identity();
  void transpose();
  void zero();

  void turnLeft(float angle);
  void turnRight(float angle);
  void scale(float s);
  Vec getTranslation();
  void setTranslation(const Vec &v);
  void setTranslation(const float &a, const float &b,
		      const float &c, const float&d);

                		
  // operator overloading
  
  float operator() (const unsigned int &i, const unsigned int &j) const;
  float& operator() (const unsigned int &i, const unsigned int &j);

  Mat operator=(const Mat &other);
  
  bool operator==(const Mat &other) const;
  bool operator!=(const Mat &other) const;
  
  Mat operator*=(const float &k);
  Mat operator*=(const Mat &other);
  
  friend Mat operator*(const Mat &M, const float &K);
  friend Mat operator*(const float &K, const Mat &M);
  friend Mat operator*(const Mat &lhm, const Mat &rhm);
  
}; //end of class Mat


#endif //_myMatrix_H
