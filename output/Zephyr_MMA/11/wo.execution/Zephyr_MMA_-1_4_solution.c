
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>

#define MMA8x5x_I2C_ADDRESS 0x1D
#define MMA8x5x_REG_OUT_X_MSB 0x01

LOG_MODULE_REGISTER(main);

struct nvs_fs fs;

void read_mma8x5x(const struct device *i2c_dev, uint8_t *value) {
    uint8_t reg_addr = MMA8x5x_REG_OUT_X_MSB;
    i2c_reg_read_byte(i2c_dev, MMA8x5x_I2C_ADDRESS, reg_addr, value);
}

int main(void) {
    const struct device *i2c_dev = device_get_binding("I2C_1");
    if (!i2c_dev) {
        LOG_ERR("Could not get I2C device");
        return -1;
    }

    fs.flash_device = (const struct device *)flash_area_open(0, NULL);
    if (!fs.flash_device) {
        LOG_ERR("Could not get flash device");
        return -1;
    }

    fs.offset = 0x0;
    fs.sector_size = 0x1000;
    fs.sector_count = 0x10;

    int ret = nvs_init(&fs, "storage");
    if (ret) {
        LOG_ERR("NVS init failed: %d", ret);
        return -1;
    }

    while (1) {
        uint8_t value;
        read_mma8x5x(i2c_dev, &value);
        LOG_INF("Read value: %d", value);

        ret = nvs_write(&fs, 0, &value, sizeof(value));
        if (ret < 0) {
            LOG_ERR("NVS write failed: %d", ret);
        }

        k_sleep(K_SECONDS(5));
    }

    return 0;
}
