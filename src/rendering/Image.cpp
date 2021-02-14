#include "Image.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

#include <iostream>
#include <filesystem>

bool Image::loadFromFile(const std::string& filename)
{
	int bpp = 0;
    int width = 0;
    int height = 0;
	uint8_t* data = stbi_load(filename.c_str(), &width, &height, &bpp, STBI_rgb_alpha);
    m_size.x = width;
    m_size.y = height;

	if (data == nullptr)
	{
		std::cout << "[Image] Unable to load texture: " << filename << std::endl;
		return false;
	}

	m_pixels.assign(data, data + (m_size.x * m_size.y * bpp));

	return true;
}

bool Image::loadFromMemory(const uint8_t* data, glm::uvec2 size)
{
    if (data && size.x > 0 && size.y > 0)
    {
        // Create a new pixel buffer first for exception safety's sake
        std::vector<uint8_t> newPixels(data, data + size.x * size.y * 4);
        
        // Commit the new pixel buffer
        m_pixels.swap(newPixels);
        
        // Assign the new size
        m_size.x = size.x;
        m_size.y = size.y;
        return true;
    }
    else
    {
        std::cout << "[Image] Failed to load image from memory, no data provided" << std::endl;
        return false;
    }
}

bool Image::saveToFile(const std::string& filename)
{
    // Make sure the image is not empty
    if (!m_pixels.empty() && (m_size.x > 0) && (m_size.y > 0))
    {
        // Deduce the image type from its extension
        std::cout << "[Image] Saving " << filename << std::endl;
        // Extract the extension
        const std::string extension = std::filesystem::path(filename).extension();
        
        if (extension == ".bmp")
        {
            // BMP format
            if (stbi_write_bmp(filename.c_str(), m_size.x, m_size.y, 4, &m_pixels[0]))
                return true;
        }
        else if (extension == ".tga")
        {
            // TGA format
            if (stbi_write_tga(filename.c_str(), m_size.x, m_size.y, 4, &m_pixels[0]))
                return true;
        }
        else if (extension == ".png")
        {
            // PNG format
            if (stbi_write_png(filename.c_str(), m_size.x, m_size.y, 4, &m_pixels[0], 0))
                return true;
        }
        else if (extension == ".jpg" || extension == ".jpeg")
        {
            // JPG format
            if (stbi_write_jpg(filename.c_str(), m_size.x, m_size.y, 4, &m_pixels[0], 90))
                return true;
        }
    }

    std::cout << "[Image] Failed to save image " << filename << std::endl;
    return false;
}

const uint8_t* Image::getData() const
{
	return &m_pixels[0];
}

glm::uvec2 Image::getSize() const
{
	return m_size;
}

glm::uvec4 Image::getPixel(uint8_t x, uint8_t y) const
{
	const uint8_t* pixel = &m_pixels[(x + y * m_size.x) * 4];
	return glm::uvec4(pixel[0], pixel[1], pixel[2], pixel[3]);
}

void Image::flipHorizontally()
{
    if (!m_pixels.empty())
    {
        std::size_t rowSize = m_size.x * 4;

        for (std::size_t y = 0; y < m_size.y; ++y)
        {
            std::vector<uint8_t>::iterator left = m_pixels.begin() + y * rowSize;
            std::vector<uint8_t>::iterator right = m_pixels.begin() + (y + 1) * rowSize - 4;

            for (std::size_t x = 0; x < m_size.x / 2; ++x)
            {
                std::swap_ranges(left, left + 4, right);

                left += 4;
                right -= 4;
            }
        }
    }
}

void Image::flipVertically()
{
    if (!m_pixels.empty())
    {
        std::size_t rowSize = m_size.x * 4;

        std::vector<uint8_t>::iterator top = m_pixels.begin();
        std::vector<uint8_t>::iterator bottom = m_pixels.end() - rowSize;

        for (std::size_t y = 0; y < m_size.y / 2; ++y)
        {
            std::swap_ranges(top, top + rowSize, bottom);

            top += rowSize;
            bottom -= rowSize;
        }
    }
}