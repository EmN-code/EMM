
#include <zephyr.h>
#include <device.h>
#include <drivers/i2c.h>
#include <storage/flash_map.h>
#include <fs/nvs.h>
#include <logging/log.h>

#define MMA8x5x_I2C_ADDRESS 0x1D
#define MMA8x5x_REG_OUT_X_MSB 0x01

#define NVS_PARTITION_ID 1
#define NVS_SECTOR_COUNT 4
#define NVS_SECTOR_SIZE 4096

LOG_MODULE_REGISTER(main);

static struct nvs_fs fs = {
    .sector_size = NVS_SECTOR_SIZE,
    .sector_count = NVS_SECTOR_COUNT,
    .offset = FLASH_AREA_OFFSET(storage),
};

void read_mma8x5x(const struct device *i2c_dev, uint8_t *data)
{
    i2c_reg_read_byte(i2c_dev, MMA8x5x_I2C_ADDRESS, MMA8x5x_REG_OUT_X_MSB, data);
}

void store_value_in_nvs(uint8_t value)
{
    int ret;
    ret = nvs_write(&fs, 0, &value, sizeof(value));
    if (ret < 0) {
        LOG_ERR("Failed to write to NVS: %d", ret);
    } else {
        LOG_INF("Value stored in NVS: %d", value);
    }
}

void main(void)
{
    const struct device *i2c_dev;
    uint8_t data;

    i2c_dev = device_get_binding("I2C_0");
    if (!i2c_dev) {
        LOG_ERR("I2C device not found");
        return;
    }

    int ret = nvs_mount(&fs);
    if (ret) {
        LOG_ERR("NVS mount failed: %d", ret);
        return;
    }

    while (1) {
        read_mma8x5x(i2c_dev, &data);
        store_value_in_nvs(data);
        k_sleep(K_SECONDS(5));
    }
}
