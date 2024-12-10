
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/flash.h>
#include <time.h>

#define MMA8x5x_I2C_ADDRESS 0x1D
#define MMA8x5x_REG_OUT_X_MSB 0x01
#define NVS_PARTITION_ID 1

LOG_MODULE_REGISTER(main);

const struct device *i2c_dev;
struct nvs_fs nvs;

int read_mma8x5x(uint8_t *value) {
    return i2c_reg_read_byte(i2c_dev, MMA8x5x_I2C_ADDRESS, MMA8x5x_REG_OUT_X_MSB, value);
}

int init_nvs(void) {
    int ret;
    struct flash_sector sector;

    nvs.flash_device = DEVICE_DT_GET(DT_CHOSEN(zephyr_flash_controller));
    if (!device_is_ready(nvs.flash_device)) {
        LOG_ERR("Flash device not ready");
        return -ENODEV;
    }

    nvs.offset = FIXED_PARTITION_OFFSET(DT_NODELABEL(storage));
    ret = flash_get_sector_info_by_id(nvs.flash_device, NVS_PARTITION_ID, &sector);
    if (ret) {
        LOG_ERR("Unable to get sector info (%d)", ret);
        return ret;
    }

    nvs.sector_size = sector.fs_size;
    nvs.sector_count = 1;

    ret = nvs_init(&nvs, NULL);
    if (ret) {
        LOG_ERR("NVS init failed (%d)", ret);
        return ret;
    }

    return 0;
}

int store_value_in_nvs(uint8_t value) {
    return nvs_write(&nvs, 0, &value, sizeof(value));
}

int main(void) {
    uint8_t mma8x5x_value;
    int ret;

    i2c_dev = DEVICE_DT_GET(DT_NODELABEL(i2c0));
    if (!device_is_ready(i2c_dev)) {
        LOG_ERR("I2C device not ready");
        return -ENODEV;
    }

    ret = init_nvs();
    if (ret) {
        LOG_ERR("NVS initialization failed");
        return -EIO;
    }

    while (1) {
        ret = read_mma8x5x(&mma8x5x_value);
        if (ret) {
            LOG_ERR("Failed to read MMA8x5x (%d)", ret);
        } else {
            LOG_INF("MMA8x5x value: %d", mma8x5x_value);
            ret = store_value_in_nvs(mma8x5x_value);
            if (ret) {
                LOG_ERR("Failed to store value in NVS (%d)", ret);
            }
        }

        k_sleep(K_SECONDS(5));
    }

    return 0;
}
