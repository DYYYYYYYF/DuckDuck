#pragma once

#include "Defines.hpp"
#include "ForwardDeclarations.hpp"

#define D_PI 3.14159265358979323846f
#define D_PI_2 2.0f * D_PI
#define D_HALF_PI 0.5f * D_PI
#define D_ONE_OVER_PI 1.0f / D_PI
#define D_ONE_OVER_TWO_PI 1.0f / D_PI_2
#define D_SQRT_TWO 1.41421356237309504880f
#define D_SQRT_THREE 1.73205080756887729352f
#define D_SQRT_ONE_OVER_TWO 0.70710678118654752440f
#define D_SQRT_ONE_OVER_THREE 0.57735026918962576450f
#define D_DEG2RAD_MULTIPLIER D_PI / 180.0f
#define D_RAD2DEG_MULTIPLIER 180.0f / D_PI

// The multiplier to convert seconds to milliseconds
#define D_SEC_TO_MS_MULTIPLIER 1000.0f

// The multiplier to convert milliseconds to seconds
#define D_MS_TO_SEC_MULTIPLIER 0.001f

// A huge number that should be larger than any valid number used
#define D_INFINITY 1e30f

// Smallest positive number where 1.0 + FLOAT_EPSILON != 0
#define D_FLOAT_EPSILON 1.192092896e-07f

//------------------------------------------------------------
// General math functions
//------------------------------------------------------------
template<typename T>
inline DAPI T Dabs(T x) {
	return abs(x);
}

template<typename T>
inline DAPI T DSin(T x, float epsilon = 1e-6) {
	if (Dabs(x - D_HALF_PI) < epsilon) {
		return 1;
	}
	else if (Dabs(x - D_HALF_PI - D_PI) < epsilon) {
		return -1;
	}
	else if (Dabs(x) < epsilon) {
		return 0;
	}

	return sin(x);
}

template<typename T>
inline DAPI T DCos(T x, float epsilon = 1e-6) {
	if (Dabs(x - D_HALF_PI) < epsilon) {
		return 0;
	}
	else if (Dabs(x - D_PI) < epsilon) {
		return -1;
	}
	else if (Dabs(x) < epsilon) {
		return 1;
	}

	return cos(x);
}

template<typename T>
inline DAPI T DTan(T x, float epsilon = 1e-6) {
	if (Dabs(std::fmod(x, D_PI)) < epsilon || Dabs(std::fmod(x, D_PI) - D_PI) < epsilon) {
		return x > 0.0 ? std::numeric_limits<T>::infinity() : -std::numeric_limits<T>::infinity();
	}

	return tan(x);
}

template<typename T>
inline DAPI T DArcTan(T x, float epsilon = 1e-6) {
	if (Dabs(x) > std::numeric_limits<T>::max()) {
		return (x > 0) ? D_HALF_PI : -D_HALF_PI;
	}

	return atan(x);
}

template<typename T>
inline DAPI T DArcTan2(T x, T y, float epsilon = 1e-6) {
	if (Dabs(x) < epsilon && Dabs(y) < epsilon) {
		return 0;
	}

	return atan2(x, y);
}

template<typename T>
inline DAPI T DAcos(T x, float epsilon = 1e-6) {
	if (x < -1.0 + epsilon) {
		return D_PI; 
	}
	if (x > 1.0 - epsilon) {
		return 0.0;  
	}

	return acos(x);
}

template<typename T>
inline DAPI T Dsqrt(T x) {
	return sqrt(x);
}

/*
*  Indicates if the value is a power of 2. 0 is considered not a power of 2.
*  @param value The value to be interpreted.
*  @return True if a power of 2, otherwise false.
*/
inline bool IsPowerOf2(unsigned int value) {
	return (value != 0) && ((value & (value - 1)) == 0);
}

DAPI int DRandom();
DAPI int DRandom(int min, int max);
DAPI float DRandom(float min, float max);

/*
* @brief Converts provided degress to radians.
* 
* @param degrees The degrees to be converted.
* @return The amount in radians.
*/
template<typename T>
inline T Deg2Rad(T degress) {
	return static_cast<T>(std::fmod(degress * D_DEG2RAD_MULTIPLIER, D_PI_2));
}

/*
* @brief Converts provided radians to degress.
* 
* @param radians The radians to be converted.
* @return The amount in degress.
*/
template<typename T>
inline T Rad2Deg(T radians) {
	return static_cast<T>(std::fmod(radians * D_RAD2DEG_MULTIPLIER, 360.0));
}
