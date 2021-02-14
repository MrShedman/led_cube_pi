#pragma once

#include <string>

std::string getExecutablePath();

std::string getRootPath();

std::string getResourcePath();

std::string SHADER_PATH(const std::string& filename);

std::string MODEL_PATH(const std::string& filename);

std::string VOXEL_PATH(const std::string& filename);

std::string URDF_PATH(const std::string& filename);