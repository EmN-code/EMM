
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
#define NVS_PARTITION_ID FIXED_PARTITION_ID(storage_partition)

LOG_MODULE_REGISTER(main);

const struct device *i2c_dev;
struct nvs_fs fs;

int read_mma8x5x(uint8_t *value) {
    return i2c_reg_read_byte(i2c_dev, MMA8x5x_I2C_ADDRESS, MMA8x5x_REG_OUT_X_MSB, value);
}

int init_nvs(void) {
    struct flash_sector *sectors;
    int count;

    if (flash_get_sectors(NVS_PARTITION_ID, &sectors, &count) != 0) {
        LOG_ERR("Failed to get flash sectors");
        return -1;
    }

    fs.flash_device = DEVICE_DT_GET(DT_CHOSEN(zephyr_flash_controller));
    if (!device_is_ready(fs.flash_device)) {
        LOG_ERR("Flash device not ready");
        return -1;
    }

    fs.sector_size = sectors[0].fs_size;
    fs.sector_count = count;

    if (nvs_init(&fs, NULL) != 0) {
        LOG_ERR("Failed to initialize NVS");
        return -1;
    }

    return 0;
}

int main(void) {
    uint8_t value;
    int ret;

    i2c_dev = DEVICE_DT_GET(DT_NODELABEL(i2c0));
    if (!device_is_ready(i2c_dev)) {
        LOG_ERR("I2C device not ready");
        return -1;
    }

    if (init_nvs() != 0) {
        LOG_ERR("NVS initialization failed");
        return -1;
    }

    while (1) {
        ret = read_mma8x5x(&value);
        if (ret != 0) {
            LOG_ERR("Failed to read MMA8x5x");
        } else {
            LOG_INF("MMA8x5x value: %d", value);
            ret = nvs_write(&fs, 0, &value, sizeof(value));
            if (ret < 0) {
                LOG_ERR("Failed to write to NVS");
            }
        }

        k_sleep(K_SECONDS(5));
    }

    return 0;
}
