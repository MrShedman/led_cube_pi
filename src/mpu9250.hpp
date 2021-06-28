#pragma once

#include <glm/vec3.hpp>
#include <glm/mat3x3.hpp>
#include "i2c.hpp"

class MPU9250
{
public:
    enum DlpfBandwidth : uint8_t
    {
        DLPF_BANDWIDTH_184HZ = 0x01,
        DLPF_BANDWIDTH_92HZ = 0x02,
        DLPF_BANDWIDTH_41HZ = 0x03,
        DLPF_BANDWIDTH_20HZ = 0x04,
        DLPF_BANDWIDTH_10HZ = 0x05,
        DLPF_BANDWIDTH_5HZ = 0x06
    };
    enum AccelRange : uint8_t
    {
        ACCEL_RANGE_2G = 0x00,
        ACCEL_RANGE_4G = 0x08,
        ACCEL_RANGE_8G = 0x10,
        ACCEL_RANGE_16G = 0x18
    };
    enum GyroRange : uint8_t
    {
        GYRO_RANGE_250DPS = 0x00,
        GYRO_RANGE_500DPS = 0x08,
        GYRO_RANGE_1000DPS = 0x10,
        GYRO_RANGE_2000DPS = 0x18
    };
    MPU9250(I2C *bus, uint8_t addr);
    bool Begin();
    bool EnableDrdyInt();
    bool DisableDrdyInt();
    void ApplyRotation(const glm::mat3 &c) { rotation_ = c; }
    inline glm::mat3 rotation() const { return rotation_; }
    bool ConfigAccelRange(const AccelRange range);
    inline AccelRange accel_range() const { return accel_range_; }
    bool ConfigGyroRange(const GyroRange range);
    inline GyroRange gyro_range() const { return gyro_range_; }
    bool ConfigSrd(const uint8_t srd);
    inline uint8_t srd() const { return srd_; }
    bool ConfigDlpf(const DlpfBandwidth dlpf);
    inline DlpfBandwidth dlpf() const { return dlpf_bandwidth_; }
    void DrdyCallback(uint8_t int_pin, void (*function)());
    bool Read();

    inline glm::vec3 accel_mps2() const { return accel_mps2_; }
    inline glm::vec3 gyro_radps() const { return gyro_radps_; }
    inline glm::vec3 mag_ut() const { return mag_ut_; }
    inline float die_temperature_c() const { return die_temperature_c_; }

private:
    I2C *i2c_;
    uint8_t conn_;
    static constexpr uint8_t SPI_READ_ = 0x80;
    /* Configuration */
    glm::mat3 rotation_ = glm::mat3(1.0f);
    AccelRange accel_range_;
    GyroRange gyro_range_;
    DlpfBandwidth dlpf_bandwidth_;
    uint8_t srd_;
    static constexpr uint8_t WHOAMI_MPU9250_ = 0x71;
    static constexpr uint8_t WHOAMI_MPU9255_ = 0x73;
    static constexpr uint8_t WHOAMI_AK8963_ = 0x48;
    /* Data */
    float accel_scale_, gyro_scale_, mag_scale_[3];
    float temp_scale_ = 333.87f;
    glm::vec3 accel_mps2_;
    glm::vec3 gyro_radps_;
    glm::vec3 mag_ut_;
    float die_temperature_c_;
    /* Registers */
    static constexpr uint8_t PWR_MGMNT_1_ = 0x6B;
    static constexpr uint8_t H_RESET_ = 0x80;
    static constexpr uint8_t CLKSEL_PLL_ = 0x01;
    static constexpr uint8_t WHOAMI_ = 0x75;
    static constexpr uint8_t ACCEL_CONFIG_ = 0x1C;
    static constexpr uint8_t GYRO_CONFIG_ = 0x1B;
    static constexpr uint8_t ACCEL_CONFIG2_ = 0x1D;
    static constexpr uint8_t CONFIG_ = 0x1A;
    static constexpr uint8_t SMPLRT_DIV_ = 0x19;
    static constexpr uint8_t INT_PIN_CFG_ = 0x37;
    static constexpr uint8_t INT_ENABLE_ = 0x38;
    static constexpr uint8_t INT_DISABLE_ = 0x00;
    static constexpr uint8_t INT_PULSE_50US_ = 0x00;
    static constexpr uint8_t INT_RAW_RDY_EN_ = 0x01;
    static constexpr uint8_t INT_STATUS_ = 0x3A;
    static constexpr uint8_t RAW_DATA_RDY_INT_ = 0x01;
    static constexpr uint8_t USER_CTRL_ = 0x6A;
    static constexpr uint8_t I2C_MST_EN_ = 0x20;
    static constexpr uint8_t I2C_MST_CLK_ = 0x0D;
    static constexpr uint8_t I2C_MST_CTRL_ = 0x24;
    static constexpr uint8_t I2C_SLV0_ADDR_ = 0x25;
    static constexpr uint8_t I2C_SLV0_REG_ = 0x26;
    static constexpr uint8_t I2C_SLV0_CTRL_ = 0x27;
    static constexpr uint8_t I2C_SLV0_DO_ = 0x63;
    static constexpr uint8_t I2C_READ_FLAG_ = 0x80;
    static constexpr uint8_t I2C_SLV0_EN_ = 0x80;
    static constexpr uint8_t EXT_SENS_DATA_00_ = 0x49;
    /* AK8963 registers */
    static constexpr uint8_t AK8963_I2C_ADDR_ = 0x0C;
    static constexpr uint8_t AK8963_HXL_ = 0x03;
    static constexpr uint8_t AK8963_CNTL1_ = 0x0A;
    static constexpr uint8_t AK8963_PWR_DOWN_ = 0x00;
    static constexpr uint8_t AK8963_CNT_MEAS1_ = 0x12;
    static constexpr uint8_t AK8963_CNT_MEAS2_ = 0x16;
    static constexpr uint8_t AK8963_FUSE_ROM_ = 0x0F;
    static constexpr uint8_t AK8963_CNTL2_ = 0x0B;
    static constexpr uint8_t AK8963_RESET_ = 0x01;
    static constexpr uint8_t AK8963_ASA_ = 0x10;
    static constexpr uint8_t AK8963_WHOAMI_ = 0x00;
    bool WriteRegister(uint8_t reg, uint8_t data);
    bool ReadRegisters(uint8_t reg, uint8_t count, uint8_t *data);
    bool WriteAk8963Register(uint8_t reg, uint8_t data);
    bool ReadAk8963Registers(uint8_t reg, uint8_t count, uint8_t *data);
};