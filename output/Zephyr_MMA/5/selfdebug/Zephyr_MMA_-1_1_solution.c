
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main);

#define MMA8x5x_I2C_ADDRESS 0x1D
#define MMA8x5x_REG_OUT_X_MSB 0x01

#define NVS_PARTITION_ID 1
#define NVS_PARTITION_LABEL "storage"

static struct nvs_fs fs;

void read_mma8x5x(const struct device *i2c_dev, int16_t *x, int16_t *y, int16_t *z) {
    uint8_t data[6];

    if (i2c_burst_read(i2c_dev, MMA8x5x_I2C_ADDRESS, MMA8x5x_REG_OUT_X_MSB, data, 6) != 0) {
        LOG_ERR("Failed to read from MMA8x5x sensor");
        return;
    }

    *x = (data[0] << 8) | data[1];
    *y = (data[2] << 8) | data[3];
    *z = (data[4] << 8) | data[5];
}

void store_value_in_nvs(int16_t x, int16_t y, int16_t z) {
    struct flash_sector sector;
    int rc;

    rc = flash_get_sector_info_by_id(NVS_PARTITION_ID, 0, &sector);
    if (rc) {
        LOG_ERR("Failed to get flash sector info: %d", rc);
        return;
    }

    fs.flash_device = (struct device *)device_get_binding(NVS_PARTITION_LABEL);
    if (!fs.flash_device) {
        LOG_ERR("Failed to get flash device binding");
        return;
    }

    fs.offset = sector.fs_offset;
    fs.sector_size = sector.fs_size;
    fs.sector_count = 1;

    rc = nvs_init(&fs, NVS_PARTITION_LABEL);
    if (rc) {
        LOG_ERR("Failed to initialize NVS: %d", rc);
        return;
    }

    rc = nvs_write(&fs, 1, &x, sizeof(x));
    if (rc < 0) {
        LOG_ERR("Failed to write X value to NVS: %d", rc);
    }

    rc = nvs_write(&fs, 2, &y, sizeof(y));
    if (rc < 0) {
        LOG_ERR("Failed to write Y value to NVS: %d", rc);
    }

    rc = nvs_write(&fs, 3, &z, sizeof(z));
    if (rc < 0) {
        LOG_ERR("Failed to write Z value to NVS: %d", rc);
    }
}

int main(void) {
    const struct device *i2c_dev = device_get_binding("I2C_0");
    if (!i2c_dev) {
        LOG_ERR("I2C device not found");
        return -1;
    }

    int16_t x, y, z;

    while (1) {
        read_mma8x5x(i2c_dev, &x, &y, &z);
        LOG_INF("MMA8x5x: X=%d, Y=%d, Z=%d", x, y, z);

        store_value_in_nvs(x, y, z);

        k_sleep(K_SECONDS(5));
    }

    return 0;
}
