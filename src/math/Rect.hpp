#pragma once

#include <glm/vec2.hpp>

#include <iostream>
#include <iomanip>

template <class T>
class Rect
{
public:
	Rect()
		:
		top(0),
		bottom(0),
		left(0),
		right(0)
	{}

	Rect(T top, T bottom, T left, T right)
		:
		top(top),
		bottom(bottom),
		left(left),
		right(right)
	{}

	Rect(const Rect<T> &rhs)
		:
		top(rhs.top),
		bottom(rhs.bottom),
		left(rhs.left),
		right(rhs.right)
	{}

	Rect(const std::vector<glm::vec<2, T>> &points)
	{
		left = std::numeric_limits<T>::max();
		right = -std::numeric_limits<T>::max();
		top = std::numeric_limits<T>::max();
		bottom = -std::numeric_limits<T>::max();

		for (size_t s = 0; s < points.size(); ++s)
			include(points[s]);
	}

	void include(const glm::vec<2, T> &point)
	{
		if (left > point.x) left = point.x;
		if (right < point.x) right = point.x;
		if (top > point.y) top = point.y;
		if (bottom < point.y) bottom = point.y;
	}

	void include(const std::vector<glm::vec<2, T>> &points)
	{
		for (size_t s = 0; s < points.size(); ++s)
			include(points[s]);
	}

	void translate(T dx, T dy)
	{
		top += dy;
		bottom += dy;
		left += dx;
		right += dx;
	}

	bool lineIntersects(const glm::vec<2, T>& begin, const glm::vec<2, T>& end)
	{
		bool contB = contains(begin);
		bool contE = contains(end);

		if (contB != contE)
		{
			return true;
		}
		else if (contB && contE)
		{
			return false;
		}

		glm::vec<2, T> topLeft(left, top);
		glm::vec<2, T> topRight(right, top);
		glm::vec<2, T> bottomLeft(left, bottom);
		glm::vec<2, T> bottomRight(right, bottom);

		std::vector<std::pair<glm::vec<2, T>, glm::vec<2, T>>> lines;

		lines.emplace_back(topLeft, topRight);
		lines.emplace_back(topRight, bottomRight);
		lines.emplace_back(bottomRight, bottomLeft);
		lines.emplace_back(bottomLeft, topLeft);

		for (size_t i = 0; i < 4; ++i)
		{
			glm::vec<2, T> CmP = lines[i].first - begin;
			glm::vec<2, T> r = end - begin;
			glm::vec<2, T> s = lines[i].second - lines[i].first;

			T CmPxr = glm::cross(CmP, r);
			T CmPxs = glm::cross(CmP, s);
			T rxs = glm::cross(r, s);

			if (CmPxr == 0.f)
			{
				// Lines are collinear, and so intersect if they have any overlap

				if (((lines[i].first.x - begin.x < 0) != (lines[i].first.x - end.x < 0))
					|| ((lines[i].first.y - begin.y < 0) != (lines[i].first.y - end.y < 0)))
				{
						return true;
				}
			}

			if (rxs == 0)
				continue; // Lines are parallel.

			T rxsr = 1 / rxs;
			T t = CmPxs * rxsr;
			T u = CmPxr * rxsr;

			if ((t >= 0) && (t <= 1) && (u >= 0) && (u <= 1))
			{
				return true;
			}
		}

		return false;
	}

	bool contains(T x, T y) const
	{
		return (x >= left && x <= right && y <= bottom && y >= top);
	}

	bool contains(const glm::vec<2, T>& point) const
	{
		return contains(point.x, point.y);
	}

	bool contains(const Rect<T> rect) const
	{
		return contains(rect.left, rect.top) && contains(rect.right, rect.bottom);
	}

	bool intersects(const Rect<T> rect) const
	{
		return (left < rect.right && right > rect.left &&
			top < rect.bottom && bottom > rect.top);
	}

	T getWidth() const
	{
		return right - left;
	}

	T getHeight() const
	{
		return bottom - top;
	}

	glm::vec<2, T> getCenter() const
	{
		T middleX = left + (right - left) / 2;
		T middleY = top + (bottom - top) / 2;

		return glm::vec<2, T>(middleX, middleY);
	}

	inline bool operator==(const Rect<T>& rhs) const
	{
		return top == rhs.top && bottom == rhs.bottom && left == rhs.left && right == rhs.right;
	}

	inline bool operator!=(const Rect<T>& rhs) const
	{
		return !operator==(rhs);
	}

public:
	T top;
	T bottom;
	T left;
	T right;
};


template <class T>
std::ostream& operator<< (std::ostream& os, const Rect<T>& m)
{
	std::cout << std::fixed << std::setprecision(5);
	std::cout << "(";

	std::cout << std::left << std::setfill(' ') << std::setw(10) << m.top << ", ";
	std::cout << std::left << std::setfill(' ') << std::setw(10) << m.bottom << ", ";
	std::cout << std::left << std::setfill(' ') << std::setw(10) << m.left << ", ";
	std::cout << std::left << std::setfill(' ') << std::setw(10) << m.right;

	std::cout << ")" << std::endl;

	return os;
}

typedef Rect<float> RectF;
typedef Rect<int> RectI;