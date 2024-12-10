#include <zephyr/drivers/sensor.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/nvs/nvs.h>

#define NVS_SECTOR_SIZE 4096
#define NVS_SECTOR_COUNT 3
#define MMA8X5X_LABEL "MMA8X5X"
#define STORAGE_ID 1

void main(void)
{
    struct nvs_fs fs;
    struct sensor_value accel[3];
    const struct device *dev;
    int rc;

    fs.sector_size = NVS_SECTOR_SIZE;
    fs.sector_count = NVS_SECTOR_COUNT;
    fs.offset = FLASH_AREA_OFFSET(storage);

    rc = nvs_init(&fs, DT_LABEL(DT_NODELABEL(storage)));
    if (rc) {
        printk("NVS Init failed: %d\n", rc);
        return;
    }

    dev = device_get_binding(MMA8X5X_LABEL);
    if (!dev) {
        printk("No device found\n");
        return;
    }

    while (1) {
        rc = sensor_sample_fetch(dev);
        if (rc) {
            printk("sensor_sample_fetch failed: %d\n", rc);
            k_sleep(K_SECONDS(5));
            continue;
        }

        rc = sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel);
        if (rc) {
            printk("sensor_channel_get failed: %d\n", rc);
            k_sleep(K_SECONDS(5));
            continue;
        }

        printk("Accel X: %d.%06d, Y: %d.%06d, Z: %d.%06d\n",
               accel[0].val1, accel[0].val2,
               accel[1].val1, accel[1].val2,
               accel[2].val1, accel[2].val2);

        rc = nvs_write(&fs, STORAGE_ID,
                       &accel, sizeof(accel));
        if (rc < 0) {
            printk("NVS write failed: %d\n", rc);
        }

        k_sleep(K_SECONDS(5));
    }
}