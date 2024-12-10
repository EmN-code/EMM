
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/printk.h>

#define MMA8x5x_I2C_ADDRESS 0x1D
#define MMA8x5x_REG_OUT_X_MSB 0x01

#define STORAGE_NODE DT_FLASH_AREA_STORAGE_NODE
#define STORAGE_SIZE DT_REG_SIZE(STORAGE_NODE)
#define STORAGE_OFFSET DT_REG_ADDR(STORAGE_NODE)

LOG_MODULE_REGISTER(main);

const struct device *i2c_dev;
struct nvs_fs fs;

void init_nvs(void) {
    fs.flash_device = FLASH_AREA_DEVICE(storage);
    fs.offset = STORAGE_OFFSET;
    fs.sector_size = STORAGE_SIZE;
    fs.sector_count = 1;

    int ret = nvs_init(&fs, "storage");
    if (ret) {
        LOG_ERR("NVS init failed: %d", ret);
    } else {
        LOG_INF("NVS init successful");
    }
}

uint8_t read_mma8x5x(void) {
    uint8_t value;
    int ret = i2c_reg_read_byte(i2c_dev, MMA8x5x_I2C_ADDRESS, MMA8x5x_REG_OUT_X_MSB, &value);
    if (ret) {
        LOG_ERR("Failed to read from MMA8x5x: %d", ret);
        return 0;
    }
    return value;
}

void store_value(uint8_t value) {
    int ret = nvs_write(&fs, 0, &value, sizeof(value));
    if (ret < 0) {
        LOG_ERR("Failed to write to NVS: %d", ret);
    } else {
        LOG_INF("Value stored successfully");
    }
}

int main(void) {
    i2c_dev = device_get_binding("I2C_0");
    if (!i2c_dev) {
        LOG_ERR("I2C device not found");
        return -1;
    }

    init_nvs();

    while (1) {
        uint8_t value = read_mma8x5x();
        store_value(value);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
