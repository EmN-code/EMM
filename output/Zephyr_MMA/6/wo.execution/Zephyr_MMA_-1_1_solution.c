
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

static struct nvs_fs fs;

void read_mma8x5x(const struct device *i2c_dev, int16_t *x, int16_t *y, int16_t *z) {
    uint8_t data[6];
    i2c_burst_read(i2c_dev, MMA8x5x_I2C_ADDRESS, MMA8x5x_REG_OUT_X_MSB, data, 6);

    *x = (data[0] << 8) | data[1];
    *y = (data[2] << 8) | data[3];
    *z = (data[4] << 8) | data[5];
}

void store_to_nvs(int16_t x, int16_t y, int16_t z) {
    struct flash_sector sector;
    int ret;

    ret = flash_get_sector_info_by_id(NVS_PARTITION_ID, 0, &sector);
    if (ret) {
        LOG_ERR("Failed to get flash sector info (%d)", ret);
        return;
    }

    fs.flash_device = (struct device *)device_get_binding("FLASH_CTRL");
    if (!fs.flash_device) {
        LOG_ERR("Flash device not found");
        return;
    }

    fs.offset = sector.start;
    fs.sector_size = sector.size;
    fs.sector_count = 1;

    ret = nvs_init(&fs, "storage");
    if (ret) {
        LOG_ERR("Failed to init NVS (%d)", ret);
        return;
    }

    ret = nvs_write(&fs, 1, &x, sizeof(x));
    if (ret < 0) {
        LOG_ERR("Failed to write X to NVS (%d)", ret);
    }

    ret = nvs_write(&fs, 2, &y, sizeof(y));
    if (ret < 0) {
        LOG_ERR("Failed to write Y to NVS (%d)", ret);
    }

    ret = nvs_write(&fs, 3, &z, sizeof(z));
    if (ret < 0) {
        LOG_ERR("Failed to write Z to NVS (%d)", ret);
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

        store_to_nvs(x, y, z);

        k_sleep(K_SECONDS(5));
    }

    return 0;
}
