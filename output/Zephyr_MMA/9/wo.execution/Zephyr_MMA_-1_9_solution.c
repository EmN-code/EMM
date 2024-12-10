
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/sys/printk.h>

#define MMA8x5x_I2C_ADDRESS 0x1D
#define MMA8x5x_REG_OUT_X_MSB 0x01
#define NVS_PARTITION_ID FIXED_PARTITION_ID(storage)

const struct device *i2c_dev;
struct nvs_fs fs;

void read_mma8x5x(uint8_t *buf) {
    i2c_burst_read(i2c_dev, MMA8x5x_I2C_ADDRESS, MMA8x5x_REG_OUT_X_MSB, buf, 6);
}

void store_value_in_nvs(uint8_t *buf) {
    nvs_write(&fs, 0, buf, 6);
}

int main(void) {
    uint8_t accel_data[6];

    i2c_dev = device_get_binding("I2C_0");
    if (!i2c_dev) {
        printk("I2C device not found\n");
        return -1;
    }

    fs.flash_device = FLASH_AREA_DEVICE(NVS_PARTITION_ID);
    fs.offset = FIXED_PARTITION_OFFSET(NVS_PARTITION_ID);
    fs.sector_size = 4096;
    fs.sector_count = 2;

    if (nvs_mount(&fs) != 0) {
        printk("NVS mount failed\n");
        return -1;
    }

    while (1) {
        read_mma8x5x(accel_data);
        store_value_in_nvs(accel_data);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
