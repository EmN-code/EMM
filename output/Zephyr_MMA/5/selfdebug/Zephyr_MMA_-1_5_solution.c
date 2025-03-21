
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
    uint8_t addr = MMA8x5x_REG_OUT_X_MSB;
    i2c_write_read(i2c_dev, MMA8x5x_I2C_ADDRESS, &addr, 1, data, 6);
}

void store_value_in_nvs(struct nvs_fs *fs, uint16_t id, uint8_t *data, size_t len) {
    int ret = nvs_write(fs, id, data, len);
    if (ret < 0) {
        LOG_ERR("Failed to write to NVS (err %d)", ret);
    } else {
        LOG_INF("Value stored in NVS");
    }
}

int main(void) {
    const struct device *i2c_dev = device_get_binding("I2C_0");
    if (!i2c_dev) {
        LOG_ERR("I2C device not found");
        return -1;
    }

    struct nvs_fs fs;
    fs.sector_size = 4096;
    fs.sector_count = 2;
    fs.offset = FLASH_AREA_OFFSET(storage);

    int ret = nvs_init(&fs, "storage");
    if (ret) {
        LOG_ERR("NVS init failed (err %d)", ret);
        return -1;
    }

    uint8_t data[6];
    while (1) {
        read_mma8x5x(i2c_dev, data);
        store_value_in_nvs(&fs, 1, data, sizeof(data));
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
