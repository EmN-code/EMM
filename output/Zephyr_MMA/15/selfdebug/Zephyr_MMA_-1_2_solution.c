
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

LOG_MODULE_REGISTER(main);

void read_mma8x5x(const struct device *i2c_dev, uint8_t *value)
{
    i2c_reg_read_byte(i2c_dev, MMA8x5x_I2C_ADDRESS, MMA8x5x_REG_OUT_X_MSB, value);
}

void store_value_in_nvs(struct nvs_fs *fs, uint8_t value)
{
    int err;
    err = nvs_write(fs, 0, &value, sizeof(value));
    if (err < 0) {
        LOG_ERR("Failed to write to NVS (err %d)", err);
    } else {
        LOG_INF("Value stored in NVS: %d", value);
    }
}

int main(void)
{
    const struct device *i2c_dev;
    struct nvs_fs fs;
    uint8_t mma8x5x_value;

    i2c_dev = device_get_binding("I2C_1");
    if (!i2c_dev) {
        LOG_ERR("I2C device not found");
        return -1;
    }

    fs.flash_device = FLASH_AREA_ID(storage_partition);
    fs.offset = 0;
    fs.sector_size = NVS_SECTOR_SIZE;
    fs.sector_count = 1;

    int err = nvs_init(&fs, fs.flash_device);
    if (err) {
        LOG_ERR("NVS init failed (err %d)", err);
        return -1;
    }

    while (1) {
        read_mma8x5x(i2c_dev, &mma8x5x_value);
        store_value_in_nvs(&fs, mma8x5x_value);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
