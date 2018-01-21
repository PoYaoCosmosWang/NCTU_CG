#pragma once
#include<math.h>
class Vec3		// vList, nList, tList ªº structure
{
public:
	float ptr[3];
	Vec3()
	{
		for (int i = 0; i < 3; ++i)
		{
			ptr[i] = 0;
		}
	}
	Vec3(float *v)
	{
		for (size_t i = 0; i<3; i++)
			ptr[i] = v[i];
	}
	Vec3(float a,float b,float c)
	{
		
			ptr[0] = a;
			ptr[1] = b;
			ptr[2] = c;
	}
	inline float& operator[](size_t index)
	{
		return ptr[index];
	}
	Vec3 operator +(Vec3 a)
	{
		Vec3 b;
		b[0]=ptr[0] + a[0];
		b[1] = ptr[1] + a[1];
		b[2] = ptr[2] + a[2];
		return b;
	}
	Vec3 operator +=(Vec3 a)
	{
		ptr[0] += a[0];
		ptr[1] += a[1];
		ptr[2] += a[2];
		return *this;
	}
	Vec3 operator -=(Vec3 a)
	{
		ptr[0] -= a[0];
		ptr[1] -= a[1];
		ptr[2] -= a[2];
		return *this;
	}
	Vec3 operator -(Vec3 a)
	{
		Vec3 b;
		b[0]=ptr[0] - a[0];
		b[1]=ptr[1] - a[1];
		b[2]=ptr[2] - a[2];
		return b;
	}
	Vec3 operator* (float n)
	{
		Vec3 b;
		b[0] = ptr[0]*n;
		b[1] = ptr[1]*n;
		b[2] = ptr[2]*n;
		return b;

	}
	Vec3 operator/= (float n)
	{
		
		ptr[0] /= n;
		ptr[1] /= n;
		ptr[2] /= n;
		return *this;

	}
	Vec3 normalize()
	{
		float norm = 0;
		norm += ptr[0] * ptr[0];
		norm += ptr[1] * ptr[1];
		norm += ptr[2] * ptr[2];
		norm = sqrt(norm);
		(*this) /= norm;
		return *this;
	}
};