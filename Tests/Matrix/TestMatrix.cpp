#include <Math/MathTypes.hpp>

void TestMatrix() {
	Matrix4 Mat1;
	Matrix4 Mat2;
	for (int i = 0; i < 16; i++) {
		Mat1[i] = i + 1;
		Mat2[i] = i + 1;
	}


	cout << "Mat1: \n" << Mat1
		<< "Mat2: \n" << Mat2
		<< "Res: \n" << Mat1.Multiply(Mat2);
}