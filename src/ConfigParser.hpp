#pragma once

#include "led-matrix.h"

#include <yaml-cpp/yaml.h>

#include <iostream>
#include <map>

using namespace rgb_matrix;

class ConfigParser
{
public:
    void parseYaml(const std::string& yaml)
    {
        YAML::Node config = YAML::LoadFile(yaml);

        //matrix_options.hardware_mapping = config["hardware_mapping"].as<std::string>().c_str();
        matrix_options.rows = config["rows"].as<int>();
        matrix_options.cols = config["cols"].as<int>();
        matrix_options.chain_length = config["chain_length"].as<int>();
        matrix_options.parallel = config["parallel"].as<int>();
        matrix_options.pwm_bits = config["pwm_bits"].as<int>();
        matrix_options.pwm_lsb_nanoseconds = config["pwm_lsb_nanoseconds"].as<int>();
        matrix_options.pwm_dither_bits = config["pwm_dither_bits"].as<int>();
        matrix_options.brightness = config["brightness"].as<int>();
        matrix_options.scan_mode = config["scan_mode"].as<int>();
        matrix_options.row_address_type = config["row_address_type"].as<int>();
        matrix_options.multiplexing = config["multiplexing"].as<int>();
        matrix_options.disable_hardware_pulsing = config["disable_hardware_pulsing"].as<bool>();
        matrix_options.show_refresh_rate = config["show_refresh_rate"].as<bool>();
        matrix_options.inverse_colors = config["inverse_colors"].as<bool>();
        //matrix_options.led_rgb_sequence = config["led_rgb_sequence"].as<std::string>().c_str();
        //matrix_options.pixel_mapper_config = config["pixel_mapper_config"].as<std::string>().c_str();
        //matrix_options.panel_type = config["panel_type"].as<std::string>().c_str();
        matrix_options.limit_refresh_rate_hz = config["limit_refresh_rate_hz"].as<int>();

        parseString(matrix_options.hardware_mapping, "hardware_mapping", config);
        parseString(matrix_options.led_rgb_sequence, "led_rgb_sequence", config);
        parseString(matrix_options.pixel_mapper_config, "pixel_mapper_config", config);
        parseString(matrix_options.panel_type, "panel_type", config);

        runtime_options.gpio_slowdown = config["gpio_slowdown"].as<int>();
        runtime_options.daemon = config["daemon"].as<int>();
        runtime_options.drop_privileges = config["drop_privileges"].as<int>();
        runtime_options.do_gpio_init = config["do_gpio_init"].as<bool>();
    }

    RGBMatrix::Options getMatrixOptions() const
    {
        return matrix_options;
    }

    rgb_matrix::RuntimeOptions getRuntimeOptions() const
    {
        return runtime_options;
    }

private:

    void parseString(const char* opt, std::string name, YAML::Node& node)
    {
        std::string value = node[name].as<std::string>();
        string_value_map[name] = value;
        opt = string_value_map[name].c_str();
    }

    std::map<std::string, std::string> string_value_map;
    RGBMatrix::Options matrix_options;
    rgb_matrix::RuntimeOptions runtime_options;
};