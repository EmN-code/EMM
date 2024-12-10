#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/init.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/storage/nvs.h>

#define I2C_DEV DT_LABEL(DT_NODELABEL(i2c0))
#define MMA8X5X_I2C_ADDR 0x1D
#define SLEEP_TIME K_SECONDS(5)
#define NVS_FLASH_SECTOR_SIZE 4096
#define NVS_STORAGE_OFFSET 0

static const struct device *i2c_dev;
static struct nvs_fs nvs;

void configure_i2c(void) {
    int ret;
    uint32_t i2c_cfg = I2C_SPEED_SET(I2C_SPEED_STANDARD) | I2C_MODE_MASTER;
    ret = i2c_configure(i2c_dev, i2c_cfg);
    if (ret != 0) {
        printk("I2C configuration failed with error %d\n", ret);
    }
}

void mma8x5x_read_and_store(void) {
    uint8_t sensor_data[6];
    struct i2c_msg msgs[2];

    // Configure I2C messages for reading the sensor
    msgs[0].buf = &sensor_data[0];
    msgs[0].len = 1;
    msgs[0].flags = I2C_MSG_WRITE;
    msgs[1].buf = &sensor_data[0];
    msgs[1].len = 6;
    msgs[1].flags = I2C_MSG_RESTART | I2C_MSG_READ | I2C_MSG_STOP;

    // Perform I2C transfer
    int ret = i2c_transfer(i2c_dev, msgs, 2, MMA8X5X_I2C_ADDR);
    if (ret != 0) {
        printk("I2C read failed with error %d\n", ret);
    }

    // Store the reading in NVS
    ret = nvs_write(&nvs, MMA8X5X_I2C_ADDR, sensor_data, sizeof(sensor_data));
    if (ret < 0) {
        printk("Failed to store sensor data in NVS (error %d)\n", ret);
    }
}

int main(void) {
    int ret;

    printk("Starting MMA8x5x sensor read and store example\n");

    i2c_dev = device_get_binding(I2C_DEV);
    if (!i2c_dev) {
        printk("I2C: Device driver not found.\n");
    }

    configure_i2c();

    // NVS Configuration
    nvs.offset = NVS_STORAGE_OFFSET;
    nvs.sector_size = NVS_FLASH_SECTOR_SIZE;
    nvs.sector_count = 3;

    ret = nvs_init(&nvs, DT_LABEL(DT_NODELABEL(flash_controller)));
    if (ret) {
        printk("NVS init failed (error %d)\n", ret);
    }

    while (1) {
        mma8x5x_read_and_store();
        k_sleep(SLEEP_TIME);
    }

    return 0;
}