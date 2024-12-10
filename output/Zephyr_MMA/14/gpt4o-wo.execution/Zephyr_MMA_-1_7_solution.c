
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>

#define MMA8x5x_I2C_ADDRESS 0x1D
#define MMA8x5x_REG_OUT_X_MSB 0x01
#define NVS_PARTITION_ID FIXED_PARTITION_ID(storage_partition)

static struct nvs_fs fs;

void read_mma8x5x(uint8_t *buf) {
    const struct device *i2c_dev = device_get_binding("I2C_0");
    if (!i2c_dev) {
        printk("I2C device not found\n");
        return;
    }

    if (i2c_burst_read(i2c_dev, MMA8x5x_I2C_ADDRESS, MMA8x5x_REG_OUT_X_MSB, buf, 6) < 0) {
        printk("Failed to read from MMA8x5x\n");
    }
}

void store_reading(uint8_t *data, size_t len) {
    if (nvs_write(&fs, 1, data, len) < 0) {
        printk("Failed to write to NVS\n");
    }
}

int main(void) {
    uint8_t accel_data[6];

    // Initialize NVS
    fs.flash_device = FIXED_PARTITION_DEVICE(NVS_PARTITION_ID);
    fs.offset = FIXED_PARTITION_OFFSET(NVS_PARTITION_ID);
    fs.sector_size = 4096;
    fs.sector_count = 2;

    if (nvs_mount(&fs) < 0) {
        printk("Failed to mount NVS\n");
        return -1;
    }

    while (1) {
        read_mma8x5x(accel_data);
        store_reading(accel_data, sizeof(accel_data));
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
