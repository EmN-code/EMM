
#include <zephyr.h>
#include <device.h>
#include <drivers/i2c.h>
#include <storage/flash_map.h>
#include <fs/nvs.h>
#include <logging/log.h>

#define MMA8x5x_I2C_ADDRESS 0x1D
#define MMA8x5x_REG_OUT_X_MSB 0x01

#define NVS_PARTITION FLASH_AREA_ID(storage)
#define NVS_SECTOR_SIZE 4096

LOG_MODULE_REGISTER(main);

static struct nvs_fs fs;

void read_mma8x5x(const struct device *i2c_dev, int16_t *x_value)
{
    uint8_t buffer[2];

    if (i2c_burst_read(i2c_dev, MMA8x5x_I2C_ADDRESS, MMA8x5x_REG_OUT_X_MSB, buffer, 2) != 0) {
        LOG_ERR("Failed to read from MMA8x5x sensor");
        return;
    }

    *x_value = (buffer[0] << 8) | buffer[1];
}

void store_value_in_nvs(int16_t value)
{
    struct nvs_fs *fs_ptr = &fs;
    int rc;

    rc = nvs_write(fs_ptr, 0, &value, sizeof(value));
    if (rc < 0) {
        LOG_ERR("Failed to write to NVS: %d", rc);
    } else {
        LOG_INF("Value stored in NVS: %d", value);
    }
}

int main(void)
{
    const struct device *i2c_dev;
    int16_t x_value;

    i2c_dev = device_get_binding("I2C_1");
    if (!i2c_dev) {
        LOG_ERR("I2C device not found");
        return -1;
    }

    fs.flash_device = NULL;
    fs.offset = NVS_PARTITION;
    fs.sector_size = NVS_SECTOR_SIZE;
    fs.sector_count = 1;

    if (nvs_init(&fs, "storage") != 0) {
        LOG_ERR("Failed to initialize NVS");
        return -1;
    }

    while (1) {
        read_mma8x5x(i2c_dev, &x_value);
        LOG_INF("MMA8x5x X value: %d", x_value);

        store_value_in_nvs(x_value);

        k_sleep(K_SECONDS(5));
    }

    return 0;
}
