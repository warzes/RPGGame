#pragma once


class AABB final
{
public:
	AABB() = default;
	AABB(const glm::vec3& Min, const glm::vec3& Max)
		: min(glm::min(Min, Max))
		, max(glm::max(Min, Max))
	{}
	AABB(const glm::vec3* points, size_t numPoints)
	{
		glm::vec3 vmin(std::numeric_limits<float>::max());
		glm::vec3 vmax(std::numeric_limits<float>::lowest());

		for (size_t i = 0; i != numPoints; i++)
		{
			vmin = glm::min(vmin, points[i]);
			vmax = glm::max(vmax, points[i]);
		}
		min = vmin;
		max = vmax;
	}
	AABB(const std::vector<glm::vec3>& points)
		: AABB(points.data(), points.size())
	{}

	void Set(const std::vector<glm::vec3>& vertexData, const std::vector<uint32_t>& indexData);

	void CombinePoint(const glm::vec3& point)
	{
		min = glm::min(min, point);
		max = glm::max(max, point);
	}

	void CombineAABB(const AABB& aabb)
	{
		min = glm::min(min, aabb.min);
		max = glm::max(max, aabb.max);
	}

	bool Overlaps(const AABB& anotherAABB) const
	{
		return max.x > anotherAABB.min.x && min.x < anotherAABB.max.x
			&& max.y > anotherAABB.min.y && min.y < anotherAABB.max.y
			&& max.z > anotherAABB.min.z && min.z < anotherAABB.max.z;
	}

	bool Inside(const glm::vec3& point) const
	{
		return max.x > point.x && min.x < point.x
			&& max.y > point.y && min.y < point.y
			&& max.z > point.z && min.z < point.z;
	}

	void Transform(const glm::mat4& transform)
	{
		glm::vec3 corners[] =
		{
			glm::vec3(min.x, min.y, min.z),
			glm::vec3(min.x, max.y, min.z),
			glm::vec3(min.x, min.y, max.z),
			glm::vec3(min.x, max.y, max.z),
			glm::vec3(max.x, min.y, min.z),
			glm::vec3(max.x, max.y, min.z),
			glm::vec3(max.x, min.y, max.z),
			glm::vec3(max.x, max.y, max.z),
		};
		for (auto& v : corners)
			v = glm::vec3(transform * glm::vec4(v, 1.0f));
		*this = AABB(corners, 8);
	}
	AABB GetTransformed(const glm::mat4& t) const
	{
		AABB b = *this;
		b.Transform(t);
		return b;
	}

	float GetVolume() const
	{
		glm::vec3 Diagonal = max - min;
		return Diagonal.x * Diagonal.y * Diagonal.z;
	}

	glm::vec3 GetSize() const { return max - min; }
	glm::vec3 GetCenter() const { return (max + min) * 0.5f; }

	glm::vec3 min{ std::numeric_limits<float>::max() };
	glm::vec3 max{ -std::numeric_limits<float>::max() };
};
