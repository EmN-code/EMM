#include <zephyr.h>
#include <sys/printk.h>
#include <device.h>
#include <drivers/sensor.h>
#include <fs/nvs.h>

#define MMA8X5X_LABEL DT_LABEL(DT_INST(0, nxp_mma8652fc))  // Adjust to your sensor
#define STORAGE_PARTITION 0x3f0000  // Adjust storage partition offset as needed

static struct nvs_fs fs;

void sensor_read_and_store(void) {
    const struct device *dev = device_get_binding(MMA8X5X_LABEL);
    if (!dev) {
        printk("No device found
");
        return;
    }

    struct sensor_value accel[3];
    while (1) {
        sensor_sample_fetch(dev);
        sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel);
        printk("Accel X: %d, Y: %d, Z: %d
", accel[0].val1, accel[1].val1, accel[2].val1);

        int res = nvs_write(&fs, 1, &accel, sizeof(accel));
        if (res < 0) {
            printk("NVS write error
");
        }

        k_sleep(K_SECONDS(5));
    }
}

void main(void) {
    struct flash_pages_info info;

    fs.offset = STORAGE_PARTITION;

    int rc = flash_get_page_info_by_offs(device_get_binding(DT_CHOSEN_ZEPHYR_FLASH_CONTROLLER_LABEL),
                                         fs.offset, &info);
    if (rc) {
        printk("Error getting flash page info
");
        return;
    }

    fs.sector_size = info.size;
    fs.sector_count = 3U;

    rc = nvs_init(&fs, DT_CHOSEN_ZEPHYR_FSTORAGE_FLASH_CONTROLLER_LABEL);
    if (rc) {
        printk("NVS init error
");
        return;
    }

    sensor_read_and_store();
}