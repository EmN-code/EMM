
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main);

#define MMA8x5x_I2C_ADDRESS 0x1D
#define MMA8x5x_REG_OUT_X_MSB 0x01

#define STORAGE_PARTITION DT_LABEL(DT_NODELABEL(storage_partition))
#define I2C_DEV DT_LABEL(DT_NODELABEL(i2c0))

static struct nvs_fs fs;

void read_mma8x5x(const struct device *i2c_dev) {
    uint8_t data[6];
    int ret;

    ret = i2c_burst_read(i2c_dev, MMA8x5x_I2C_ADDRESS, MMA8x5x_REG_OUT_X_MSB, data, sizeof(data));
    if (ret < 0) {
        LOG_ERR("Failed to read from MMA8x5x sensor: %d", ret);
        return;
    }

    int16_t x = (data[0] << 8) | data[1];
    int16_t y = (data[2] << 8) | data[3];
    int16_t z = (data[4] << 8) | data[5];

    LOG_INF("MMA8x5x: X=%d, Y=%d, Z=%d", x, y, z);

    // Store the read value persistently using NVS
    struct nvs_fs *fs_ptr = &fs;
    ret = nvs_write(fs_ptr, 0, &x, sizeof(x));
    if (ret < 0) {
        LOG_ERR("Failed to write to NVS: %d", ret);
    }
}

int main(void) {
    const struct device *i2c_dev = device_get_binding(I2C_DEV);
    if (!i2c_dev) {
        LOG_ERR("I2C device not found");
        return -1;
    }

    // Initialize NVS
    fs.flash_device = STORAGE_PARTITION;
    fs.offset = 0;
    fs.sector_size = 4096;
    fs.sector_count = 2;

    int ret = nvs_init(&fs, fs.flash_device);
    if (ret) {
        LOG_ERR("NVS init failed: %d", ret);
        return -1;
    }

    while (1) {
        read_mma8x5x(i2c_dev);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
