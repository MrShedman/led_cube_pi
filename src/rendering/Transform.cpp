#include "Transform.hpp"

#include <glm/gtc/matrix_transform.hpp>

Transform::Transform()
{
	reset();
}

Transform::~Transform()
{
}

void Transform::setPosition(float x, float y, float z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
	m_transformNeedUpdate = true;
	m_inverseTransformNeedUpdate = true;
}

void Transform::setPosition(const glm::vec3& position)
{
	setPosition(position.x, position.y, position.z);
}

void Transform::setRotation(Angle angle)
{
	setRotation(glm::vec3(0.0f, 0.0f, 1.0f), angle);
}

void Transform::setRotation(const glm::vec3& axis, Angle angle)
{
    glm::quat rotation = glm::angleAxis(angle.asRadians(), glm::normalize(axis));

	setRotation(rotation);
}

void Transform::setRotation(const glm::vec3& pivot, const glm::vec3& axis, Angle angle)
{
    glm::quat rotation = glm::angleAxis(angle.asRadians(), glm::normalize(axis));

	glm::vec3 vector = m_position - m_origin - pivot;

	setPosition(pivot + vector * rotation + m_origin);

	setRotation(rotation);
}

void Transform::setRotation(const glm::quat& rotation)
{
	m_rotation = rotation;

	m_transformNeedUpdate = true;
	m_inverseTransformNeedUpdate = true;
}

void Transform::setScale(float factor)
{
	setScale(factor, factor, factor);
}

void Transform::setScale(float factorX, float factorY, float factorZ)
{
	m_scale.x = factorX;
	m_scale.y = factorY;
	m_scale.z = factorZ;
	m_transformNeedUpdate = true;
	m_inverseTransformNeedUpdate = true;
}

void Transform::setScale(const glm::vec3& factors)
{
	setScale(factors.x, factors.y, factors.z);
}

void Transform::setOrigin(float x, float y, float z)
{
	m_origin.x = x;
	m_origin.y = y;
	m_origin.z = z;
	m_transformNeedUpdate = true;
	m_inverseTransformNeedUpdate = true;
}

void Transform::setOrigin(const glm::vec3& origin)
{
	setOrigin(origin.x, origin.y, origin.z);
}

const glm::vec3& Transform::getPosition() const
{
	return m_position;
}

Angle Transform::get2DRotation() const
{
	glm::vec3 right = getRotation() * glm::vec3(1.0f, 0.0f, 0.0f);

	return radians(std::atan(right.y / right.x)).asDegrees();
}

const glm::quat& Transform::getRotation() const
{
	return m_rotation;
}

const glm::vec3& Transform::getScale() const
{
	return m_scale;
}

const glm::vec3& Transform::getOrigin() const
{
	return m_origin;
}

void Transform::move(float offsetX, float offsetY, float offsetZ)
{
	setPosition(m_position.x + offsetX, m_position.y + offsetY, m_position.z + offsetZ);
}

void Transform::move(const glm::vec3& offset)
{
	setPosition(m_position + offset);
}

void Transform::rotate(Angle angle)
{
	rotate(glm::vec3(0.0f, 0.0f, 1.0f), angle);
}

void Transform::rotate(const glm::vec3& axis, Angle angle)
{
    glm::quat q = glm::angleAxis(angle.asRadians(), glm::normalize(axis));
	rotate(q);
}

void Transform::rotate(const glm::vec3& pivot, const glm::vec3& axis, Angle angle)
{
    glm::quat rotation = glm::angleAxis(angle.asRadians(), glm::normalize(axis));

	glm::vec3 vector = m_position - m_origin - pivot;

	setPosition(pivot + vector * rotation + m_origin);

	rotate(rotation);
}

void Transform::rotate(const glm::quat& rotation)
{
	setRotation(glm::normalize(rotation * m_rotation));
}

void Transform::scale(float factorX, float factorY, float factorZ)
{
	setScale(m_scale.x * factorX, m_scale.y * factorY, m_scale.z * factorZ);
}

void Transform::scale(const glm::vec3& factor)
{
	setScale(m_scale.x * factor.x, m_scale.y * factor.y, m_scale.z * factor.z);
}

void Transform::reset()
{
	m_origin = glm::vec3(0.0f, 0.0f, 0.0f);
	m_position = glm::vec3(0.0f, 0.0f, 0.0f);
	m_rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	m_scale = glm::vec3(1.0f, 1.0f, 1.0f);
	m_transform = glm::mat4(1.0f);
	m_transformNeedUpdate = true;
	m_inverseTransform = glm::mat4(1.0f);
	m_inverseTransformNeedUpdate = true;
}

void Transform::update() const
{
	// Recompute the combined transform if needed
	if (m_transformNeedUpdate)
	{
        glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0), m_position - m_origin);
        glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0), m_scale);

		m_transform = translationMatrix * glm::mat4_cast(m_rotation) * scaleMatrix;

		m_transformNeedUpdate = false;
	}
}

const glm::mat4& Transform::getTransform() const
{
	update();

	return m_transform;
}

const glm::mat4& Transform::getInverseTransform() const
{
	// Recompute the inverse transform if needed
	if (m_inverseTransformNeedUpdate)
	{
		m_inverseTransform = glm::inverse(getTransform());
		m_inverseTransformNeedUpdate = false;
	}

	return m_inverseTransform;
}

bool Transform::transformNeedsUpdate() const
{
	return m_transformNeedUpdate;
}