#include "Transform.hpp"

Transform::Transform() {
	SetPRS(Vector3(0.0f), Quaternion(), Vector3(1.0f));
	Local = Matrix4::Identity();
}

Transform::Transform(const Transform& trans) {
	SetPRS(trans.GetPosition(), trans.GetRotation(), trans.GetScale());
	Local = trans.Local;
}

Transform::Transform(const Vector3& position) {
	SetPRS(position, Quaternion(), Vector3(1.0f));
	Local = Matrix4::Identity();
}

Transform::Transform(const Quaternion& rotation) {
	SetPRS(Vector3(0.0f), rotation, Vector3(1.0f));
	Local = Matrix4::Identity();
}

Transform::Transform(const Vector3& position, const Quaternion& rotation) {
	SetPRS(position, rotation, Vector3(1.0f));
	Local = Matrix4::Identity();
}

Transform::Transform(const Vector3& position, const Quaternion& rotation, const Vector3& scale) {
	SetPRS(position, rotation, scale);
	Local = Matrix4::Identity();
}

void Transform::Translate(const Vector3& translation) {
	vPosition = vPosition + translation;
	IsDirty = true;
}

void Transform::Rotate(const Quaternion& rotation) {
	vRotation = rotation.Multiply(vRotation);
	IsDirty = true;
}

void Transform::Scale(const Vector3& scale) {
	vScale = vScale * scale;
	IsDirty = true;
}

void Transform::SetPR(const Vector3& pos, const Quaternion& rotation) {
	vPosition = pos;
	vRotation = rotation;
	IsDirty = true;
}

void Transform::SetPRS(const Vector3& pos, const Quaternion& rotation, const Vector3& scale) {
	vPosition = pos;
	vRotation = rotation;
	vScale = scale;
	IsDirty = true;
}

void Transform::TransformRotate(const Vector3& translation, const Quaternion& rotation) {
	vPosition = vPosition + translation;
	vRotation = rotation.Multiply(vRotation);
	IsDirty = true;
}

void Transform::UpdateLocal() {
	Matrix4 R = vRotation.ToRotationMatrix();
	Matrix4 T = Matrix4::FromTranslation(vPosition);
	Matrix4 S = Matrix4::FromScale(vScale);

	Local = T.Multiply(R.Multiply(S));
	IsDirty = false;
}

Matrix4 Transform::GetLocal() {
	if (IsDirty) {
		UpdateLocal();
	}

	return Local;
}
