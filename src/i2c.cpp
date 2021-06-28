#include "i2c.hpp"

#include <stdexcept>

I2C::I2C(const std::string &dev_path, uint8_t addr)

    : m_dev_path(dev_path), m_addr(addr)
{

    // Open device file
    m_dev_fd = open(m_dev_path.c_str(), O_RDWR);

    if (m_dev_fd < 0)
        throw std::runtime_error("(Fatal) `I2C`: can't open I2C device\n");

    // Select slave device
    if (ioctl(m_dev_fd, I2C_SLAVE, addr) < 0)
        throw std::runtime_error("(Fatal) `I2C`: can't select slave device\n");
}

I2C::~I2C()
{
    close(m_dev_fd);
}

void I2C::write(uint8_t *data, uint8_t size)
{

    if (::write(m_dev_fd, data, size) != size)
        throw std::runtime_error("(Fatal) `I2C`: failed to write to the bus\n");
}

void I2C::read(uint8_t *data, uint8_t size)
{

    if (::read(m_dev_fd, data, size) != size)
        throw std::runtime_error("(Fatal) `I2C`: failed to read from the bus\n");
}

void I2C::writeReg8(uint8_t reg, uint8_t data)
{

    // Build the buffer to send
    m_buffer[0] = reg;
    m_buffer[1] = data;

    // Write the data on the device
    write(m_buffer, 2);
}

uint8_t I2C::readReg8(uint8_t reg)
{

    // Select the register on the device
    write(&reg, 1);

    // Read the data from the device
    read(m_buffer, 1);

    return m_buffer[0];
}

void I2C::writeReg16(uint8_t reg, uint16_t data)
{
    // Build the buffer to send
    m_buffer[0] = reg;
    m_buffer[1] = data & 0xFF;
    m_buffer[2] = (data >> 8) & 0xFF;

    // Write the data on the device
    write(m_buffer, 2);
}

uint16_t I2C::readReg16(uint8_t reg)
{
    // Select the register on the device
    write(&reg, 1);

    // Read the data from the device
    read(m_buffer, 2);

    // Merge the 16 bit data
    return (uint16_t)m_buffer[0] | ((uint16_t)m_buffer[1] << 8);
}

void I2C::readRegMulti(uint8_t reg, uint8_t count, uint8_t *data)
{
    // Select the register on the device
    write(&reg, 1);

    read(data, count);
}