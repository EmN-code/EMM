
#include <zephyr/fs/nvs.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/mqtt.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/net/coap.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/sys/printk.h>
#include <zephyr/net/mqtt_sn.h>
#include <zephyr/drivers/flash.h>

#define MMA8x5x_I2C_ADDRESS 0x1D
#define MMA8x5x_REG_OUT_X_MSB 0x01

#define NVS_PARTITION_ID FIXED_PARTITION_ID(storage)
#define NVS_PARTITION FIXED_PARTITION_DEVICE(storage)

struct nvs_fs fs;

void read_mma8x5x(uint8_t *value) {
    const struct device *i2c_dev = device_get_binding("I2C_1");
    if (!i2c_dev) {
        printk("I2C device not found\n");
        return;
    }

    if (i2c_reg_read_byte(i2c_dev, MMA8x5x_I2C_ADDRESS, MMA8x5x_REG_OUT_X_MSB, value) != 0) {
        printk("Failed to read from MMA8x5x\n");
    }
}

void store_value_in_nvs(uint8_t value) {
    if (nvs_write(&fs, 1, &value, sizeof(value)) < 0) {
        printk("Failed to write to NVS\n");
    }
}

int main(void) {
    uint8_t mma8x5x_value;

    // Initialize NVS
    fs.flash_device = NVS_PARTITION;
    fs.offset = FIXED_PARTITION_OFFSET(storage);
    fs.sector_size = 4096;
    fs.sector_count = 2;

    if (nvs_init(&fs, fs.flash_device) != 0) {
        printk("NVS init failed\n");
        return -1;
    }

    while (1) {
        read_mma8x5x(&mma8x5x_value);
        printk("MMA8x5x value: %d\n", mma8x5x_value);
        store_value_in_nvs(mma8x5x_value);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
