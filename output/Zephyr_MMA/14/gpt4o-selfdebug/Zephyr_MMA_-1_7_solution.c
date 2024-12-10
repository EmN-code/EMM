#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>

#define MMA8X5X_NODE DT_NODELABEL(mma8x5x)
#define NVS_SECTOR_SIZE 0x4000  /* 16K */
#define NVS_SECTOR_COUNT 3
#define NVS_PARTITION storage

static struct nvs_fs fs;
static const struct device *mma8x5x_dev = DEVICE_DT_GET(MMA8X5X_NODE);

void main(void) {
    struct sensor_value accel[3];
    int rc;

    if (!device_is_ready(mma8x5x_dev)) {
        printk("Sensor device not found\n");
        return;
    }

    /* NVS file system configuration */
    fs.offset = FLASH_AREA_OFFSET(NVS_PARTITION);
    fs.sector_size = NVS_SECTOR_SIZE;
    fs.sector_count = NVS_SECTOR_COUNT;
    rc = nvs_mount(&fs);
    if (rc) {
        printk("Flash Init failed (%d)\n", rc);
        return;
    }

    while (1) {
        rc = sensor_sample_fetch(mma8x5x_dev);
        if (rc == 0) {
            rc = sensor_channel_get(mma8x5x_dev, SENSOR_CHAN_ACCEL_XYZ, accel);
            if (rc == 0) {
                printk("Accel X: %d.%06d, Y: %d.%06d, Z: %d.%06d\n",
                       accel[0].val1, accel[0].val2,
                       accel[1].val1, accel[1].val2,
                       accel[2].val1, accel[2].val2);

                /* Persist data in NVS */
                rc = nvs_write(&fs, 1, &accel, sizeof(accel));
                if (rc < 0) {
                    printk("NVS write error (%d)\n", rc);
                }
            }
        }
        k_sleep(K_SECONDS(5));
    }
}