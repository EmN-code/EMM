
#include <zephyr.h>
#include <device.h>
#include <drivers/i2c.h>
#include <storage/flash_map.h>
#include <fs/nvs.h>
#include <logging/log.h>

#define MMA8x5x_I2C_ADDRESS 0x1D
#define MMA8x5x_REG_OUT_X_MSB 0x01
#define NVS_PARTITION_ID FIXED_PARTITION_ID(storage_partition)
#define NVS_SECTOR_SIZE 4096
#define NVS_SECTOR_COUNT 4

LOG_MODULE_REGISTER(main);

struct nvs_fs fs = {
    .sector_size = NVS_SECTOR_SIZE,
    .sector_count = NVS_SECTOR_COUNT,
    .offset = FIXED_PARTITION_DEVICE_GET(NVS_PARTITION_ID),
};

void read_mma8x5x(const struct device *i2c_dev, uint8_t *data) {
    uint8_t reg_addr = MMA8x5x_REG_OUT_X_MSB;
    i2c_write_read(i2c_dev, MMA8x5x_I2C_ADDRESS, &reg_addr, sizeof(reg_addr), data, 6);
}

void store_value_in_nvs(uint8_t *data) {
    int ret;
    ret = nvs_init(&fs, fs.offset);
    if (ret) {
        LOG_ERR("NVS init failed: %d", ret);
        return;
    }

    ret = nvs_write(&fs, 1, data, 6);
    if (ret < 0) {
        LOG_ERR("NVS write failed: %d", ret);
    } else {
        LOG_INF("NVS write success: %d bytes", ret);
    }
}

void main(void) {
    const struct device *i2c_dev = device_get_binding("I2C_1");
    if (!i2c_dev) {
        LOG_ERR("I2C device not found");
        return;
    }

    uint8_t data[6];

    while (1) {
        read_mma8x5x(i2c_dev, data);
        store_value_in_nvs(data);
        k_sleep(K_SECONDS(5));
    }
}
