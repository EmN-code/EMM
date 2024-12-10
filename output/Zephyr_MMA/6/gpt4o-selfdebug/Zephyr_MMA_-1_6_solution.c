#include <zephyr.h>
#include <device.h>
#include <drivers/i2c.h>
#include <storage/flash_map.h>
#include <storage/flash.h>
#include <nvs/nvs.h>
#include <sys/printk.h>

/* Replace with actual I2C device binding */
#define I2C_DEV DT_LABEL(DT_NODELABEL(i2c0))

/* NVS and MMA8x5x configurations */
#define STORAGE_PARTITION_ID FIXED_PARTITION_ID(storage)
#define NVS_SECTOR_SIZE 4096
#define NVS_SECTOR_COUNT 3
#define MMA8X5X_ADDR 0x1D  // Example I2C address (modify accordingly)

void mma8x5x_read_and_store(const struct device *i2c_dev, struct nvs_fs *nvs)
{
    uint8_t sensor_data[6];
    int err;
    
    struct i2c_msg msgs[2];

    /* Set up I2C messages to read data from MMA8x5x */
    msgs[0].buf = sensor_data;
    msgs[0].len = 1;
    msgs[0].flags = I2C_MSG_WRITE | I2C_MSG_STOP;
    msgs[1].buf = sensor_data;
    msgs[1].len = sizeof(sensor_data);
    msgs[1].flags = I2C_MSG_READ | I2C_MSG_STOP;
    
    err = i2c_transfer(i2c_dev, msgs, 2, MMA8X5X_ADDR);
    if (err) {
        printk("Failed to read MMA8x5x sensor data: %d\n", err);
        return;
    }

    printk("Sensor data read: %02X %02X %02X %02X %02X %02X\n",
            sensor_data[0], sensor_data[1], sensor_data[2],
            sensor_data[3], sensor_data[4], sensor_data[5]);

    /* Store data using NVS */
    err = nvs_write(nvs, NVS_SECTOR_COUNT, sensor_data, sizeof(sensor_data));
    if (err < 0) {
        printk("Failed to write sensor data to NVS: %d\n", err);
    } else {
        printk("Sensor data stored in NVS.\n");
    }
}

void main(void)
{
    const struct device *i2c_dev;
    struct nvs_fs nvs;
    struct flash_pages_info info;
    int err;

    printk("MMA8x5x Sensor Reading to NVS Example\n");

    /* Initialize I2C device */
    i2c_dev = device_get_binding(I2C_DEV);
    if (!i2c_dev) {
        printk("Failed to bind I2C device\n");
        return;
    }

    /* Setup NVS filesystem */
    nvs.offset = FLASH_AREA_OFFSET(storage);
    err = flash_get_page_info_by_offs(device_get_binding(FLASH_AREA_DEV(storage)), nvs.offset, &info);
    if (err) {
        printk("Failed to get page info: %d\n", err);
        return;
    }

    nvs.sector_size = info.size;
    nvs.sector_count = NVS_SECTOR_COUNT;

    err = nvs_init(&nvs, device_get_binding(FLASH_AREA_DEV(storage)));
    if (err) {
        printk("Failed to initialize NVS: %d\n", err);
        return;
    }

    /* Periodically read from MMA8x5x and store in NVS */
    while (1) {
        mma8x5x_read_and_store(i2c_dev, &nvs);
        k_sleep(K_SECONDS(5));
    }
}