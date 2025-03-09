#pragma once

#include "CuteMath.h"

// Transform consisting of scale, rotation (quaternion) and translation.
class Transform final
{
public:
	Transform() = default;
	Transform(const Vec3& scaling, const Quat& rotation, const Vec3& translation);

	Transform operator*(const Transform& other) const;
	Transform& operator*=(const Transform& other);

	Transform Multiply(const Transform& other) const;

	void SetFromMatrix(const Mat4& matrix);
	void SetWorldTransform(const Transform& t);
	void SetWorldTransform(const Vec3& scaling, const Quat& rotation, const Vec3& translation);
	void SetLocalTransform(const Transform& t);
	void SetLocalTransform(const Vec3& scaling, const Quat& rotation, const Vec3& translation);
	void SetLocalScaling(const Vec3& scaling);
	void SetWorldScaling(const Vec3& scaling);
	void SetLocalRotation(const Quat& rotation);
	void SetWorldRotation(const Quat& rotation);
	void SetLocalTranslation(const Vec3& translation);
	void SetWorldTranslation(const Vec3& translation);

	float GetPitch() const;
	float GetYaw() const;
	float GetRoll() const;

	Vec3 GetLocalScaling() const;
	Quat GetLocalRotation() const;
	Vec3 GetLocalTranslation() const;
	Mat4 GetLocalMatrix() const;
	Vec3 GetWorldScaling() const;
	Quat GetWorldRotation() const;
	Vec3 GetWorldTranslation() const;
	Mat4 GetWorldMatrix() const;

	void SetDirty();
	void RecalculateTransform();

	void SetParent(Transform* t);
	void AddChild(Transform* t);
	void RemoveChild(Transform* t);

	Vec4 GetWorldForward()
	{
		return Vec4::Transform(Vec4::FORWARD, m_rotationWorld);
	}

private:
	Vec3 m_scalingWorld = Vec3(1.0f);
	Quat m_rotationWorld = Quat::IDENTITY;
	Vec3 m_translationWorld = Vec3::ZERO;
	Mat4 m_matrixWorld = Mat4::IDENTITY;

	Vec3 m_translationLocal = Vec3::ZERO;
	Vec3 m_scalingLocal = Vec3(1.0f);
	Quat m_rotationLocal = Quat::IDENTITY;
	Mat4 m_matrixLocal = Mat4::IDENTITY;

	bool m_isDirty = true;

	Transform* m_parent = nullptr;
	std::vector<Transform*> m_children;
};