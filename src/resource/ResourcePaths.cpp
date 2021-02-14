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