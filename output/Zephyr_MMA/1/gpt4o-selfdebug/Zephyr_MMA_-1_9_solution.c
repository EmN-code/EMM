#include <zephyr/types.h> 
#include <zephyr/sys/printk.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/storage/nvs/nvs.h>
#include <zephyr/kernel.h>

#define SLEEP_TIME_MS   5000
#define MMA8X5X_NODE DT_ALIAS(mma8x5x)
#define FLASH_STORE_OFFSET FLASH_AREA_OFFSET(storage)
#define FLASH_STORE_SIZE   FLASH_AREA_SIZE(storage)

#if !DT_NODE_HAS_STATUS(MMA8X5X_NODE, okay)
#error "Unsupported board: mma8x5x devicetree alias is not defined"
#endif

static const struct device *i2c_dev = DEVICE_DT_GET(DT_BUS(MMA8X5X_NODE));
static const struct device *flash_dev = DEVICE_DT_GET(DT_NODELABEL(flash));
static struct nvs_fs nvs;
static struct i2c_msg msgs[2];

void mma8x5x_read_and_store(void) {
    uint8_t sensor_data[6];
    int err;
    
    /* Configure I2C messages for sensor data reading */
    msgs[0].buf = (uint8_t[]){0x01}; // Set starting register
    msgs[0].len = 1;
    msgs[0].flags = I2C_MSG_WRITE;
    
    msgs[1].buf = sensor_data;
    msgs[1].len = 6;
    msgs[1].flags = I2C_MSG_READ | I2C_MSG_STOP;

    /* Read sensor data */
    err = i2c_transfer(i2c_dev, msgs, 2, DT_REG_ADDR(MMA8X5X_NODE));
    if (err) {
        printk("Failed to read sensor: %d\n", err);
        return;
    }
    
    /* Store reading in NVS */
    ssize_t nvs_err = nvs_write(&nvs, FLASH_STORE_OFFSET, &sensor_data, sizeof(sensor_data));
    if (nvs_err < 0) {
        printk("Failed to write to NVS: %d\n", nvs_err);
    } else {
        printk("Stored sensor data\n");
    }
}

int main(void) {
    /* Ensure devices are ready */
    if (!device_is_ready(i2c_dev)) {
        printk("I2C device not found\n");
        return -1;
    }

    if (!device_is_ready(flash_dev)) {
        printk("Flash device not found\n");
        return -1;
    }

    /* Set up NVS */
    nvs.offset = FLASH_STORE_OFFSET;
    nvs.sector_size = FLASH_STORE_SIZE;
    nvs.sector_count = 3;
    int err = nvs_init(&nvs, flash_dev);
    if (err) {
        printk("NVS initialization failed: %d\n", err);
        return err;
    }

    while (1) {
        mma8x5x_read_and_store();
        k_msleep(SLEEP_TIME_MS);
    }
    return 0;
}