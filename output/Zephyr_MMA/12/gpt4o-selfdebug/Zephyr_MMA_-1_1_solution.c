#include <zephyr/storage/flash_map.h>
#include <zephyr/storage/nvs.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/kernel.h>

/* Constants defining the I2C spec for MMA8x5x */
#define I2C_DEV DT_LABEL(DT_NODELABEL(i2c0))
#define MMA8X5X_ADDR 0x1D /* Default I2C address for an MMA8x5x sensor */

/* NVS related constants */
#define NVS_PART_LABEL storage
#define NVS_STORAGE_ID 1
#define STORAGE_KEY 1
#define STORAGE_SECTOR_SIZE 0x1000
#define STORAGE_SECTOR_COUNT 3

/* Define the sleep time between reads */
#define SLEEP_TIME_MS (5 * 1000)

void main(void) {
    const struct device *i2c_dev;
    struct nvs_fs nvs;
    struct flash_sector fs;

    /* I2C device initialization */
    i2c_dev = device_get_binding(I2C_DEV);
    if (!i2c_dev) {
        printk("I2C: Device driver not found.\n");
        return;
    }

    /* NVS initialization with default settings */
    nvs.offset = FLASH_AREA_OFFSET(storage);
    nvs.sector_size = STORAGE_SECTOR_SIZE;
    nvs.sector_count = STORAGE_SECTOR_COUNT;

    /* Attempt to initialize the NVS storage */
    if (nvs_init(&nvs, "PARTITION_LABEL") != 0) {
        printk("Error initializing NVS\n");
        return;
    }

    uint8_t sensor_data = 0; /* Variable to store the sensor data */

    while (1) {
        /* Reading data from the MMA8x5x sensor over I2C */
        if (i2c_reg_read_byte(i2c_dev, MMA8X5X_ADDR, 0x00, &sensor_data) != 0) {
            printk("Failed to read sensor data\n");
        } else {
            printk("Sensor data: %d\n", sensor_data);

            /* Store the data persistently using NVS */
            if (nvs_write(&nvs, STORAGE_KEY, &sensor_data, sizeof(sensor_data)) != sizeof(sensor_data)) {
                printk("Failed to write to NVS\n");
            }
        }

        /* Sleep for the defined interval */
        k_sleep(K_MSEC(SLEEP_TIME_MS));
    }
}