
#include <zephyr/kernel.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/printk.h>

#define MMA8x5x_I2C_ADDR 0x1D
#define MMA8x5x_REG_OUT_X_MSB 0x01
#define NVS_PARTITION_ID FIXED_PARTITION_ID(storage_partition)

LOG_MODULE_REGISTER(main);

void read_mma8x5x(const struct device *i2c_dev, int16_t *x, int16_t *y, int16_t *z) {
    uint8_t buf[6];
    int ret;

    ret = i2c_burst_read(i2c_dev, MMA8x5x_I2C_ADDR, MMA8x5x_REG_OUT_X_MSB, buf, sizeof(buf));
    if (ret < 0) {
        LOG_ERR("Failed to read from MMA8x5x: %d", ret);
        return;
    }

    *x = (buf[0] << 8) | buf[1];
    *y = (buf[2] << 8) | buf[3];
    *z = (buf[4] << 8) | buf[5];
}

void store_to_nvs(struct nvs_fs *fs, uint16_t id, const void *data, size_t len) {
    int ret;

    ret = nvs_write(fs, id, data, len);
    if (ret < 0) {
        LOG_ERR("Failed to write to NVS: %d", ret);
    }
}

int main(void) {
    const struct device *i2c_dev;
    struct nvs_fs fs;
    int16_t x, y, z;

    i2c_dev = device_get_binding("I2C_1");
    if (!i2c_dev) {
        LOG_ERR("I2C device not found");
        return -1;
    }

    fs.flash_device = FLASH_AREA_DEVICE(NVS_PARTITION_ID);
    fs.offset = FIXED_PARTITION_OFFSET(NVS_PARTITION_ID);
    fs.sector_size = FIXED_PARTITION_SECTOR_SIZE(NVS_PARTITION_ID);
    fs.sector_count = FIXED_PARTITION_SECTOR_COUNT(NVS_PARTITION_ID);

    int ret = nvs_mount(&fs);
    if (ret) {
        LOG_ERR("NVS mount failed: %d", ret);
        return -1;
    }

    while (1) {
        read_mma8x5x(i2c_dev, &x, &y, &z);
        LOG_INF("MMA8x5x: X=%d, Y=%d, Z=%d", x, y, z);

        store_to_nvs(&fs, 1, &x, sizeof(x));
        store_to_nvs(&fs, 2, &y, sizeof(y));
        store_to_nvs(&fs, 3, &z, sizeof(z));

        k_sleep(K_SECONDS(5));
    }

    return 0;
}
