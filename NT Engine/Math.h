#ifndef _MATH_H_
#define _MATH_H_

#include "BasicIncludes.h"


namespace NTEngine
{
	class Math
	{
	public:
		template <typename T>
		DLLEXPORT static T Min(const T& a, const T& b)
		{
			//If 'a' is less than 'b', then return 'a' 
			//otherwise return 'b'
			return a < b ? a : b;
		}

		template <typename T>
		DLLEXPORT static T Max(const T& a, const T& b)
		{
			//If 'a' is more than 'b', then return 'a'
			//otherwise return 'b'
			return a > b ? a : b;
		}

		template <typename T>
		DLLEXPORT static T Lerp(const T& a, const T& b, float t)
		{
			//Linearly interpolates between 'a' and 'b' based on given factor 't' which ranges between 0 and 1
			//if t = 0, then 'a' will be returned 
			//if t = 1, then 'b' would be returned
			//if t = 0.5, then ('a' + 'b') / 2 would be returned

			return a + (b - a) * t;
		}

		template <typename T>
		DLLEXPORT static T Clamp(const T& x, const T& min, const T& max)
		{
			//Clamps 'x' between 'min' and 'max'
			//if 'x' is less than 'min', then 'min' would be returned
			//if 'x' is more than 'max', then 'max' would be returned
			//if 'x' is in-between 'min' and 'max', then 'x' would be returned


			//This one is more readable and easy to understand
			if (x < min)
				return min;
			else if (x > max)
				return max;
			else
				return x;

			//OR

			//return x < low ? low : (x > high ? high : x); 
		}

		DLLEXPORT static XMMATRIX InverseTranspose(CXMMATRIX M)
		{
			//make a copy because we're going to zero-out the translation row in the matrix
			//if we do that without making a copy of 'M', then the original matrix which is passed
			//in this function's parameter would also lose the translation row because our parameter is CXMMATRIX

			XMMATRIX A = M;

			//Zero out the translation row because "InverseTranspose" is applied to normals
			//and normals are vectors
			//So, we don't need or want the inverse-transpose of the translation
			A.r[3] = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

			XMVECTOR det = XMMatrixDeterminant(A);
			return XMMatrixTranspose(XMMatrixInverse(&det, A));
		}

		static const float Infinity; //The largest value a float can store is equivalent to infinity
		static const float PI; //We all love Pie
	};
}


#endif