#pragma once

#include <glm/vec3.hpp>
#include <glm/common.hpp>    

template <class T>
class AABB
{
public:

	AABB<T>()
		:
		max(std::numeric_limits<T>::lowest(), std::numeric_limits<T>::lowest(), std::numeric_limits<T>::lowest()),
		min(std::numeric_limits<T>::max(), std::numeric_limits<T>::max(), std::numeric_limits<T>::max())
	{}

	AABB<T>(const AABB<T>& rhs)
		:
		max(rhs.max),
		min(rhs.min)
	{}

	AABB<T>(const glm::vec<3, T>& min, const glm::vec<3, T>& max)
		:
		max(max),
		min(min)
	{}

	AABB<T>(const std::vector<glm::vec<3, T>>& points)
	{
		include(points);
	}

    bool contains(const glm::vec<3, T>& point) const
    {
        return (point.x > min.x && point.x < max.x &&
                point.y > min.y && point.y < max.y &&
                point.z > min.z && point.z < max.z);
    }

	void include(const glm::vec<3, T>& point)
	{
        min = glm::min(min, point);
        max = glm::max(max, point);
	}

	void include(const std::vector<glm::vec<3, T>>& points)
	{
		for (auto&p : points)
			include(p);
	}

	T getWidth() const
	{
		return max.x - min.x;
	}

	T getHeight() const
	{
		return max.y - min.y;
	}

	T getDepth() const
	{
		return max.z - min.z;
	}

	T getVolume() const
	{
		return getWidth() * getHeight() * getDepth();
	}

    glm::vec3 getAbsoluteSize() const
    {
        return glm::vec3(getWidth(), getHeight(), getDepth());
    }

public:

	glm::vec<3, T> min;
	glm::vec<3, T> max;
};

typedef AABB<float> AABBf;
typedef AABB<int> AABBi;