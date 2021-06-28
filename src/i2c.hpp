#pragma once

#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <string>

class I2C
{

public:
    I2C(const std::string &dev_path, uint8_t addr);

    virtual ~I2C();

    void write(uint8_t *data, uint8_t size);

    void read(uint8_t *data, uint8_t size);

    void writeReg8(uint8_t reg, uint8_t data);

    uint8_t readReg8(uint8_t reg);

    void writeReg16(uint8_t reg, uint16_t data);

    uint16_t readReg16(uint8_t reg);

    void readRegMulti(uint8_t reg, uint8_t count, uint8_t* data);

private:
    std::string m_dev_path;

    uint32_t m_addr;

    int m_dev_fd;

    uint8_t m_buffer[3];
};