
#include <zephyr.h>
#include <device.h>
#include <drivers/i2c.h>
#include <storage/flash_map.h>
#include <fs/nvs.h>
#include <logging/log.h>

#define MMA8x5x_I2C_ADDRESS 0x1D
#define MMA8x5x_REG_OUT_X_MSB 0x01
#define NVS_PARTITION FLASH_AREA_ID(storage)

LOG_MODULE_REGISTER(main);

void read_mma8x5x(const struct device *i2c_dev, uint8_t *data) {
    uint8_t reg_addr = MMA8x5x_REG_OUT_X_MSB;
    i2c_burst_read(i2c_dev, MMA8x5x_I2C_ADDRESS, reg_addr, data, 6);
}

void store_value_in_nvs(struct nvs_fs *fs, uint8_t *data) {
    int ret;
    ret = nvs_write(fs, 0, data, 6);
    if (ret < 0) {
        LOG_ERR("NVS write failed: %d", ret);
    } else {
        LOG_INF("NVS write successful");
    }
}

int main(void) {
    const struct device *i2c_dev;
    struct nvs_fs fs;
    uint8_t data[6];

    i2c_dev = device_get_binding("I2C_1");
    if (!i2c_dev) {
        LOG_ERR("Could not get I2C device");
        return -1;
    }

    fs.flash_device = FLASH_AREA_DEVICE(NVS_PARTITION);
    fs.offset = FIXED_PARTITION_OFFSET(storage);
    fs.sector_size = 4096;
    fs.id_size = 2;

    int ret = nvs_mount(&fs);
    if (ret) {
        LOG_ERR("NVS mount failed: %d", ret);
        return -1;
    }

    while (1) {
        read_mma8x5x(i2c_dev, data);
        store_value_in_nvs(&fs, data);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
