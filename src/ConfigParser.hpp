#pragma once

#include "led-matrix.h"

#include <yaml-cpp/yaml.h>

#include <map>

using namespace rgb_matrix;

class ConfigParser
{
public:
    
    void parseYaml(const std::string& yaml);

    const RGBMatrix::Options& getMatrixOptions() const
    {
        return matrix_options;
    }

    const rgb_matrix::RuntimeOptions& getRuntimeOptions() const
    {
        return runtime_options;
    }

private:

    void parseString(const char* opt, std::string name, YAML::Node& node);

    std::map<std::string, std::string> string_value_map;
    RGBMatrix::Options matrix_options;
    rgb_matrix::RuntimeOptions runtime_options;
};