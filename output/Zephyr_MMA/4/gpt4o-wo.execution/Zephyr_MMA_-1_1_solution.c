#include <zephyr/kernel.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/device.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/storage/flash.h>

#define I2C_DEV DT_LABEL(DT_NODELABEL(i2c0))
#define MMA8x5x_ADDR 0x1D
#define MMA8x5x_REG 0x00
#define STORAGE_PARTITION_ID STORAGE_PARTITION_ID_APP
#define SLEEP_TIME K_SECONDS(5)

void main(void) {
    const struct device *i2c_dev;
    uint8_t buffer;
    struct flash_area *storage_handle;
    int ret;

    // Initialize I2C device
    i2c_dev = device_get_binding(I2C_DEV);
    if (!i2c_dev) {
        printk("I2C device not found\n");
        return;
    }

    // Open the NVS storage area
    ret = flash_area_open(STORAGE_PARTITION_ID, &storage_handle);
    if (ret) {
        printk("Failed to open NVS storage\n");
        return;
    }

    while (1) {
        // Read data from MMA8x5x sensor
        ret = i2c_reg_read_byte(i2c_dev, MMA8x5x_ADDR, MMA8x5x_REG, &buffer);
        if (ret) {
            printk("Failed to read from sensor\n");
        } else {
            printk("Read value: %d\n", buffer);

            // Write the value to NVS
            ret = flash_area_write(storage_handle, 0, &buffer, sizeof(buffer));
            if (ret) {
                printk("Failed to write to NVS\n");
            }
        }

        // Sleep for 5 seconds
        k_sleep(SLEEP_TIME);
    }
}
flash_area_close(storage_handle);