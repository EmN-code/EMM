
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

#define NVS_PARTITION_ID 1
#define NVS_SECTOR_COUNT 4
#define NVS_SECTOR_SIZE 4096

struct nvs_fs nvs = {
    .sector_size = NVS_SECTOR_SIZE,
    .sector_count = NVS_SECTOR_COUNT,
};

int read_mma8x5x(uint8_t *value) {
    const struct device *i2c_dev = device_get_binding("I2C_1");
    if (!i2c_dev) {
        printk("I2C device not found\n");
        return -1;
    }

    uint8_t reg_addr = MMA8x5x_REG_OUT_X_MSB;
    if (i2c_reg_read_byte(i2c_dev, MMA8x5x_I2C_ADDRESS, reg_addr, value) != 0) {
        printk("Failed to read from MMA8x5x\n");
        return -1;
    }

    return 0;
}

int store_value_in_nvs(uint8_t value) {
    if (nvs_write(&nvs, 0, &value, sizeof(value)) != 0) {
        printk("Failed to write to NVS\n");
        return -1;
    }

    return 0;
}

int main(void) {
    uint8_t mma8x5x_value;

    if (flash_area_open(NVS_PARTITION_ID, (const struct flash_area **)&nvs.flash_device) != 0) {
        printk("Failed to open flash area\n");
        return -1;
    }

    if (nvs_init(&nvs, nvs.flash_device) != 0) {
        printk("Failed to initialize NVS\n");
        return -1;
    }

    while (1) {
        if (read_mma8x5x(&mma8x5x_value) == 0) {
            printk("MMA8x5x value: %d\n", mma8x5x_value);
            store_value_in_nvs(mma8x5x_value);
        }

        k_sleep(K_SECONDS(5));
    }

    return 0;
}
