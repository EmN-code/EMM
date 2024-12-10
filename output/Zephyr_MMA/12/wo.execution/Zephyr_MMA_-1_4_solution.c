
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

#define STORAGE_PARTITION flash_area_id(2)

LOG_MODULE_REGISTER(main);

struct nvs_fs fs;

int read_mma8x5x(const struct device *i2c_dev, uint8_t *value) {
    return i2c_reg_read_byte(i2c_dev, MMA8x5x_I2C_ADDRESS, MMA8x5x_REG_OUT_X_MSB, value);
}

int main(void) {
    const struct device *i2c_dev = DEVICE_DT_GET(DT_NODELABEL(i2c0));
    if (!device_is_ready(i2c_dev)) {
        LOG_ERR("I2C device not ready");
        return -1;
    }

    struct flash_sector sector;
    if (flash_get_sector_info_by_id(STORAGE_PARTITION, 0, &sector) != 0) {
        LOG_ERR("Failed to get flash sector info");
        return -1;
    }

    fs.flash_device = DEVICE_DT_GET(DT_CHOSEN(zephyr_flash_controller));
    if (!device_is_ready(fs.flash_device)) {
        LOG_ERR("Flash device not ready");
        return -1;
    }

    fs.offset = sector.start;
    fs.sector_size = sector.size;
    fs.sector_count = 1;

    if (nvs_init(&fs, NULL) != 0) {
        LOG_ERR("NVS init failed");
        return -1;
    }

    while (1) {
        uint8_t value;
        if (read_mma8x5x(i2c_dev, &value) == 0) {
            LOG_INF("Read value: %d", value);
            if (nvs_write(&fs, 1, &value, sizeof(value)) != 0) {
                LOG_ERR("NVS write failed");
            }
        } else {
            LOG_ERR("Failed to read from MMA8x5x");
        }
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
