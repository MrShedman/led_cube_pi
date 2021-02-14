#include "ResourcePaths.hpp"

#include <filesystem>

std::string getExecutablePath()
{
    return std::filesystem::canonical("/proc/self/exe").remove_filename();
}

std::string getRootPath()
{
    // go up two folders from the executable
    return std::filesystem::path(getExecutablePath()).parent_path().parent_path().parent_path();
}

std::string getResourcePath()
{
    return getRootPath() + "/res/";
}

std::string SHADER_PATH(const std::string& filename)
{
    return getResourcePath() + "shaders/" + filename;
}

std::string MODEL_PATH(const std::string& filename)
{
    return getResourcePath() + "models/" + filename;
}

std::string VOXEL_PATH(const std::string& filename)
{
    return getResourcePath() + "voxels/" + filename;
}

std::string URDF_PATH(const std::string& filename)
{
    return getResourcePath() + "robots/" + filename;
}