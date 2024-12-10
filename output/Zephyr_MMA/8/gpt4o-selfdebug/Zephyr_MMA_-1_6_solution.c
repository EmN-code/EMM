#include <zephyr/devices/sensor.h>
#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/storage/nvs/nvs.h>
#include <zephyr/zephyr.h>

#define NVS_SECTOR_SIZE FLASH_AREA_SIZE(storage)
#define NVS_SECTOR_COUNT 3

static struct nvs_fs fs;
static const struct device *dev;

void main(void) {
    struct sensor_value accel[3];
    struct flash_sector sector;
    int err;

    /* Configure NVS */
    fs.offset = FLASH_AREA_OFFSET(storage);
    fs.sector_size = NVS_SECTOR_SIZE;
    fs.sector_count = NVS_SECTOR_COUNT;

    err = nvs_mount(&fs);
    if (err) {
        printk("NVS mount failed (err %d)\n", err);
        return;
    }

    /* Get sensor device binding */
    dev = DEVICE_DT_GET(DT_NODELABEL(mma8652fc));
    if (!device_is_ready(dev)) {
        printk("Device %s is not ready\n", dev->name);
        return;
    }

    while (1) {
        /* Fetch the accelerometer readings */
        sensor_sample_fetch(dev);
        sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel);

        printk("Acceleration X: %d.%06d\n", accel[0].val1, accel[0].val2);
        printk("Acceleration Y: %d.%06d\n", accel[1].val1, accel[1].val2);
        printk("Acceleration Z: %d.%06d\n", accel[2].val1, accel[2].val2);

        /* Store the X-axis reading in NVS */
        err = nvs_write(&fs, 1, &accel[0], sizeof(accel[0]));
        if (err < 0) {
            printk("NVS write error %d\n", err);
        }

        k_sleep(K_SECONDS(5));
    }
}