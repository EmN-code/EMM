
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main);

#define MMA8x5x_I2C_ADDRESS 0x1D
#define MMA8x5x_REG_OUT_X_MSB 0x01
#define NVS_PARTITION_ID 1
#define NVS_STORAGE_ID 1

const struct device *i2c_dev;
struct nvs_fs nvs;

int read_mma8x5x(uint8_t *data) {
    return i2c_reg_read_byte(i2c_dev, MMA8x5x_I2C_ADDRESS, MMA8x5x_REG_OUT_X_MSB, data);
}

int init_nvs(void) {
    struct flash_sector_param sectors[1];
    const struct flash_area *fa;

    if (flash_area_open(NVS_PARTITION_ID, &fa) != 0) {
        LOG_ERR("Failed to open flash area");
        return -1;
    }

    sectors[0].fs_size = fa->fa_size;
    sectors[0].fs_addr = fa->fa_off;

    nvs.sector_size = sectors[0].fs_size;
    nvs.sector_count = 1;
    nvs.offset = 0;

    if (nvs_init(&nvs, fa->fa_dev) != 0) {
        LOG_ERR("Failed to initialize NVS");
        return -1;
    }

    return 0;
}

int store_value_in_nvs(uint8_t value) {
    return nvs_write(&nvs, NVS_STORAGE_ID, &value, sizeof(value));
}

int main(void) {
    uint8_t mma8x5x_data;

    i2c_dev = device_get_binding("I2C_0");
    if (!i2c_dev) {
        LOG_ERR("I2C device not found");
        return -1;
    }

    if (init_nvs() != 0) {
        LOG_ERR("NVS initialization failed");
        return -1;
    }

    while (1) {
        if (read_mma8x5x(&mma8x5x_data) == 0) {
            LOG_INF("MMA8x5x data: %d", mma8x5x_data);
            if (store_value_in_nvs(mma8x5x_data) != 0) {
                LOG_ERR("Failed to store value in NVS");
            }
        } else {
            LOG_ERR("Failed to read MMA8x5x");
        }

        k_sleep(K_SECONDS(5));
    }

    return 0;
}
