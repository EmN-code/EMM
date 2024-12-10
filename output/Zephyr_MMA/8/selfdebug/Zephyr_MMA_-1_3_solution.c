
#include <zephyr.h>
#include <device.h>
#include <drivers/i2c.h>
#include <storage/flash_map.h>
#include <fs/nvs.h>
#include <logging/log.h>

#define MMA8x5x_I2C_ADDRESS 0x1D
#define MMA8x5x_REG_OUT_X_MSB 0x01
#define NVS_PARTITION_ID 0

LOG_MODULE_REGISTER(main);

const struct device *i2c_dev;
struct nvs_fs fs;

void read_mma8x5x(void) {
    uint8_t data[6];
    int ret;

    ret = i2c_burst_read(i2c_dev, MMA8x5x_I2C_ADDRESS, MMA8x5x_REG_OUT_X_MSB, data, sizeof(data));
    if (ret < 0) {
        LOG_ERR("Failed to read from MMA8x5x sensor: %d", ret);
        return;
    }

    int16_t x = (data[0] << 8) | data[1];
    int16_t y = (data[2] << 8) | data[3];
    int16_t z = (data[4] << 8) | data[5];

    LOG_INF("MMA8x5x: X=%d, Y=%d, Z=%d", x, y, z);

    // Store the read value persistently in NVS
    ret = nvs_write(&fs, 0, &x, sizeof(x));
    if (ret < 0) {
        LOG_ERR("Failed to write to NVS: %d", ret);
    } else {
        LOG_INF("Value written to NVS: %d", x);
    }
}

void main(void) {
    int ret;

    // Initialize I2C device
    i2c_dev = device_get_binding("I2C_0");
    if (!i2c_dev) {
        LOG_ERR("I2C device not found");
        return;
    }

    // Initialize NVS
    fs.flash_device = FLASH_AREA_ID(NVS_PARTITION_ID);
    fs.offset = 0;
    fs.sector_size = 4096;
    fs.sector_count = 1;

    ret = nvs_init(&fs, fs.flash_device);
    if (ret) {
        LOG_ERR("NVS init failed: %d", ret);
        return;
    }

    // Read MMA8x5x every 5 seconds
    while (1) {
        read_mma8x5x();
        k_sleep(K_SECONDS(5));
    }
}
