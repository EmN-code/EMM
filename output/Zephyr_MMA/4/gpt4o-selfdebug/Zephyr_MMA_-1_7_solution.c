#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/fs/nvs.h>

#define STORAGE_NVS_SECTOR_SIZE 4096
#define STORAGE_NVS_SECTOR_COUNT 3
#define MMA8X5X_NODE DT_INST(0, nxp_mma8652)

#if !DT_NODE_HAS_STATUS(MMA8X5X_NODE, okay)
#error "No mma8x5x compatible node found in the device tree"
#endif

#define STORAGE_NODE_ID DT_NODELABEL(storage_partition)
static struct nvs_fs nvs;

void main(void) {
    const struct device *dev = DEVICE_DT_GET(MMA8X5X_NODE);
    if (!device_is_ready(dev)) {
        printk("MMA8x5x device not ready\n");
        return;
    }

    nvs.offset = FLASH_AREA_OFFSET(STORAGE_NODE_ID);
    nvs.sector_size = STORAGE_NVS_SECTOR_SIZE;
    nvs.sector_count = STORAGE_NVS_SECTOR_COUNT;

    int rc = nvs_mount(&nvs);
    if (rc) {
        printk("Failed to mount NVS: %d\n", rc);
        return;
    }

    struct sensor_value accel[3];
    while (1) {
        rc = sensor_sample_fetch(dev);
        if (rc) {
            printk("Failed to fetch sensor samples: %d\n", rc);
            continue;
        }

        rc = sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel);
        if (rc) {
            printk("Failed to get sensor channel: %d\n", rc);
            continue;
        }

        printk("Acceleration (X, Y, Z): %d, %d, %d\n", accel[0].val1, accel[1].val1, accel[2].val1);

        rc = nvs_write(&nvs, RECORD_ACCEL, accel, sizeof(accel));
        if (rc < 0) {
            printk("Failed to write to NVS: %d\n", rc);
        }

        k_sleep(K_SECONDS(5));
    }
}