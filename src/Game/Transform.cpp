#include "stdafx.h"
#include "Transform.h"
//=============================================================================
Transform::Transform(const Vec3& scaling, const Quat& rotation, const Vec3& translation)
{
	SetWorldScaling(scaling);
	SetWorldRotation(rotation);
	SetWorldTranslation(translation);
}
//=============================================================================
Transform Transform::operator*(const Transform& other) const
{
	Mat4 mat = m_matrixWorld * other.GetWorldMatrix();
	Vec3 scaling;
	Quat rotation;
	Vec3 translation;
	mat.Decompose(scaling, rotation, translation);
	return {
	scaling, rotation, translation
	};
}
//=============================================================================
Transform& Transform::operator*=(const Transform& other)
{
	m_matrixWorld *= other.GetWorldMatrix();

	SetDirty();
	RecalculateTransform();

	return *this;
}
//=============================================================================
void Transform::SetFromMatrix(const Mat4& matrix)
{
	Vec3 scaling;
	Quat rotation;
	Vec3 translation;
	matrix.Decompose(scaling, rotation, translation);
	SetWorldScaling(scaling);
	SetWorldRotation(rotation);
	SetWorldTranslation(translation);
}
//=============================================================================
void Transform::SetWorldTransform(const Transform& t)
{
	SetWorldTransform(t.GetWorldScaling(), t.GetWorldRotation(), t.GetWorldTranslation());
}
//=============================================================================
void Transform::SetWorldTransform(const Vec3& scaling, const Quat& rotation, const Vec3& translation)
{
	if (m_scalingWorld != scaling)
	{
		m_scalingWorld = scaling;
		SetDirty();
	}

	if (m_rotationWorld != rotation)
	{
		m_rotationWorld = rotation;
		SetDirty();
	}

	if (m_translationWorld != translation)
	{
		m_translationWorld = translation;
		SetDirty();
	}

	RecalculateTransform();
}
//=============================================================================
void Transform::SetLocalTransform(const Transform& t)
{
	SetLocalTransform(t.GetLocalScaling(), t.GetLocalRotation(), t.GetLocalTranslation());
}
//=============================================================================
void Transform::SetLocalTransform(const Vec3& scaling, const Quat& rotation, const Vec3& translation)
{
	if (m_scalingLocal != scaling)
	{
		m_scalingLocal = scaling;
		SetDirty();
	}

	if (m_rotationLocal != rotation)
	{
		m_rotationLocal = rotation;
		SetDirty();
	}

	if (m_translationLocal != translation)
	{
		m_translationLocal = translation;
		SetDirty();
	}

	RecalculateTransform();
}
//=============================================================================
void Transform::SetLocalScaling(const Vec3& scaling)
{
	if (m_scalingLocal != scaling)
	{
		m_scalingLocal = scaling;
		SetDirty();
		RecalculateTransform();
	}
}
//=============================================================================
void Transform::SetWorldScaling(const Vec3& scaling)
{
	if (m_scalingWorld != scaling)
	{
		m_scalingWorld = scaling;
		SetDirty();

		if (m_parent == nullptr)
		{
			SetLocalScaling(scaling);
		}
		else
		{
			SetLocalScaling(scaling / m_parent->GetWorldScaling());
		}
	}
}
//=============================================================================
void Transform::SetLocalRotation(const Quat& rotation)
{
	if (m_rotationLocal != rotation)
	{
		m_rotationLocal = rotation;
		SetDirty();
		RecalculateTransform();
	}
}
//=============================================================================
void Transform::SetWorldRotation(const Quat& rotation)
{
	if (m_rotationWorld != rotation)
	{
		m_rotationWorld = rotation;
		SetDirty();

		if (m_parent == nullptr)
		{
			SetLocalRotation(rotation);
		}
		else
		{
			SetLocalRotation(rotation * m_parent->GetWorldRotation().Inverse());
		}
	}
}
//=============================================================================
void Transform::SetLocalTranslation(const Vec3& translation)
{
	if (m_translationLocal != translation)
	{
		m_translationLocal = translation;
		SetDirty();
		RecalculateTransform();
	}
}
//=============================================================================
void Transform::SetWorldTranslation(const Vec3& translation)
{
	if (m_translationWorld != translation)
	{
		m_translationWorld = translation;
		SetDirty();

		if (m_parent == nullptr)
		{
			SetLocalTranslation(translation);
		}
		else
		{
			SetLocalTranslation(translation * m_parent->GetWorldMatrix().Invert());
		}
	}
}
//=============================================================================
Vec3 Transform::GetLocalScaling() const
{
	return m_scalingLocal;
}
//=============================================================================
Quat Transform::GetLocalRotation() const
{
	return m_rotationLocal;
}
//=============================================================================
Vec3 Transform::GetLocalTranslation() const
{
	return m_translationLocal;
}
//=============================================================================
Mat4 Transform::GetLocalMatrix() const
{
	return m_matrixLocal;
}
//=============================================================================
Vec3 Transform::GetWorldScaling() const
{
	return m_scalingWorld;
}
//=============================================================================
Quat Transform::GetWorldRotation() const
{
	return m_rotationWorld;
}
//=============================================================================
Vec3 Transform::GetWorldTranslation() const
{
	return m_translationWorld;
}
//=============================================================================
Mat4 Transform::GetWorldMatrix() const
{
	return m_matrixWorld;
}
//=============================================================================
void Transform::SetDirty()
{
	m_isDirty = true;
}
//=============================================================================
void Transform::RecalculateTransform()
{
	if (m_isDirty == false)
	{
		return;
	}

	// First recalculate this transform
	m_matrixLocal = Mat4::SRT(m_scalingLocal, m_rotationLocal, m_translationLocal);
	m_matrixWorld = m_matrixLocal;
	if (m_parent != nullptr)
	{
		m_matrixWorld *= m_parent->GetWorldMatrix();
	}

	m_matrixLocal.Decompose(m_scalingLocal, m_rotationLocal, m_translationLocal);
	m_matrixWorld.Decompose(m_scalingWorld, m_rotationWorld, m_translationWorld);

	// Then its children
	for (Transform* t : m_children)
	{
		t->SetDirty();
		t->RecalculateTransform();
	}

	m_isDirty = false;
}
//=============================================================================
void Transform::SetParent(Transform* t)
{
	assert(t != this);

	if (m_parent != nullptr)
	{
		// TODO: Remove from previous parent
		assert(false);
	}

	m_parent = t;

	SetDirty();
	RecalculateTransform();
}
//=============================================================================
void Transform::AddChild(Transform* t)
{
	assert(t != nullptr);
	assert(t != this);
	m_children.push_back(t);
	t->SetParent(this);
}
//=============================================================================
void Transform::RemoveChild(Transform* t)
{
	assert(t != nullptr);
	assert(t != this);

	m_children.erase(remove_if(m_children.begin(), m_children.end(), [t](Transform* child_transform)
		{
			return child_transform == t;
		}),
		m_children.end());
	t->SetParent(nullptr);
}
//=============================================================================