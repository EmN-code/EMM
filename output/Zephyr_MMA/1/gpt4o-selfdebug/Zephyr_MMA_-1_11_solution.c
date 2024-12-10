#include <zephyr.h>
#include <device.h>
#include <drivers/sensor.h>
#include <drivers/sensor/mma8x5x.h>
#include <zephyr/drivers/storage/nvs.h>
#include <storage/flash_map.h>

#define STORAGE_PARTITION    storage_partition_label
#define NVS_SECTOR_SIZE      4096
#define NVS_SECTOR_COUNT     3
#define NVS_STORAGE_OFFSET   FLASH_AREA_OFFSET(STORAGE_PARTITION)

struct nvs_fs fs;

void main(void) {
    const struct device *mma8x5x_dev;
    struct sensor_value accel[3];
    int ret;

    // Initialize the device
    mma8x5x_dev = device_get_binding("MMA8X5X");
    if (!mma8x5x_dev) {
        printk("Device not found\n");
        return;
    }
    printk("Found sensor device\n");

    // Setup NVS
    fs.sector_size = NVS_SECTOR_SIZE;
    fs.sector_count = NVS_SECTOR_COUNT;
    fs.offset = NVS_STORAGE_OFFSET;

    ret = nvs_init(&fs, DT_CHOSEN_ZEPHYR_FLASH_CONTROLLER_LABEL);
    if (ret) {
        printk("NVS init failed: %d\n", ret);
        return;
    }

    while (1) {
        ret = sensor_sample_fetch(mma8x5x_dev);
        if (ret) {
            printk("Sensor fetch failed: %d\n", ret);
        } else {
            sensor_channel_get(mma8x5x_dev, SENSOR_CHAN_ACCEL_XYZ, accel);
            printk("Ax = %d.%06d, Ay = %d.%06d, Az = %d.%06d\n",
                   accel[0].val1, accel[0].val2,
                   accel[1].val1, accel[1].val2,
                   accel[2].val1, accel[2].val2);

            // Store the readings in NVS
            ret = nvs_write(&fs, 1, &accel, sizeof(accel));
            if (ret < 0) {
                printk("NVS write error: %d\n", ret);
            } else {
                printk("Stored values to NVS\n");
            }
        }
        k_sleep(K_SECONDS(5));
    }
}