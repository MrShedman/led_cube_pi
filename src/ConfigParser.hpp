#pragma once

#include "led-matrix.h"

#include <yaml-cpp/yaml.h>

#include <map>

class ConfigParser
{
public:
    
    void parseYaml(const std::string& yaml);

    const rgb_matrix::RGBMatrix::Options& getMatrixOptions() const
    {
        return matrix_options;
    }

    const rgb_matrix::RuntimeOptions& getRuntimeOptions() const
    {
        return runtime_options;
    }

    std::string get_imu_dev() const
    {
        return imu_dev;
    }

    uint8_t get_imu_address() const
    {
        return imu_address;
    }

private:

    void parseString(const char* opt, std::string name, YAML::Node& node);

    std::map<std::string, std::string> string_value_map;
    rgb_matrix::RGBMatrix::Options matrix_options;
    rgb_matrix::RuntimeOptions runtime_options;
    std::string imu_dev;
    uint8_t imu_address;
};