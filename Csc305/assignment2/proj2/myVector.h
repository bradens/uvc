/*********
 Vector Class
Written by Amy Gooch
Copyright 2003
*********/


#ifndef _myVector_H
#define _myVector_H

class MyVec4f {
 public:
  float data[4];

  MyVec4f();
  MyVec4f(const float& a, const float& b, const float& c, const float& d);
  MyVec4f(const MyVec4f& copied);
  ~MyVec4f();

  void set(float& a, float& b, float& c, float& d);
  float norm(); //returns length of vector
  void normalize(); //makes vector unit length
  void zero(); //assigns zeros to all vector values

  float x(){return data[0];}
  float y(){return data[1];}
  float z(){return data[2];}
  float w(){return data[3];}

  void x(const float& val){data[0]=val;}
  void y(const float& val){data[1]=val;}
  void z(const float& val){data[2]=val;}
  void w(const float& val){data[3]=val;}

  MyVec4f operator = (const MyVec4f &other);
  MyVec4f operator + (const MyVec4f &other);
  MyVec4f operator - (const MyVec4f &other);

  MyVec4f operator += (const MyVec4f &other);
  MyVec4f operator -= (const MyVec4f &other);
  MyVec4f operator *= (const float &scalar);

  bool operator == (const MyVec4f &other);
  bool operator != (const MyVec4f &other);

  friend MyVec4f operator *(const float &s, const MyVec4f &v);
  friend MyVec4f operator *(const MyVec4f &v, const float &s);
  
  friend MyVec4f cross_product(const MyVec4f &LHV, const MyVec4f &RHV); 
  friend float dot_product(const MyVec4f &LHV, const MyVec4f &RHV);
};

#endif  //_myVector_H

