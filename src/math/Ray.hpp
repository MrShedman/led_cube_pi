#pragma once

#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

//#define CULLING 

class Ray
{
public:

	Ray()
		:
		origin(glm::vec3()),
		direction(glm::vec3())
	{}

	Ray(const glm::vec3& origin, const glm::vec3& direction)
		:
		origin(origin),
		direction(glm::normalize(direction))
	{}

	bool triangleIntersects(const glm::vec3& vert0, const glm::vec3& vert1, const glm::vec3& vert2, float &t)
	{
		glm::vec3 edge1, edge2, tvec, pvec, qvec;
		float det;
		float u, v;
		const float EPSILON = 0.000001f;

		edge1 = vert1 - vert0;
		edge2 = vert2 - vert0;

		pvec = glm::cross(direction, edge2);
		det = glm::dot(edge1, pvec);

#ifdef CULLING
		if (det < EPSILON)
			return false;
		tvec = origin - vert0;

		u = tvec.Dot(pvec);
		if ((u < 0.0f) || (u > det))
			return false;

		qvec = tvec.Cross(edge1);
		v = direction.Dot(qvec);
		if (v < 0.0f || u + v > det)
			return false;

		t = edge2.Dot(qvec) / det;
		return true;
#else
		if (det > -EPSILON && det < EPSILON)
			return false;

		float inv_det = 1.0f / det;
		tvec = origin - vert0;
		u = glm::dot(tvec, pvec) * inv_det;
		if (u < 0.0f || u > 1.0f)
			return false;

		qvec = glm::cross(tvec, edge1);

		v = glm::dot(direction, qvec) * inv_det;
		if (v < 0.0f || u + v > 1.0f)
			return false;

		t = glm::dot(edge2, qvec) * inv_det;
		return true;
#endif
	}

	glm::vec3 origin;
	glm::vec3 direction;
};