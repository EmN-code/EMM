#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/nvs/nvs.h>

#define I2C_NODE DT_NODELABEL(i2c0)
#define MMA8X5X_NODE DT_NODELABEL(mma8x5x)
#define STORAGE_PARTITION DT_NODELABEL(storage)

const struct device *i2c_dev;
const struct device *mma8x5x_dev;
struct nvs_fs fs;

void setup_nvs()
{
    static struct flash_area *fa;
    int rc = flash_area_open(DT_FLASH_AREA_STORAGE_ID, &fa);
    if (rc < 0) {
        printk("Flash area open failed: %d\n", rc);
        return;
    }
    fs.offset = DT_FLASH_AREA_STORAGE_OFFSET;
    rc = nvs_mount(&fs);
    if (rc) {
        printk("NVS mount failed: %d\n", rc);
    }
}

void read_mma8x5x_and_store()
{
    /* Ensure devices are ready */
    mma8x5x_dev = device_get_binding(DT_LABEL(MMA8X5X_NODE));
    if (!mma8x5x_dev) {
        printk("MMA8x5x device not found\n");
        return;
    }
    
    i2c_dev = device_get_binding(DT_LABEL(I2C_NODE));
    if (!i2c_dev) {
        printk("I2C device not found\n");
        return;
    }

    while (1) {
        struct sensor_value accel_data;
        int rc = sensor_sample_fetch(mma8x5x_dev);
        if (rc == 0) {
            sensor_channel_get(mma8x5x_dev, SENSOR_CHAN_ACCEL_XYZ, &accel_data);
            printk("Acceleration x: %d\n", accel_data.val1);

            rc = nvs_write(&fs, FLASH_STORAGE_ID, &accel_data.val1, sizeof(accel_data.val1));
            if (rc > 0) {
                printk("Stored acceleration value\n");
            } else {
                printk("Failed to store value: %d\n", rc);
            }
        } else {
            printk("Failed to fetch sensor data\n");
        }

        k_sleep(K_SECONDS(5));
    }
}

int main(void)
{
    printk("Zephyr MMA8x5x data logger starting\n");
    setup_nvs();
    read_mma8x5x_and_store();
    return 0;
}