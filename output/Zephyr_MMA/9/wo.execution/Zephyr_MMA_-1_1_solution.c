
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>

#define MMA8x5x_I2C_ADDRESS 0x1D
#define MMA8x5x_REG_OUT_X_MSB 0x01
#define NVS_PARTITION FLASH_AREA_ID(storage)

#define SLEEP_TIME_MS 5000

static struct nvs_fs fs;

void read_mma8x5x(const struct device *i2c_dev, int16_t *x, int16_t *y, int16_t *z) {
    uint8_t data[6];

    i2c_burst_read(i2c_dev, MMA8x5x_I2C_ADDRESS, MMA8x5x_REG_OUT_X_MSB, data, 6);

    *x = (data[0] << 8) | data[1];
    *y = (data[2] << 8) | data[3];
    *z = (data[4] << 8) | data[5];
}

void store_value_in_nvs(int16_t x, int16_t y, int16_t z) {
    struct nvs_fs *fs_ptr = &fs;
    int rc;

    rc = nvs_write(fs_ptr, 0, &x, sizeof(x));
    if (rc < 0) {
        printk("Failed to write x value to NVS: %d\n", rc);
        return;
    }

    rc = nvs_write(fs_ptr, 1, &y, sizeof(y));
    if (rc < 0) {
        printk("Failed to write y value to NVS: %d\n", rc);
        return;
    }

    rc = nvs_write(fs_ptr, 2, &z, sizeof(z));
    if (rc < 0) {
        printk("Failed to write z value to NVS: %d\n", rc);
        return;
    }
}

int main(void) {
    const struct device *i2c_dev = device_get_binding("I2C_1");
    if (!i2c_dev) {
        printk("I2C device not found\n");
        return -1;
    }

    int16_t x, y, z;

    // Initialize NVS
    fs.sector_size = 4096;
    fs.sector_count = 2;
    fs.offset = FLASH_AREA_OFFSET(storage);

    int rc = nvs_mount(&fs);
    if (rc) {
        printk("NVS mount failed: %d\n", rc);
        return -1;
    }

    while (1) {
        read_mma8x5x(i2c_dev, &x, &y, &z);
        printk("MMA8x5x: x=%d, y=%d, z=%d\n", x, y, z);

        store_value_in_nvs(x, y, z);

        k_sleep(K_MSEC(SLEEP_TIME_MS));
    }

    return 0;
}
