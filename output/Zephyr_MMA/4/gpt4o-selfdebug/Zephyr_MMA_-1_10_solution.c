#include <zephyr/syscalls/i2c.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/settings/settings.h>
#include <zephyr/device.h>
#include <zephyr.h>

#define MMA8X5X_ADDR 0x1C // Replace with actual I2C address for the MMA8x5x sensor
#define STORAGE_KEY "sensor/value"

void mma8x5x_read(const struct device *i2c_dev, int *value) {
    // Buffer to read the data
    uint8_t buf[2];
    struct i2c_msg msgs[2];
    msgs[0].flags = I2C_MSG_WRITE;
    msgs[0].len = 1;
    msgs[0].buf = buf;

    msgs[1].flags = I2C_MSG_READ | I2C_MSG_STOP;
    msgs[1].len = 1;
    msgs[1].buf = buf;

    buf[0] = 0x01; // point to a data register on MMA8x5x
    if (i2c_transfer(i2c_dev, msgs, 2, MMA8X5X_ADDR) == 0) {
        *value = (int)buf[0]; // Assign read value
    }
}

void store_value_nvs(int value) {
    char value_str[5];
    snprintf(value_str, sizeof(value_str), "%d", value);
    settings_save_one(STORAGE_KEY, value_str, strlen(value_str));
}

void main(void) {
    const struct device *i2c_dev = device_get_binding(DT_LABEL(DT_ALIAS(i2c_0)));
    if (i2c_dev == NULL) {
        printk("Failed to get I2C device\n");
        return;
    }

    int sensor_value;

    while (1) {
        mma8x5x_read(i2c_dev, &sensor_value);
        printk("Read value: %d\n", sensor_value);

        store_value_nvs(sensor_value);

        k_sleep(K_SECONDS(5));
    }
}
