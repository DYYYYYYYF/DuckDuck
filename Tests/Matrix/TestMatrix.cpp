#include <Math/MathTypes.hpp>

void TestMatrix() {

	LOG_INFO("Vector2<float> size: %ld", sizeof(TVector2<float>));
	LOG_INFO("Vector3<float> size: %ld", sizeof(TVector3<float>));
	LOG_INFO("Vector4<float> size: %ld", sizeof(TVector4<float>));
	LOG_INFO("TMatrix4<float> size: %ld", sizeof(TMatrix4<float>));

	LOG_INFO("Vector2<double> size: %ld", sizeof(TVector2<double>));
	LOG_INFO("Vector3<double> size: %ld", sizeof(TVector3<double>));
	LOG_INFO("Vector4<double> size: %ld", sizeof(TVector4<double>));
	LOG_INFO("TMatrix4<double> size: %ld", sizeof(TMatrix4<double>));

	Matrix4 Mat1;
	Matrix4 Mat2;
	for (int i = 0; i < 16; i++) {
		Mat1[i] = i + 1.0f;
		Mat2[i] = i + 1.0f;
	}


	cout << "Mat1: \n" << Mat1
		<< "Mat2: \n" << Mat2
		<< "Multiply: \n" << Mat1.Multiply(Mat2);

	Mat1.Transpose();
	cout << "Transpose: \n" << Mat1;

	Vector Vec1(1, 2, 3);
	Vector4 Vec4(0, 2, 0, 1);
	Matrix4 Transform(
		1, 0, 0, 1,
		0, 1, 0, 1,
		0, 0, 1, 1,
		0, 0, 0, 1
	);
	cout << "Vector3 right multiply Matrix: \n" << Vec1 * Transform;
	cout << "Matrix left multiply Vector3: \n" << Transform * Vec1;

	cout << "Vector4 right multiply Matrix: \n" << Vec4 * Transform;
	cout << "Matrix left multiply Vector4: \n" << Transform * Vec4;

	Vector4 Vec4_2(1, 0, 0, 1);
	cout << "Vector4 add: " << Vec4 + Vec4_2;
	cout << "Vector4 Sub: " << Vec4 - Vec4_2;
	cout << "Vector4 Mul: " << Vec4 * Vec4_2;
	cout << "Vector4 Div: " << Vec4 / Vec4_2;

	cout << "Vector4 length: " << Vec4.Length() << endl;
	cout << "Vector4 normalized: " << Vec4.Normalize() << endl;
	cout << "Vector4 length: " << Vec4.Length() << endl;


}