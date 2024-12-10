
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/flash.h>

#define MMA8x5x_I2C_ADDRESS 0x1D
#define MMA8x5x_REG_OUT_X_MSB 0x01

LOG_MODULE_REGISTER(main);

void read_mma8x5x(const struct device *i2c_dev, uint8_t *value) {
    uint8_t reg_addr = MMA8x5x_REG_OUT_X_MSB;
    i2c_reg_read_byte(i2c_dev, MMA8x5x_I2C_ADDRESS, reg_addr, value);
}

int main(void) {
    const struct device *i2c_dev = DEVICE_DT_GET(DT_NODELABEL(i2c0));
    if (!device_is_ready(i2c_dev)) {
        LOG_ERR("I2C device not ready");
        return -1;
    }

    struct nvs_fs fs;
    fs.flash_device = (void *)FLASH_AREA_ID(storage_partition);
    fs.offset = FIXED_PARTITION_OFFSET(storage_partition);
    fs.sector_size = 4096;
    fs.sector_count = 2;

    int ret = nvs_init(&fs, fs.flash_device);
    if (ret) {
        LOG_ERR("NVS init failed: %d", ret);
        return -1;
    }

    uint8_t value;
    while (1) {
        read_mma8x5x(i2c_dev, &value);
        LOG_INF("Read value: %d", value);

        ret = nvs_write(&fs, 1, &value, sizeof(value));
        if (ret < 0) {
            LOG_ERR("NVS write failed: %d", ret);
        }

        k_sleep(K_SECONDS(5));
    }

    return 0;
}
