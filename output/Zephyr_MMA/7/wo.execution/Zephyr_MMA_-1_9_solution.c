
#include <zephyr/types.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>
#include <zephyr/kernel.h>

#define MMA8x5x_I2C_ADDRESS 0x1D
#define MMA8x5x_REG_OUT_X_MSB 0x01

LOG_MODULE_REGISTER(main);

static struct nvs_fs fs;

void init_nvs(void) {
    int err;
    struct flash_pages_info info;

    fs.flash_device = FLASH_AREA_ID(storage);
    fs.offset = FIXED_PARTITION_OFFSET(storage);
    fs.sector_size = flash_get_page_info_by_offs(device_get_binding(fs.flash_device), fs.offset, &info);
    fs.sector_count = FIXED_PARTITION_SIZE(storage) / fs.sector_size;

    err = nvs_init(&fs, fs.flash_device);
    if (err) {
        LOG_ERR("NVS init failed: %d", err);
    } else {
        LOG_INF("NVS init successful");
    }
}

void read_mma8x5x(void) {
    const struct device *i2c_dev = device_get_binding("I2C_1");
    uint8_t value;
    int err;

    if (!device_is_ready(i2c_dev)) {
        LOG_ERR("I2C device not ready");
        return;
    }

    err = i2c_reg_read_byte(i2c_dev, MMA8x5x_I2C_ADDRESS, MMA8x5x_REG_OUT_X_MSB, &value);
    if (err) {
        LOG_ERR("Failed to read from MMA8x5x: %d", err);
    } else {
        LOG_INF("MMA8x5x read value: %d", value);

        err = nvs_write(&fs, 1, &value, sizeof(value));
        if (err) {
            LOG_ERR("NVS write failed: %d", err);
        } else {
            LOG_INF("NVS write successful");
        }
    }
}

int main(void) {
    init_nvs();

    while (1) {
        read_mma8x5x();
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
