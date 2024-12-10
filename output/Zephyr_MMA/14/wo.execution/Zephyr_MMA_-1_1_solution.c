
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
#define NVS_PARTITION FLASH_AREA_ID(storage)

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

void store_value_in_nvs(int16_t value) {
    struct nvs_fs *fs_ptr = &fs;
    int rc;

    rc = nvs_write(fs_ptr, 0, &value, sizeof(value));
    if (rc < 0) {
        LOG_ERR("Failed to write to NVS: %d", rc);
    } else {
        LOG_INF("Value stored in NVS: %d", value);
    }
}

int main(void) {
    const struct device *i2c_dev = DEVICE_DT_GET(DT_NODELABEL(i2c0));
    int16_t x, y, z;

    if (!device_is_ready(i2c_dev)) {
        LOG_ERR("I2C device not ready");
        return -1;
    }

    fs.sector_size = 4096;
    fs.sector_count = 2;
    fs.offset = FLASH_AREA_OFFSET(storage);

    int rc = nvs_mount(&fs);
    if (rc) {
        LOG_ERR("NVS mount failed: %d", rc);
        return -1;
    }

    while (1) {
        read_mma8x5x(i2c_dev, &x, &y, &z);
        LOG_INF("MMA8x5x: X=%d, Y=%d, Z=%d", x, y, z);

        store_value_in_nvs(x);

        k_sleep(K_SECONDS(5));
    }

    return 0;
}
