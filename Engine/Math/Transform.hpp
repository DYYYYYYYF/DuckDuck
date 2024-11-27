#pragma once
#include "MathTypes.hpp"

class DAPI Transform {
public:
	/**
	 * @brief Creates and returns a new transform, using a zero vector for position,
	 * identity quaternion for rotation, and a one vector for scale. Also has a nullptr
	 * parent. Marked dirty by default.
	 */
	Transform();

	Transform(const Transform& trans);

	template<typename T>
	Transform(const std::vector<T>& dat) {
		SetPosition(Vec3((float)dat[12], (float)dat[13], (float)dat[14]));

		float ScaleX = Vec3((float)dat[0], (float)dat[1], (float)dat[2]).Length();
		float ScaleY = Vec3((float)dat[4], (float)dat[5], (float)dat[6]).Length();
		float ScaleZ = Vec3((float)dat[8], (float)dat[9], (float)dat[10]).Length();
		SetScale(Vec3(ScaleX, ScaleY, ScaleZ));

		Matrix4 Rotation = Matrix4::Identity();
		Rotation[0] = (float)dat[0] / ScaleX;
		Rotation[1] = (float)dat[1] / ScaleX;
		Rotation[2] = (float)dat[2] / ScaleX;
		Rotation[4] = (float)dat[4] / ScaleY;
		Rotation[5] = (float)dat[5] / ScaleY;
		Rotation[6] = (float)dat[6] / ScaleY;
		Rotation[8] = (float)dat[8] / ScaleZ;
		Rotation[9] = (float)dat[9] / ScaleZ;
		Rotation[10] = (float)dat[10] / ScaleZ;

		Quaternion Quat = MatrixToQuat(Rotation);
		SetRotation(Quat);
	}

	/**
	 * @brief Creates a transform from the given position.
	 * Uses a zero rotation and a one scale.
	 * 
	 * @param position The position to be used.
	 */
	Transform(const Vec3& position);

	/**
	 * @brief Creates a transform from the given rotation.
	 * Uses one scale.
	 *
	 * @param rotation The rotation to be used.
	 */
	Transform(const Quaternion& rotation);

	/**
	 * @brief Creates a transform from the given rotation and position.
	 * Uses one scale.
	 *
	 * @param position The position to be used.
	 * @param rotation The rotation to be used.
	 */
	Transform(const Vec3& position, const Quaternion& rotation);

	/**
	 * @brief Creates a transform.
	 *
	 * @param position The position to be used.
	 * @param rotation The rotation to be used.
	 * @param scale The scale to be used.
	 */
	Transform(const Vec3& position, const Quaternion& rotation, const Vec3& scale);

public:
	void SetParentTransform(Transform* t) { Parent = t; IsDirty = true; }
	Transform* GetParentTransform() { return Parent; }

	void SetPosition(Vec3 pos) { vPosition = pos; IsDirty = true; }
	const Vec3& GetPosition() const { return vPosition; }

	void SetScale(Vec3 scale) { vScale = scale; IsDirty = true; }
	const Vec3& GetScale() const { return vScale; }

	void SetRotation(Quaternion quat) { vRotation = quat; IsDirty = true; }
	const Quaternion& GetRotation() const { return vRotation; }


	void Translate(const Vec3& translation);
	void Rotate(const Quaternion& rotation);
	void Scale(const Vec3& scale);

	void SetPR(const Vec3& pos, const Quaternion& rotation);
	void SetPRS(const Vec3& pos, const Quaternion& rotation, const Vec3& scale);
	void TransformRotate(const Vec3& translation, const Quaternion& rotation);

	Matrix4 GetLocal();
	Matrix4 GetWorldTransform();

private:
	void UpdateLocal();

private:
	Vec3 vPosition;
	Quaternion vRotation;
	Vec3 vScale;
	bool IsDirty;
	Matrix4 Local;
	class Transform* Parent;
};