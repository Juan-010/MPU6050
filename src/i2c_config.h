#ifndef I2C_CONFIG_H
#define I2C_CONFIG_H

#include "driver/i2c.h"

// I2C master configuration
#define I2C_MASTER_SCL_IO 22       // GPIO number for I2C master clock
#define I2C_MASTER_SDA_IO 21       // GPIO number for I2C master data
#define I2C_MASTER_NUM I2C_NUM_0   // I2C port number for master dev
#define I2C_MASTER_FREQ_HZ 100000  // I2C master clock frequency
#define I2C_MASTER_TX_BUF_DISABLE 0
#define I2C_MASTER_RX_BUF_DISABLE 0
esp_err_t i2c_master_init(void);


#endif