
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>

#define MMA8x5x_I2C_ADDRESS 0x1D
#define MMA8x5x_REG_OUT_X_MSB 0x01

LOG_MODULE_REGISTER(main);

static struct nvs_fs fs = {
    .sector_size = DT_FLASH_AREA_STORAGE_SIZE,
    .sector_count = 3,
    .offset = DT_FLASH_AREA_STORAGE_OFFSET,
};

void read_mma8x5x(const struct device *i2c_dev, uint8_t *value) {
    uint8_t reg_addr = MMA8x5x_REG_OUT_X_MSB;
    i2c_reg_read_byte(i2c_dev, MMA8x5x_I2C_ADDRESS, reg_addr, value);
}

void store_value_in_nvs(uint8_t value) {
    nvs_write(&fs, 0, &value, sizeof(value));
}

int main(void) {
    const struct device *i2c_dev = device_get_binding("I2C_1");
    if (!i2c_dev) {
        LOG_ERR("I2C device not found");
        return -1;
    }

    int ret = nvs_init(&fs, DT_FLASH_AREA_STORAGE_LABEL);
    if (ret) {
        LOG_ERR("NVS init failed: %d", ret);
        return -1;
    }

    while (1) {
        uint8_t value;
        read_mma8x5x(i2c_dev, &value);
        store_value_in_nvs(value);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
