#pragma once
#include "Vector.hpp"

//-----------------------------------------------
// Quaternion
//-----------------------------------------------
template<typename T>
struct TQuaternion {
	union
	{
#if defined(SIMD_SUPPORTED)
		// Used for SIMD operations
		/*alignas(16) */__m128 data;
#endif
		// An array of x, y, z, w
		/*alignas(16) */T elements[4] = { 0.0f };
		struct
		{
			union
			{
				T x, r, s;
			};
			union
			{
				T y, g, t;
			};
			union
			{
				T z, b, p;
			};
			union
			{
				T w, a, q;
			};
		};
	};

public:
	TQuaternion() {
		x = 0.0;
		y = 0.0;
		z = 0.0;
		w = 1.0;
	}

	TQuaternion(T x, T y, T z, T w) {
		r = x;
		g = y;
		b = z;
		a = w;
	}

	TQuaternion(const TVector3<T>& euler) {
		T EulerX = Deg2Rad(euler.x);
		T EulerY = Deg2Rad(euler.y);
		T EulerZ = Deg2Rad(euler.z);

		T CosPitch = DCos(EulerX / 2);
		T SinPitch = DSin(EulerX / 2);
		T CosYaw = DCos(EulerY / 2);
		T SinYaw = DSin(EulerY / 2);
		T CosRoll = DCos(EulerZ / 2);
		T SinRoll = DSin(EulerZ / 2);

		// Calculate quaternion
		w = CosPitch * CosYaw * CosRoll + SinPitch * SinYaw * SinRoll;
		x = SinPitch * CosYaw * CosRoll - CosPitch * SinYaw * SinRoll;
		y = CosPitch * SinYaw * CosRoll + SinPitch * CosYaw * SinRoll;
		z = CosPitch * CosYaw * SinRoll - SinPitch * SinYaw * CosRoll;
	}

	TQuaternion(const TVector3<T>& axis, T angle, bool normalize = true) {
		T HalfAngle = 0.5f * angle;
		T s = DSin(HalfAngle);
		T c = DCos(HalfAngle);

		x = s * axis.x;
		y = s * axis.y;
		z = s * axis.z;
		w = c;

		if (normalize) {
			Normalize();
		}
	}

	TQuaternion(const TQuaternion& quat) {
		x = quat.x;
		y = quat.y;
		z = quat.z;
		w = quat.w;
	}

public:
	inline TVector3<T> ToEuler() const {
		T Pitch = 0.0f;
		T Yaw = 0.0f;
		T Roll = 0.0f;

		// Pitch
		T SinR_CosP = 2.0f * (w * x + y * z);
		T CosR_CosP = 1.0f - 2.0f * (x * x + y * y);
		Roll = DArcTan2(SinR_CosP, CosR_CosP);

		// Yaw
		T Sinp = 2.0 * (w * x + y * z);
		if (Dabs(Sinp) >= 1.0) {
			Pitch = copysign(D_PI / 2.0, Sinp);
		}
		else {
			Pitch = DSin(Sinp);
		}

		// Roll
		T SinY_CosP = 2.0f * (w * z + x * y);
		T CosY_CosP = 1.0f - 2.0f * (y * y + z * z);
		Yaw = DArcTan2(SinY_CosP, CosY_CosP);

		return TVector3<T>(Pitch, Yaw, Roll);
	}

	inline float Normal() const {
		return Dsqrt(
			x * x +
			y * y +
			z * z +
			w * w
		);
	}

	// 共轭
	inline void Conjugate() {
		x *= -1;
		y *= -1;
		z *= -1;
	}

	inline void Inverse() {
		Conjugate();
	}

	inline TQuaternion<T> Multiply(const TQuaternion<T>& q) {
		Quaternion NewQuat;
		NewQuat.x = x * q.w +
			y * q.z -
			z * q.y +
			w * q.x;
		NewQuat.y = -x * q.z +
			y * q.w +
			z * q.x +
			w * q.y;
		NewQuat.z = x * q.y -
			y * q.x +
			z * q.w +
			w * q.z;
		NewQuat.w = -x * q.x -
			y * q.y -
			z * q.z +
			w * q.w;

		return NewQuat;
	}

	float LengthSquared() const { return x * x + y * y + z * z + w * w; }
	float Length() const { return Dsqrt(LengthSquared()); }
	TQuaternion<T> Normalize() {
		x /= Length();
		y /= Length();
		z /= Length();
		w /= Length();

		return *this;
	}

	float Dot(const TQuaternion<T>& v) {
		return x * v.x + y * v.y + z * v.z + w * v.w;
	}
};
