#pragma once

#include <string>
#include <vector>

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

class Image
{
public:

	bool loadFromFile(const std::string& filename);
    bool loadFromMemory(const uint8_t* data, glm::uvec2 size);
    bool saveToFile(const std::string& filename);

	const uint8_t* getData() const;
	glm::uvec2 getSize() const;

	glm::uvec4 getPixel(uint8_t x, uint8_t y) const;

    void flipHorizontally();
    void flipVertically();

private:

	std::vector<uint8_t> m_pixels;
	glm::uvec2 m_size;
};