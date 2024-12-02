#pragma once
#include "Vector.hpp"

template<typename T>
struct TVertex3 {
	TVector3<T> position;
	TVector3<T> normal;
	TVector2<T> texcoord;
	TVector4<T> color;
	TVector4<T> tangent;

	TVertex3() {
		position = TVector3<T>(0.0f);
		normal = TVector3<T>(0, 0, 1);
		texcoord = TVector2<T>(0, 0);
		color = TVector4<T>(1, 1, 1, 1);
		tangent = TVector4<T>(0, 0, 0, 1);
	}

	TVertex3(const TVertex3& v) {
		position = v.position;
		normal = v.normal;
		texcoord = v.texcoord;
		color = v.color;
		tangent = v.tangent;
	}

	bool Compare(const TVertex3& v0) {
		return position.Compare(v0.position, D_FLOAT_EPSILON) &&
			normal.Compare(v0.normal, D_FLOAT_EPSILON) &&
			texcoord.Compare(v0.texcoord, D_FLOAT_EPSILON) &&
			color.Compare(v0.color, D_FLOAT_EPSILON) &&
			tangent.Compare(v0.tangent, D_FLOAT_EPSILON);
	}
};

template<typename T>
struct TVertex2 {
	TVertex2() {
		position = TVector2<T>();
		texcoord = TVector2<T>();
	}

	TVertex2(const TVector2<T>& pos) {
		position = pos;
		texcoord = TVector2<T>();
	}

	TVertex2(const TVector2<T>& pos, const TVector2<T>& tex) {
		position = pos;
		texcoord = tex;
	}

	TVector2<T> position;
	TVector2<T> texcoord;
};
