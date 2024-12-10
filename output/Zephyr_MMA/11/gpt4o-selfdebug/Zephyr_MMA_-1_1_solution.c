#include <zephyr/types.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/sys/printk.h>
#include <zephyr/kernel.h>
#include <device.h>
#include <drivers/sensor.h>
#include <flash.h>
#include <storage/flash_map.h>

#define SLEEP_TIME_MS   (5000)
#define MMA8X5X_NODE DT_INST(0, nxp_mma8652fc)
#define FLASH_AREA_ID  FLASH_AREA_ID(storage)

static const struct flash_area *storage_area;
static struct sensor_value mma_x, mma_y, mma_z;

void main(void) {

    int rc;
    const struct device *mma8x5x_dev = SENSOR_DT_GET(MMA8X5X_NODE);

    if (!device_is_ready(mma8x5x_dev)) {
        printk("Device MMA8x5x is not ready\n");
        return;
    }

    rc = flash_area_open(FLASH_AREA_ID, &storage_area);
    if (rc != 0) {
        printk("Flash area open failed with error %d\n", rc);
        return;
    }

    while (1) {
        rc = sensor_sample_fetch(mma8x5x_dev);
        if (rc == 0) {
            rc = sensor_channel_get(mma8x5x_dev, SENSOR_CHAN_ACCEL_X, &mma_x);
            rc = sensor_channel_get(mma8x5x_dev, SENSOR_CHAN_ACCEL_Y, &mma_y);
            rc = sensor_channel_get(mma8x5x_dev, SENSOR_CHAN_ACCEL_Z, &mma_z);
            printk("Accelerometer X: %d, Y: %d, Z: %d\n", mma_x.val1, mma_y.val1, mma_z.val1);
        }

        rc = flash_area_erase(storage_area, 0, FLASH_AREA_SIZE(storage_area));
        if (rc != 0) {
            printk("Flash erase failed with error %d\n", rc);
        }

        rc = flash_area_write(storage_area, 0, &mma_x.val1, sizeof(mma_x.val1));
        if (rc != 0) {
            printk("Flash write failed with error %d\n", rc);
        }

        k_msleep(SLEEP_TIME_MS);
    }

    flash_area_close(storage_area);
}