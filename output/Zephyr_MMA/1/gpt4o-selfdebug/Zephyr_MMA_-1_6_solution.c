#include <zephyr/init.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/storage/nvs.h>

#define SLEEP_TIME_MS   5000
#define SENSOR_NODE DT_ALIAS(mma8x5x)
#define STORAGE_PARTITION "storage"
#define NVS_ID 1

static const struct device *mma8x5x_dev;
static struct nvs_fs fs;

void mma8x5x_read_and_store(void) {
    struct sensor_value accel[3];
    int ret;

    ret = sensor_sample_fetch(mma8x5x_dev);
    if (ret) {
        printk("Failed to fetch sample: %d\n", ret);
        return;
    }

    ret = sensor_channel_get(mma8x5x_dev, SENSOR_CHAN_ACCEL_XYZ, accel);
    if (ret) {
        printk("Failed to get sensor data: %d\n", ret);
        return;
    }

    printk("Acceleration X: %d.%06d, Y: %d.%06d, Z: %d.%06d\n",
           accel[0].val1, accel[0].val2,
           accel[1].val1, accel[1].val2,
           accel[2].val1, accel[2].val2);

    ret = nvs_write(&fs, NVS_ID, &accel, sizeof(accel));
    if (ret < 0) {
        printk("Error writing data to NVS: %d\n", ret);
    }
}

int main(void) {
    int ret;
    struct flash_area *fa;

    printk("Starting MMA8x5x sensor read and NVS storage\n");

    mma8x5x_dev = device_get_binding(DT_LABEL(SENSOR_NODE));
    if (!mma8x5x_dev) {
        printk("No device found\n");
        return -1;
    }

    ret = flash_area_open(FLASH_AREA_ID(STORAGE_PARTITION), &fa);
    if (ret) {
        printk("Flash area open failed\n");
        return ret;
    }

    fs.offset = fa->fa_off;
    ret = nvs_init(&fs, STORAGE_PARTITION);
    if (ret) {
        printk("NVS initialization failed: %d\n", ret);
        return ret;
    }

    while (1) {
        mma8x5x_read_and_store();
        k_sleep(K_MSEC(SLEEP_TIME_MS));
    }

    return 0;
}