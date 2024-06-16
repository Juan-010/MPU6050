/**
 * @file bmp280.h
 * @brief BMP280 sensor driver.
 * @author Enrique Walter Philippeaux & Juan Ignacio Sanchez Serantes
 * @date 2024-06-10
 */

#ifndef BMP280_H
#define BMP280_H

#include <stdint.h>
#include <driver/spi_master.h>
#include "calibration.h"
#include "registers.h"

class BMP280
{
public:
    /**
     * @brief BMP280 sensor modes.
     */
    enum Mode
    {
        LOW_POWER = 0, //!< Low power portable device. 
        DYNAMIC = 1,   //!< Dynamic portable device.
        WEATHER = 2,   //!< Weather station.
        ELEVATOR = 3,  //!< Floor change detection.
        DROP = 4,      //!< Drop detection.
        NAVIGATION = 5 //!< Indoor navigation.
    };

    /**
     * @brief Construct a new BMP280 object.
     */
    BMP280(){
        
    }

    /**
     * @brief Initialize the BMP280 sensor.
     * @param spi SPI device handle.
     * @param mode Sensor mode.
     * @return 0 on success, -1 on error.
     * @note This function must be called before calling any other function.
     * @note The sensor must be powered on before calling this function.
     * @note The sensor must be connected to the SPI bus before calling this function.
     * @note The SPI bus must be initialized before calling this function.
     * @note The SPI device must be added to the SPI bus before calling this function.
     * @note The SPI device must be configured before calling this function.
     */
    esp_err_t init(Mode mode);

    /**
     * @brief Set the sensor mode.
     * @param mode Sensor mode.
     * @return 0 on success, -1 on error.
     */
    esp_err_t setMode(Mode mode);
    

    /**
     * @brief Get the SPI device handle.
     * @return SPI device handle.
     */
    spi_device_handle_t *getSPIHandle()
    {
        return &_spi;
    }

    /**
     * @brief Read all data from the sensor and perform required compensations.
     * @return 0 on success, -1 on error.
     */
    esp_err_t read();

    /**
     * @brief Get the temperature in degrees Celsius.
     * @return temperature in DegC, resolution is 0.01 DegC. Output value of “5123” equals 51.23 DegC.
     */
    int32_t getTemp(){
        return _temp;
    }

    /**
     * @brief Get the temperature in degrees Celsius.
     * @return float temperature in DegC.
     */
    float getTempF(){
        return _temp / 100.0f;
    }

    /**
     * @brief Get the pressure in Pa.
     * @return Returns pressure in Pa as unsigned 32 bit integer. Output value of “96386” equals 96386 Pa = 963.86 hPa
     */
    uint32_t getPress()
    {
        return _press;
    }

    /**
     * @brief Get the pressure in Pa.
     * @return returns pressure in Pa as float.
     */
    float getPressF()
    {
        return _press / 100.0f;
    }

protected:
    /**
     * @brief Read calibration data from the sensor.
     */
    void readCalibrationData();
    
    /**
     * @brief Write data to a register of the BMP280 sensor.
     * @param reg Register address.
     * @param data Data to write.
     * @return 0 on success, -1 on error.
     */
    esp_err_t writeReg(uint8_t reg, uint8_t data);

    /**
     * @brief Write data to a register of the BMP280 sensor.
     * @param reg Register address.
     * @param buffer Data to write.
     * @param n Number of bytes to write.
     * @return 0 on success, -1 on error.
     */
    esp_err_t writeReg(uint8_t reg, uint8_t *buffer, uint8_t n);

    /**
     * @brief Read data from a register of the BMP280 sensor.
     * @param reg Register address.
     * @return Data read from the register.
     */
    uint8_t readReg(uint8_t reg);

    /**
     * @brief Read data from n registers of the BMP280 sensor.
     * @param reg Register address.
     * @param buffer Buffer to store the data read.
     * @param n Number of bytes to read.
     * @return 0 on success, -1 on error.
     */
    uint8_t readReg(uint8_t reg, uint8_t *buffer, uint8_t n);

    /**
     * @brief Compensate the temperature.
     * @param uncompensatedTemp Uncompensated temperature.
     * @return Compensated temperature.
     */
    int32_t compensateTemp(int32_t uncompensatedTemp);

    /**
     * @brief Compensate the pressure.
     * @param uncompensatedPress Uncompensated pressure.
     * @return Compensated pressure.
     */
    uint32_t compensatePress(int32_t uncompensatedPress);

private:
    spi_device_handle_t _spi;
    struct bmp280_calib_param _calib_data;
    int32_t _t_fine;
    Mode _mode;

    int32_t _temp;
    uint32_t _press;
    uint8_t __read_buffer[6];
};

#endif // BMP280_H