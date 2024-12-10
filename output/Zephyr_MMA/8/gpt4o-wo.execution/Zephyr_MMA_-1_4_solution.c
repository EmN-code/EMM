
#include <zephyr/kernel.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>

#define MMA8x5x_I2C_ADDRESS 0x1D
#define MMA8x5x_REG_OUT_X_MSB 0x01

#define NVS_PARTITION_ID FIXED_PARTITION_ID(storage_partition)
#define NVS_PARTITION FIXED_PARTITION_DEVICE(storage_partition)

LOG_MODULE_REGISTER(main);

static struct nvs_fs fs;

int read_mma8x5x(const struct device *i2c_dev, uint8_t *data)
{
    return i2c_reg_read_byte(i2c_dev, MMA8x5x_I2C_ADDRESS, MMA8x5x_REG_OUT_X_MSB, data);
}

int init_nvs(void)
{
    fs.flash_device = NVS_PARTITION;
    fs.offset = FIXED_PARTITION_OFFSET(storage_partition);
    fs.sector_size = 4096;
    fs.sector_count = 2;

    return nvs_init(&fs, "storage");
}

int store_reading(uint8_t value)
{
    return nvs_write(&fs, 0, &value, sizeof(value));
}

int main(void)
{
    const struct device *i2c_dev = device_get_binding("I2C_1");
    if (!i2c_dev) {
        LOG_ERR("Could not get I2C device");
        return -1;
    }

    if (init_nvs() != 0) {
        LOG_ERR("NVS init failed");
        return -1;
    }

    while (1) {
        uint8_t reading;
        if (read_mma8x5x(i2c_dev, &reading) == 0) {
            LOG_INF("MMA8x5x reading: %d", reading);
            if (store_reading(reading) != sizeof(reading)) {
                LOG_ERR("Failed to store reading");
            }
        } else {
            LOG_ERR("Failed to read MMA8x5x");
        }

        k_sleep(K_SECONDS(5));
    }

    return 0;
}
