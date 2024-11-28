#pragma once
#include "DMath.hpp"
#include "Core/DMemory.hpp"

template<typename T>
struct TVector2 {
	static_assert(std::is_floating_point<T>::value);

public:
	union
	{
		T elements[2] = { 0.0f };
		struct {
			union
			{
				T x, r, s, u;
			};
			union
			{
				T y, g, t, v;
			};
		};
	};

public:
	TVector2() { Zero(); }
	TVector2(T x, T y) {
		r = x;
		g = y;
	}

	TVector2(const TVector2& v) {
		r = v.x;
		g = v.y;
	}

	void Zero() {
		x = 0.0f;
		y = 0.0f;
	}

	void One() {
		x = 1.0f;
		y = 1.0f;
	}

	/*
	* @brief Returns the squared length of the provided vector.
	*
	* @param vector The vector to retrieve the squared length of.
	* @return The squared length.
	*/
	T LengthSquared() const { return x * x + y * y; }

	/*
	* @brief Returns the length of the provided vector.
	*
	* @param vector The vector to retrieve the length of.
	* @return The length.
	*/
	T Length() const { return Dsqrt(LengthSquared()); }

	/*
	* @brief Normalizes vector
	*/
	TVector2 Normalize() {
		x /= Length();
		y /= Length();

		return *this;
	}

	/*
	* @brief Compares all elements of vector and ensures the difference is less than tolerance.
	*
	* @param vec The other vector2.
	* @param tolerance The difference tolerance. Typically K_FLOAT_EPSILON or similar.
	* @return True if within tolerance, otherwise false.
	*/
	bool Compare(const TVector2& vec, double tolerance = 0.000001) const {
		if (Dabs(x - vec.x) > tolerance) {
			return false;
		}

		if (Dabs(y - vec.y) > tolerance) {
			return false;
		}

		return true;
	}

	/*
	* @brief Returns the distance between two vectors.
	*
	* @param vec Another vector.
	* @return The distance between this vector and the other.
	*/
	T Distance(const TVector2& vec) const {
		TVector2 d{ x - vec.x, y - vec.y };
		return d.Length();
	}

	TVector2 operator+(const TVector2& v) {
		return TVector2{ x + v.x, y + v.y };
	}

	TVector2 operator-(const TVector2& v) {
		return TVector2{ x - v.x, y - v.y };
	}

	TVector2 operator*(int num) {
		return TVector2{ x * num, y * num };
	}

	TVector2 operator*(float num) {
		return TVector2{ x * num, y * num };
	}

	TVector2 operator/(int num) {
		return TVector2{ x / num, y / num };
	}

	TVector2 operator/(float num) {
		return TVector2{ x / num, y / num };
	}

	// Negative
	TVector2 operator-() {
		return TVector2(-x, -y);
	}

	friend std::ostream& operator<<(std::ostream& os, const TVector2& vec) {
		return os << "x: " << vec.x << " y: " << vec.y << "\n";
	}
};

template<typename T>
struct TVector3 {
	static_assert(std::is_floating_point<T>::value);

public:
	union {
		T elements[3] = { 0.0f };
		struct {
			union
			{
				// First element
				T x, r, s, u;
			};
			union
			{
				// Sec element
				T y, g, t, v;
			};
			union
			{
				// Third element
				T z, b, p, w;
			};
		};
	};

public:
	TVector3() { Zero(); }

	TVector3(float x) {
		r = x;
		g = x;
		b = x;
	}

	TVector3(float x, float y, float z) {
		r = x;
		g = y;
		b = z;
	}

	TVector3(const TVector3& v) {
		r = v.x;
		g = v.y;
		b = v.z;
	}

	void Zero() {
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	}

	void One() {
		x = 1.0f;
		y = 1.0f;
		z = 1.0f;
	}

	/*
	* @brief Returns the squared length of the provided vector.
	*
	* @param vector The vector to retrieve the squared length of.
	* @return The squared length.
	*/
	T LengthSquared() const { return x * x + y * y + z * z; }

	/*
	* @brief Returns the length of the provided vector.
	*
	* @param vector The vector to retrieve the length of.
	* @return The length.
	*/
	T Length() const { return Dsqrt(LengthSquared()); }

	/*
	* @brief Normalizes vector
	*/
	TVector3 Normalize() {
		x /= Length();
		y /= Length();
		z /= Length();

		return *this;
	}

	/*
	* @brief Compares all elements of vector and ensures the difference is less than tolerance.
	*
	* @param vec The other vector2.
	* @param tolerance The difference tolerance. Typically K_FLOAT_EPSILON or similar.
	* @return True if within tolerance, otherwise false.
	*/
	bool Compare(const TVector3& vec, double tolerance = 0.000001) const {
		if (Dabs(x - vec.x) > tolerance) {
			return false;
		}

		if (Dabs(y - vec.y) > tolerance) {
			return false;
		}

		if (Dabs(z - vec.z) > tolerance) {
			return false;
		}

		return true;
	}

	/*
	* @brief Returns the product between two vectors.
	*
	* @param vec Another vector.
	* @return The distance between this vector and the other.
	*/
	T Dot(const TVector3& vec) const {
		return x * vec.x + y * vec.y + z * vec.z;
	}

	/*
	* @brief Calculates and returns the cross product of two vectors.
	*
	* @param vec Another vector.
	* @return The cross product result of this vector and the other.
	*/
	TVector3 Cross(const TVector3& vec) const {
		return TVector3{
			y * vec.z - z * vec.y,
			z * vec.x - x * vec.z,
			x * vec.y - y * vec.x
		};
	}

	/*
	* @brief Returns the distance between two vectors.
	*
	* @param vec Another vector.
	* @return The distance between this vector and the other.
	*/
	float Distance(const TVector3& vec) const {
		TVector3 d{ x - vec.x, y - vec.y, z - vec.z };
		return d.Length();
	}

	/**
	 * @brief Transform v by m. NOTE: It is assumed by this function that the vector
	 * v is a point, not a direction, and is calculated as if a w component with a value of 1.0f is there.
	 *
	 * @param m The matrix to transform by.
	 * @return A transformed copy of v.
	 */
	TVector3 Transform(struct TMatrix4<T> m) {
		TVector3 out;
		out.x = x * m.data[0 + 0] + y * m.data[4 + 0] + z * m.data[8 + 0] + 1.0f * m.data[12 + 0];
		out.y = x * m.data[0 + 1] + y * m.data[4 + 1] + z * m.data[8 + 1] + 1.0f * m.data[12 + 1];
		out.z = x * m.data[0 + 2] + y * m.data[4 + 2] + z * m.data[8 + 2] + 1.0f * m.data[12 + 2];
		return out;
	}

	static TVector3 Forward() {
		return TVector3(0.0f, 0.0f, -1.0f);
	}

	static TVector3 Backward() {
		return TVector3(0.0f, 0.0f, 1.0f);
	}

	static TVector3 Left() {
		return TVector3(-1.0f, 0.0f, 0.0f);
	}

	static TVector3 Right() {
		return TVector3(1.0f, 0.0f, 0.0f);
	}

	static TVector3 Up() {
		return TVector3(0.0f, 1.0f, 0.0f);
	}

	static TVector3 Down() {
		return TVector3(0.0f, -1.0f, 0.0f);
	}

	// Add
	TVector3 operator+(const TVector3& v) {
		return TVector3{ x + v.x, y + v.y, z + v.z };
	}

	friend TVector3 operator+(const TVector3& v1, const TVector3& v2) {
		return TVector3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
	}

	// Sub
	TVector3 operator-(const TVector3& v) {
		return TVector3{ x - v.x, y - v.y, z - v.z };
	}

	friend TVector3 operator-(const TVector3& v1, const TVector3& v2) {
		return TVector3{ v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
	}

	// Mut
	TVector3 operator*(const TVector3& v) {
		TVector3 t = TVector3(x, y, z);
		return TVector3{ t.x * v.x, t.y * v.y, t.z * v.z };
	}

	friend TVector3 operator*(const TVector3& v1, const TVector3& v2) {
		return TVector3{ v1.x * v2.x, v1.y * v2.y, v1.z * v2.z };
	}

	TVector3 operator*(float num) {
		TVector3 t = TVector3(x, y, z);
		return TVector3{ t.x * num, t.y * num, t.z * num };
	}

	friend TVector3 operator*(const TVector3& v, float num) {
		return TVector3{ v.x * num, v.y * num, v.z * num };
	}

	TVector3 operator/(const TVector3& v) {
		return TVector3{ x / v.x, y / v.y, z / v.z };
	}

	// Div
	TVector3 operator/(int num) {
		return TVector3{ x / num, y / num, y / num };
	}

	TVector3 operator/(float num) {
		return TVector3{ x / num, y / num, y / num };
	}

	// Negative
	TVector3 operator-() {
		return TVector3(-x, -y, -z);
	}

	friend std::ostream& operator<<(std::ostream& os, const TVector3& vec) {
		return os << "x: " << vec.x << " y: " << vec.y << " z: " << vec.z << "\n";
	}
};

template<typename T>
struct TVector4 {
	static_assert(std::is_floating_point<T>::value);

public:
	union
	{
#if defined(DUSE_SIMD)
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
	TVector4() { Zero(); }

	TVector4(TVector3<T> vec, float w = 1.0f) {
#if defined(DUSE_SIMD)
		data = _mm_setr_ps(x, y, z, w);
#else
		r = vec.x;
		g = vec.y;
		b = vec.z;
		a = w;
#endif
	}

	TVector4(T x) {
		r = x;
		g = x;
		b = x;
		a = x;
	}

	TVector4(T x, T y, T z, T w) {
		r = x;
		g = y;
		b = z;
		a = w;
	}

	TVector4(const TVector4& v) {
		r = v.x;
		g = v.y;
		b = v.z;
		a = v.w;
	}

	void Zero() {
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
		w = 0.0f;
	}

	void One() {
		x = 1.0f;
		y = 1.0f;
		z = 1.0f;
		w = 1.0f;
	}

	/*
	* @brief Returns the squared length of the provided vector.
	*
	* @param vector The vector to retrieve the squared length of.
	* @return The squared length.
	*/
	T LengthSquared() const { return x * x + y * y + z * z + w * w; }

	/*
	* @brief Returns the length of the provided vector.
	*
	* @param vector The vector to retrieve the length of.
	* @return The length.
	*/
	T Length() const { return Dsqrt(LengthSquared()); }

	/*
	* @brief Normalizes vector
	*/
	TVector4 Normalize() {
		x /= Length();
		y /= Length();
		z /= Length();
		w /= Length();

		return *this;
	}

	/*
	* @brief Compares all elements of vector and ensures the difference is less than tolerance.
	*
	* @param vec The other vector2.
	* @param tolerance The difference tolerance. Typically K_FLOAT_EPSILON or similar.
	* @return True if within tolerance, otherwise false.
	*/
	bool Compare(const TVector4& vec, double tolerance = 0.000001f) const {
		if (Dabs(x - vec.x) > tolerance) {
			return false;
		}

		if (Dabs(y - vec.y) > tolerance) {
			return false;
		}

		if (Dabs(z - vec.z) > tolerance) {
			return false;
		}

		if (Dabs(w - vec.w) > tolerance) {
			return false;
		}

		return true;
	}

	/*
	* @brief Returns the product between two vectors.
	*
	* @param vec Another vector.
	* @return The distance between this vector and the other.
	*/
	T Dot(const TVector4& vec) const {
		return x * vec.x + y * vec.y + z * vec.z + w * vec.w;
	}

	/*
	* @brief Returns the distance between two vectors.
	*
	* @param vec Another vector.
	* @return The distance between this vector and the other.
	*/
	T Distance(const TVector4& vec) {
		TVector4 d{ x - vec.x, y - vec.y, z - vec.z, w - vec.w };
		return d.Length();
	}

	static TVector4 StringToVec4(const char* str) {
		if (str == nullptr) {
			return TVector4{ 1.0f, 1.0f, 1.0f, 1.0f };
		}

		TVector4 Result;
		if constexpr (std::is_same<T, float>::value) {
			sscanf(str, "%f %f %f %f", &Result.x, &Result.y, &Result.z, &Result.w);
		}
		else if constexpr (std::is_same<T, double>::value) {
			sscanf(str, "%lf %lf %lf %lf", &Result.x, &Result.y, &Result.z, &Result.w);
		}
		else {
			return Result;
		}
		return Result;
	}

	static TVector4 Identity() {
		TVector4 Result = TVector4(0.0f, 0.0f, 0.0f, 1.0f);
		return Result;
	}

public:
	// Add
	TVector4 operator+(const TVector4& v) {
		return TVector4{ x + v.x, y + v.y, z + v.z, w + v.w };
	}

	// Sub
	TVector4 operator-(const TVector4& v) {
		return TVector4{ x - v.x, y - v.y, z - v.z, w - v.w };
	}

	// Mut
	TVector4 operator*(const TVector4& v) {
		return TVector4{ x * v.x, y * v.y, z * v.z, w * v.w };
	}

	TVector4 operator*(int num) {
		return TVector4{ x * num, y * num, z * num, w * num };
	}

	TVector4 operator*(float num) {
		return TVector4{ x * num, y * num, z * num, w * num };
	}

	// Div
	TVector4 operator/(int num) {
		return TVector4{ x / num, y / num, y / num, w / num };
	}

	TVector4 operator/(float num) {
		return TVector4{ x / num, y / num, y / num, w / num };
	}

	TVector4 operator/(const TVector4& v) {
		return TVector4{ x / v.x, y / v.y, z / v.z, w / v.w };
	}

	// Negative
	TVector4 operator-() {
		return TVector4(-x, -y, -z, -w);
	}

	friend std::ostream& operator<<(std::ostream& os, const TVector4& vec) {
		return os << "x: " << vec.x << " y: " << vec.y << " z: " << vec.z << " w: " << vec.w << "\n";
	}
};
