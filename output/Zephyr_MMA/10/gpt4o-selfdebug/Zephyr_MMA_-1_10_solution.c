// Device Tree Overlay
// Make sure this overlay file is applied when building your application

&i2c0 {
    mma8x5x: mma8x5x@1c {
        compatible = "nxp,mma8x5x";
        reg = <0x1C>;
        label = "MMA8X5X";
    };
};

// main.c file
#include <zephyr/sys/util.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>

#define MMA8X5X_NODE DT_ALIAS(mma8x5x)
#define STORAGE_PARTITION_ID FIXED_PARTITION_ID(storage_partition)

struct nvs_fs fs;

void main(void) {
    const struct device *dev = DEVICE_DT_GET(MMA8X5X_NODE);
    struct sensor_value mma_value;
    int rc;

    // NVS file system setup
    struct flash_area *fa;

    rc = flash_area_open(STORAGE_PARTITION_ID, &fa);
    if (rc) {
        printk("Flash area open failure: error %d\n", rc);
        return;
    }

    fs.offset = fa->fa_off;
    fs.sector_size = fa->fa_size;
    fs.sector_count = 1U;

    rc = nvs_init(&fs, DT_LABEL(DT_CHOSEN(zephyr_flash_ctrl)));  
    if (rc) {
        printk("NVS init failure: error %d\n", rc);
        return;
    }

    // Checking if the device is ready
    if (!device_is_ready(dev)) {
        printk("Device %s is not ready\n", dev->name);
        return;
    }

    // Timer setup to trigger every 5 seconds
    while (1) {
        rc = sensor_sample_fetch(dev);

        if (rc) {
            printk("Sensor fetch error: %d\n", rc);
        } else {
            rc = sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, &mma_value);

            if (rc) {
                printk("Sensor channel get error: %d\n", rc);
            } else {
                printk("Acceleration X: %d.%06d, Y: %d.%06d, Z: %d.%06d\n",
                        mma_value.val1, mma_value.val2,
                        mma_value.val1, mma_value.val2,
                        mma_value.val1, mma_value.val2);

                // Store in NVS
                rc = nvs_write(&fs, 1, &mma_value, sizeof(mma_value));
                if (rc < 0) {
                    printk("NVS Write Error: %d\n", rc);
                }
            }
        }
        k_sleep(K_SECONDS(5));
    }
}
