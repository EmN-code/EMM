
#include <zephyr.h>
#include <device.h>
#include <drivers/i2c.h>
#include <storage/flash_map.h>
#include <fs/nvs.h>
#include <logging/log.h>

#define MMA8x5x_I2C_ADDRESS 0x1D
#define MMA8x5x_REG_OUT_X_MSB 0x01
#define NVS_PARTITION FLASH_AREA_ID(storage)
#define NVS_NAMESPACE "mma8x5x"
#define NVS_KEY "accel_data"

LOG_MODULE_REGISTER(main);

static struct nvs_fs fs = {
    .sector_size = DT_FLASH_ERASE_BLOCK_SIZE,
    .sector_count = DT_FLASH_AREA_STORAGE_SIZE / DT_FLASH_ERASE_BLOCK_SIZE,
    .offset = DT_FLASH_AREA_STORAGE_OFFSET,
};

static int read_mma8x5x(const struct device *i2c_dev, uint8_t *data)
{
    return i2c_reg_read_byte(i2c_dev, MMA8x5x_I2C_ADDRESS, MMA8x5x_REG_OUT_X_MSB, data);
}

static int store_data_to_nvs(uint8_t data)
{
    int ret;
    ret = nvs_init(&fs, NVS_PARTITION);
    if (ret) {
        LOG_ERR("NVS init failed: %d", ret);
        return ret;
    }

    ret = nvs_write(&fs, NVS_KEY, &data, sizeof(data));
    if (ret < 0) {
        LOG_ERR("NVS write failed: %d", ret);
        return ret;
    }

    LOG_INF("Data stored in NVS: %d", data);
    return 0;
}

int main(void)
{
    const struct device *i2c_dev = device_get_binding("I2C_0");
    if (!i2c_dev) {
        LOG_ERR("I2C device not found");
        return -1;
    }

    uint8_t accel_data;
    while (1) {
        if (read_mma8x5x(i2c_dev, &accel_data) == 0) {
            LOG_INF("MMA8x5x read: %d", accel_data);
            store_data_to_nvs(accel_data);
        } else {
            LOG_ERR("Failed to read MMA8x5x");
        }

        k_sleep(K_SECONDS(5));
    }

    return 0;
}
