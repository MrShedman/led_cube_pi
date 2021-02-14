#pragma once

#include "math/Angle.hpp"

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include <glm/gtc/quaternion.hpp>

class Transform
{
public:

	Transform();

	virtual ~Transform();

	void setPosition(float x, float y, float z = 0.0f);

	void setPosition(const glm::vec3& position);

	void setRotation(Angle angle);

	void setRotation(const glm::vec3& axis, Angle angle);

	void setRotation(const glm::vec3& pivot, const glm::vec3& axis, Angle angle);

	void setRotation(const glm::quat& rotation);

	void setScale(float factor);

	void setScale(float factorX, float factorY, float factorZ = 1.0f);

	void setScale(const glm::vec3& factors);

	void setOrigin(float x, float y, float z = 0.0f);

	void setOrigin(const glm::vec3& origin);

	const glm::vec3& getPosition() const;

	Angle get2DRotation() const;

	const glm::quat& getRotation() const;

	const glm::vec3& getScale() const;

	const glm::vec3& getOrigin() const;

	void move(float offsetX, float offsetY, float offsetZ = 0.0f);

	void move(const glm::vec3& offset);

	void rotate(Angle angle);

	void rotate(const glm::vec3& axis, Angle angle);

	void rotate(const glm::vec3& pivot, const glm::vec3& axis, Angle angle);

	void rotate(const glm::quat& rotation);

	void scale(float factorX, float factorY, float factorZ = 1.0f);

	void scale(const glm::vec3& factor);

	const glm::mat4& getTransform() const;

	const glm::mat4& getInverseTransform() const;

	bool transformNeedsUpdate() const;

	void reset();

private:

	void update() const;

	glm::vec3          m_origin;                     ///< Origin of translation/rotation/scaling of the object
	glm::vec3          m_position;                   ///< Position of the object in the 3D world
	glm::quat		   m_rotation;					///< Orientation of the object
	glm::vec3          m_scale;                      ///< Scale of the object
	mutable glm::mat4  m_transform;                  ///< Combined transformation of the object
	mutable bool       m_transformNeedUpdate;        ///< Does the transform need to be recomputed?
	mutable glm::mat4  m_inverseTransform;           ///< Combined transformation of the object
	mutable bool       m_inverseTransformNeedUpdate; ///< Does the transform need to be recomputed?
};