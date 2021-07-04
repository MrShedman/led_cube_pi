#include "mpu9250.hpp"

#include "chrono/time.h"

namespace
{
    const float DEG_TO_RAD = 0.017453292519943295769236907684886f;
    const float GRAVITY = 9.81f; 
    void delay(int ms)
    {
        sleep(milliseconds(ms));
    }
}

MPU9250::MPU9250(I2C *bus, uint8_t addr)
{
    i2c_ = bus;
    conn_ = addr;
}

bool MPU9250::Begin()
{
    /* Select clock source to gyro */
    if (!WriteRegister(PWR_MGMNT_1_, CLKSEL_PLL_))
    {
        return false;
    }
    /* Enable I2C master mode */
    if (!WriteRegister(USER_CTRL_, I2C_MST_EN_))
    {
        return false;
    }
    /* Set the I2C bus speed to 400 kHz */
    if (!WriteRegister(I2C_MST_CTRL_, I2C_MST_CLK_))
    {
        return false;
    }
    /* Set AK8963 to power down */
    WriteAk8963Register(AK8963_CNTL1_, AK8963_PWR_DOWN_);
    /* Reset the MPU9250 */
    WriteRegister(PWR_MGMNT_1_, H_RESET_);
    /* Wait for MPU-9250 to come back up */
    delay(1);
    /* Reset the AK8963 */
    WriteAk8963Register(AK8963_CNTL2_, AK8963_RESET_);
    /* Select clock source to gyro */
    if (!WriteRegister(PWR_MGMNT_1_, CLKSEL_PLL_))
    {
        return false;
    }
    /* Check the WHO AM I byte */
    uint8_t who_am_i;
    if (!ReadRegisters(WHOAMI_, sizeof(who_am_i), &who_am_i))
    {
        return false;
    }
    if ((who_am_i != WHOAMI_MPU9250_) && (who_am_i != WHOAMI_MPU9255_))
    {
        return false;
    }
    /* Enable I2C master mode */
    if (!WriteRegister(USER_CTRL_, I2C_MST_EN_))
    {
        return false;
    }
    /* Set the I2C bus speed to 400 kHz */
    if (!WriteRegister(I2C_MST_CTRL_, I2C_MST_CLK_))
    {
        return false;
    }
    /* Check the AK8963 WHOAMI */
    if (!ReadAk8963Registers(AK8963_WHOAMI_, sizeof(who_am_i), &who_am_i))
    {
        return false;
    }
    if (who_am_i != WHOAMI_AK8963_)
    {
        return false;
    }
    /* Get the magnetometer calibration */
    /* Set AK8963 to power down */
    if (!WriteAk8963Register(AK8963_CNTL1_, AK8963_PWR_DOWN_))
    {
        return false;
    }
    delay(100); // long wait between AK8963 mode changes
    /* Set AK8963 to FUSE ROM access */
    if (!WriteAk8963Register(AK8963_CNTL1_, AK8963_FUSE_ROM_))
    {
        return false;
    }
    delay(100); // long wait between AK8963 mode changes
    /* Read the AK8963 ASA registers and compute magnetometer scale factors */
    uint8_t asa_buff[3];
    if (!ReadAk8963Registers(AK8963_ASA_, sizeof(asa_buff), asa_buff))
    {
        return false;
    }
    mag_scale_[0] = ((static_cast<float>(asa_buff[0]) - 128.0f) / 256.0f + 1.0f) * 4912.0f / 32760.0f;
    mag_scale_[1] = ((static_cast<float>(asa_buff[1]) - 128.0f) / 256.0f + 1.0f) * 4912.0f / 32760.0f;
    mag_scale_[2] = ((static_cast<float>(asa_buff[2]) - 128.0f) / 256.0f + 1.0f) * 4912.0f / 32760.0f;
    /* Set AK8963 to power down */
    if (!WriteAk8963Register(AK8963_CNTL1_, AK8963_PWR_DOWN_))
    {
        return false;
    }
    /* Set AK8963 to 16 bit resolution, 100 Hz update rate */
    if (!WriteAk8963Register(AK8963_CNTL1_, AK8963_CNT_MEAS2_))
    {
        return false;
    }
    delay(100); // long wait between AK8963 mode changes
    /* Select clock source to gyro */
    if (!WriteRegister(PWR_MGMNT_1_, CLKSEL_PLL_))
    {
        return false;
    }
    /* Instruct the MPU9250 to get 7 bytes from the AK8963 at the sample rate */
    uint8_t mag_data[7];
    if (!ReadAk8963Registers(AK8963_HXL_, sizeof(mag_data), mag_data))
    {
        return false;
    }
    /* Set the accel range to 16G by default */
    if (!ConfigAccelRange(ACCEL_RANGE_4G))
    {
        return false;
    }
    /* Set the gyro range to 2000DPS by default*/
    if (!ConfigGyroRange(GYRO_RANGE_250DPS))
    {
        return false;
    }
    /* Set the DLPF to 20HZ by default */
    if (!ConfigDlpf(DLPF_BANDWIDTH_184HZ))
    {
        return false;
    }
    /* Set the SRD to 0 by default */
    if (!ConfigSrd(4))
    {
        return false;
    }
    return true;
}
bool MPU9250::EnableDrdyInt()
{
    if (!WriteRegister(INT_PIN_CFG_, INT_PULSE_50US_))
    {
        return false;
    }
    if (!WriteRegister(INT_ENABLE_, INT_RAW_RDY_EN_))
    {
        return false;
    }
    return true;
}
bool MPU9250::DisableDrdyInt()
{
    if (!WriteRegister(INT_ENABLE_, INT_DISABLE_))
    {
        return false;
    }
    return true;
}
bool MPU9250::ConfigAccelRange(const AccelRange range)
{
    AccelRange requested_range;
    float requested_scale;
    /* Check input is valid and set requested range and scale */
    switch (range)
    {
    case ACCEL_RANGE_2G:
    {
        requested_range = range;
        requested_scale = 2.0f / 32767.5f;
        break;
    }
    case ACCEL_RANGE_4G:
    {
        requested_range = range;
        requested_scale = 4.0f / 32767.5f;
        break;
    }
    case ACCEL_RANGE_8G:
    {
        requested_range = range;
        requested_scale = 8.0f / 32767.5f;
        break;
    }
    case ACCEL_RANGE_16G:
    {
        requested_range = range;
        requested_scale = 16.0f / 32767.5f;
        break;
    }
    default:
    {
        return false;
    }
    }
    /* Try setting the requested range */
    if (!WriteRegister(ACCEL_CONFIG_, requested_range))
    {
        return false;
    }
    /* Update stored range and scale */
    accel_range_ = requested_range;
    accel_scale_ = requested_scale;
    return true;
}
bool MPU9250::ConfigGyroRange(const GyroRange range)
{
    GyroRange requested_range;
    float requested_scale;
    /* Check input is valid and set requested range and scale */
    switch (range)
    {
    case GYRO_RANGE_250DPS:
    {
        requested_range = range;
        requested_scale = 250.0f / 32767.5f;
        break;
    }
    case GYRO_RANGE_500DPS:
    {
        requested_range = range;
        requested_scale = 500.0f / 32767.5f;
        break;
    }
    case GYRO_RANGE_1000DPS:
    {
        requested_range = range;
        requested_scale = 1000.0f / 32767.5f;
        break;
    }
    case GYRO_RANGE_2000DPS:
    {
        requested_range = range;
        requested_scale = 2000.0f / 32767.5f;
        break;
    }
    default:
    {
        return false;
    }
    }
    /* Try setting the requested range */
    if (!WriteRegister(GYRO_CONFIG_, requested_range))
    {
        return false;
    }
    /* Update stored range and scale */
    gyro_range_ = requested_range;
    gyro_scale_ = requested_scale;
    return true;
}
bool MPU9250::ConfigSrd(const uint8_t srd)
{
    /* Changing the SRD to allow us to set the magnetometer successfully */
    if (!WriteRegister(SMPLRT_DIV_, 19))
    {
        return false;
    }
    /* Set the magnetometer sample rate */
    if (srd > 9)
    {
        /* Set AK8963 to power down */
        WriteAk8963Register(AK8963_CNTL1_, AK8963_PWR_DOWN_);
        delay(100); // long wait between AK8963 mode changes
        /* Set AK8963 to 16 bit resolution, 8 Hz update rate */
        if (!WriteAk8963Register(AK8963_CNTL1_, AK8963_CNT_MEAS1_))
        {
            return false;
        }
        delay(100); // long wait between AK8963 mode changes
        /* Instruct the MPU9250 to get 7 bytes from the AK8963 at the sample rate */
        uint8_t mag_data[7];
        if (!ReadAk8963Registers(AK8963_HXL_, sizeof(mag_data), mag_data))
        {
            return false;
        }
    }
    else
    {
        /* Set AK8963 to power down */
        WriteAk8963Register(AK8963_CNTL1_, AK8963_PWR_DOWN_);
        delay(100); // long wait between AK8963 mode changes
        /* Set AK8963 to 16 bit resolution, 100 Hz update rate */
        if (!WriteAk8963Register(AK8963_CNTL1_, AK8963_CNT_MEAS2_))
        {
            return false;
        }
        delay(100); // long wait between AK8963 mode changes
        /* Instruct the MPU9250 to get 7 bytes from the AK8963 at the sample rate */
        uint8_t mag_data[7];
        if (!ReadAk8963Registers(AK8963_HXL_, sizeof(mag_data), mag_data))
        {
            return false;
        }
    }
    /* Set the IMU sample rate */
    if (!WriteRegister(SMPLRT_DIV_, srd))
    {
        return false;
    }
    srd_ = srd;
    return true;
}
bool MPU9250::ConfigDlpf(const DlpfBandwidth dlpf)
{
    DlpfBandwidth requested_dlpf;
    /* Check input is valid and set requested dlpf */
    switch (dlpf)
    {
    case DLPF_BANDWIDTH_184HZ:
    {
        requested_dlpf = dlpf;
        break;
    }
    case DLPF_BANDWIDTH_92HZ:
    {
        requested_dlpf = dlpf;
        break;
    }
    case DLPF_BANDWIDTH_41HZ:
    {
        requested_dlpf = dlpf;
        break;
    }
    case DLPF_BANDWIDTH_20HZ:
    {
        requested_dlpf = dlpf;
        break;
    }
    case DLPF_BANDWIDTH_10HZ:
    {
        requested_dlpf = dlpf;
        break;
    }
    case DLPF_BANDWIDTH_5HZ:
    {
        requested_dlpf = dlpf;
        break;
    }
    default:
    {
        return false;
    }
    }
    /* Try setting the dlpf */
    if (!WriteRegister(ACCEL_CONFIG2_, requested_dlpf))
    {
        return false;
    }
    if (!WriteRegister(CONFIG_, requested_dlpf))
    {
        return false;
    }
    /* Update stored dlpf */
    dlpf_bandwidth_ = requested_dlpf;
    return true;
}

void MPU9250::DrdyCallback(uint8_t int_pin, void (*function)())
{
    //   pinMode(int_pin, INPUT);
    //   attachInterrupt(int_pin, function, RISING);
}

bool MPU9250::Read()
{
    /* Read the data registers */
    uint8_t data_buff[22];
    if (!ReadRegisters(INT_STATUS_, sizeof(data_buff), data_buff))
    {
        return false;
    }
    /* Check if data is ready */
    bool data_ready = (data_buff[0] & RAW_DATA_RDY_INT_);
    if (!data_ready)
    {
        return false;
    }
    /* Unpack the buffer */
    int16_t accel_counts[3], gyro_counts[3], temp_counts, mag_counts[3];
    accel_counts[0] = static_cast<int16_t>(data_buff[1]) << 8 | data_buff[2];
    accel_counts[1] = static_cast<int16_t>(data_buff[3]) << 8 | data_buff[4];
    accel_counts[2] = static_cast<int16_t>(data_buff[5]) << 8 | data_buff[6];
    temp_counts = static_cast<int16_t>(data_buff[7]) << 8 | data_buff[8];
    gyro_counts[0] = static_cast<int16_t>(data_buff[9]) << 8 | data_buff[10];
    gyro_counts[1] = static_cast<int16_t>(data_buff[11]) << 8 | data_buff[12];
    gyro_counts[2] = static_cast<int16_t>(data_buff[13]) << 8 | data_buff[14];
    mag_counts[0] = static_cast<int16_t>(data_buff[16]) << 8 | data_buff[15];
    mag_counts[1] = static_cast<int16_t>(data_buff[18]) << 8 | data_buff[17];
    mag_counts[2] = static_cast<int16_t>(data_buff[20]) << 8 | data_buff[19];
    /* Convert to float values and rotate the accel / gyro axis */
    glm::vec3 accel, gyro, mag;
    float temp;
    accel.x = static_cast<float>(accel_counts[0]) * GRAVITY * accel_scale_;
    accel.y = static_cast<float>(accel_counts[1]) * GRAVITY * accel_scale_;
    accel.z = static_cast<float>(accel_counts[2]) * GRAVITY * accel_scale_;

    gyro.x = static_cast<float>(gyro_counts[0]) * DEG_TO_RAD * gyro_scale_;
    gyro.y = static_cast<float>(gyro_counts[1]) * DEG_TO_RAD * gyro_scale_;
    gyro.z = static_cast<float>(gyro_counts[2]) * DEG_TO_RAD * gyro_scale_;

    mag.x = static_cast<float>(mag_counts[0]) * mag_scale_[0];
    mag.y = static_cast<float>(mag_counts[1]) * mag_scale_[1];
    mag.z = static_cast<float>(mag_counts[2]) * mag_scale_[2];

    die_temperature_c_ = (static_cast<float>(temp_counts) - 21.0f) / temp_scale_ + 21.0f;
   
    /* Apply rotation */
    accel_mps2_ = rotation_ * accel;
    gyro_radps_ = rotation_ * gyro;
    mag_ut_ = rotation_ * mag;
    return true;
}

#include <iostream>

bool MPU9250::WriteRegister(uint8_t reg, uint8_t data)
{
    uint8_t ret_val;
    //std::cout << reg << std::endl;

    // i2c_->beginTransmission(conn_);
    // i2c_->write(reg);
    // i2c_->write(data);
    // i2c_->endTransmission();
    i2c_->writeReg8(reg, data);

    delay(10);
    ReadRegisters(reg, sizeof(ret_val), &ret_val);
    if (data == ret_val)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool MPU9250::ReadRegisters(uint8_t reg, uint8_t count, uint8_t *data)
{
    i2c_->readRegMulti(reg, count, data);
    return true;
    // i2c_->beginTransmission(conn_);
    // i2c_->write(reg);
    // i2c_->endTransmission(false);
    // uint8_t bytes_rx = i2c_->requestFrom(conn_, count);
    // if (bytes_rx == count) {
    //   for (std::size_t i = 0; i < count; i++) {
    //     data[i] = i2c_->read();
    //   }
    //   return true;
    // } else {read(data, count);
    //   return false;
    // }
}

bool MPU9250::WriteAk8963Register(uint8_t reg, uint8_t data)
{
    uint8_t ret_val;
    if (!WriteRegister(I2C_SLV0_ADDR_, AK8963_I2C_ADDR_))
    {
        return false;
    }
    if (!WriteRegister(I2C_SLV0_REG_, reg))
    {
        return false;
    }
    if (!WriteRegister(I2C_SLV0_DO_, data))
    {
        return false;
    }
    if (!WriteRegister(I2C_SLV0_CTRL_, I2C_SLV0_EN_ | sizeof(data)))
    {
        return false;
    }
    if (!ReadAk8963Registers(reg, sizeof(ret_val), &ret_val))
    {
        return false;
    }
    if (data == ret_val)
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool MPU9250::ReadAk8963Registers(uint8_t reg, uint8_t count, uint8_t *data)
{
    if (!WriteRegister(I2C_SLV0_ADDR_, AK8963_I2C_ADDR_ | I2C_READ_FLAG_))
    {
        return false;
    }
    if (!WriteRegister(I2C_SLV0_REG_, reg))
    {
        return false;
    }
    if (!WriteRegister(I2C_SLV0_CTRL_, I2C_SLV0_EN_ | count))
    {
        return false;
    }
    delay(1);
    return ReadRegisters(EXT_SENS_DATA_00_, count, data);
}