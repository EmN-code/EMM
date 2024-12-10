#include <zephyr/sys/kernel.h>
#include <zephyr/sys/sys_io.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/devicetree.h>
#include <zephyr/device.h>

#define MMA8X5X_I2C_NODE DT_NODELABEL(i2c1) // Change this to match your i2c node
#define MMA8X5X_I2C_ADDR 0x1C              // Ensure this matches your sensor's address

const struct device *i2c_dev;

static uint8_t sensor_reading;

void read_mma8x5x_sensor(void) {
    if (i2c_dev == NULL) {
        printk("I2C device not found\n");
        return;
    }

    int ret = i2c_reg_read_byte(i2c_dev, MMA8X5X_I2C_ADDR, 0x00, &sensor_reading);
    if (ret) {
        printk("Failed to read sensor: %d\n", ret);
    }
}

void store_reading_nvs(void) {
    // Placeholder for NVS storage implementation
    // You would typically store sensor_reading to a specific partition
    printk("Storing sensor reading: %d\n", sensor_reading);
}

void main(void) {
    i2c_dev = DEVICE_DT_GET(MMA8X5X_I2C_NODE);
    if (!device_is_ready(i2c_dev)) {
        printk("I2C controller not found\n");
        return;
    }

    while (1) {
        read_mma8x5x_sensor();
        store_reading_nvs();
        k_sleep(K_SECONDS(5));
    }
}