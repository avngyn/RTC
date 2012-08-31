#ifndef VECTOR3D_H
#define VECTOR3D_H


class Vector3D {
   public:
      Vector3D();
      Vector3D(double x, double y, double z);
      ~Vector3D();

      double operator[](int i);

      Vector3D & operator=(const Vector3D &v);      
      Vector3D operator+(const Vector3D &v) const;
      Vector3D operator-(const Vector3D &v) const;
      Vector3D operator*(double s) const;
      Vector3D operator/(double s) const;
      void operator+=(const Vector3D &v);
      void operator-=(const Vector3D &v);
      void operator*=(double s);
      void operator/=(double s);

      void Clear();
      Vector3D Cross(const Vector3D &v);
      double Dot(const Vector3D &v);
      void Normalize();
      double Length();
   public:
      double x, y, z;
};


#endif // VECTOR3D_H
