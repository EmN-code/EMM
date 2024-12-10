
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main);

#define MMA8x5x_I2C_ADDRESS 0x1D
#define MMA8x5x_REG_OUT_X_MSB 0x01
#define NVS_PARTITION FLASH_AREA_ID(storage)

const struct device *i2c_dev;
struct nvs_fs nvs;

int read_mma8x5x(uint8_t *data) {
    return i2c_reg_read_byte(i2c_dev, MMA8x5x_I2C_ADDRESS, MMA8x5x_REG_OUT_X_MSB, data);
}

int init_nvs(void) {
    int ret;
    struct flash_pages_info info;

    nvs.flash_device = FLASH_AREA_DEVICE(NVS_PARTITION);
    if (!device_is_ready(nvs.flash_device)) {
        LOG_ERR("Flash device not ready");
        return -ENODEV;
    }

    nvs.offset = FLASH_AREA_OFFSET(NVS_PARTITION);
    ret = flash_get_page_info_by_offs(nvs.flash_device, nvs.offset, &info);
    if (ret) {
        LOG_ERR("Unable to get page info");
        return ret;
    }

    nvs.sector_size = info.size;
    nvs.sector_count = 3; // Adjust based on your needs

    ret = nvs_init(&nvs, NVS_PARTITION);
    if (ret) {
        LOG_ERR("NVS init failed");
        return ret;
    }

    return 0;
}

int store_value_in_nvs(uint8_t value) {
    int ret;
    ret = nvs_write(&nvs, 0, &value, sizeof(value));
    if (ret < 0) {
        LOG_ERR("NVS write failed");
        return ret;
    }
    return 0;
}

int main(void) {
    uint8_t mma8x5x_data;
    int ret;

    i2c_dev = device_get_binding("I2C_1");
    if (!i2c_dev) {
        LOG_ERR("I2C device not found");
        return -ENODEV;
    }

    ret = init_nvs();
    if (ret) {
        LOG_ERR("NVS initialization failed");
        return ret;
    }

    while (1) {
        ret = read_mma8x5x(&mma8x5x_data);
        if (ret) {
            LOG_ERR("Failed to read MMA8x5x");
        } else {
            LOG_INF("MMA8x5x data: %d", mma8x5x_data);
            ret = store_value_in_nvs(mma8x5x_data);
            if (ret) {
                LOG_ERR("Failed to store value in NVS");
            }
        }

        k_sleep(K_SECONDS(5));
    }

    return 0;
}
