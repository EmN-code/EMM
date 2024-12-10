
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

struct nvs_fs fs;

void read_mma8x5x(const struct device *i2c_dev, uint8_t *value) {
    uint8_t reg_addr = MMA8x5x_REG_OUT_X_MSB;
    i2c_reg_read_byte(i2c_dev, MMA8x5x_I2C_ADDRESS, reg_addr, value);
}

void store_value_in_nvs(uint8_t value) {
    int ret = nvs_write(&fs, 0, &value, sizeof(value));
    if (ret < 0) {
        LOG_ERR("NVS write failed: %d", ret);
    } else {
        LOG_INF("NVS write successful");
    }
}

int main(void) {
    const struct device *i2c_dev = DEVICE_DT_GET(DT_NODELABEL(i2c0));
    if (!device_is_ready(i2c_dev)) {
        LOG_ERR("I2C device not ready");
        return -1;
    }

    struct flash_sector sector;
    int ret = flash_get_sector_info_by_id(FLASH_AREA_ID(storage), 0, &sector);
    if (ret) {
        LOG_ERR("Failed to get flash sector info: %d", ret);
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

    ret = nvs_init(&fs, NULL);
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
