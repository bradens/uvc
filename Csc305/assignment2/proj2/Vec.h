/*********
 Vector Class
Written by Amy Gooch
Copyright 2003
*********/


#ifndef _myVector_H
#define _myVector_H

class Vec {
 public:
  float data[4];

  Vec();
  Vec(const float& a, const float& b, const float& c, const float& d);
  Vec(const Vec& copied);
  ~Vec();

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

  Vec operator = (const Vec &other);
  Vec operator + (const Vec &other);
  Vec operator - (const Vec &other);

  Vec operator += (const Vec &other);
  Vec operator -= (const Vec &other);
  Vec operator *= (const float &scalar);

  bool operator == (const Vec &other);
  bool operator != (const Vec &other);

  friend Vec operator *(const float &s, const Vec &v);
  friend Vec operator *(const Vec &v, const float &s);
  
  friend Vec cross_product(const Vec &LHV, const Vec &RHV);
  friend float dot_product(const Vec &LHV, const Vec &RHV);
};

#endif  //_myVector_H

