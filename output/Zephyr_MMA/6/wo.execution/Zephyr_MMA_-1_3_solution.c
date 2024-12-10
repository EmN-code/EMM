
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>

#define SLEEP_TIME_MS   5000
#define STORAGE_PARTITION_ID FIXED_PARTITION_ID(storage_partition)

static struct nvs_fs fs;

void init_nvs(void) {
    struct flash_sector *sector;
    int rc;

    fs.flash_device = FLASH_AREA_DEVICE(STORAGE_PARTITION_ID);
    if (!fs.flash_device) {
        printk("Error: no flash device\n");
        return;
    }

    rc = flash_get_page_info_by_idx(fs.flash_device, 0, &sector);
    if (rc) {
        printk("Error: unable to get flash page info\n");
        return;
    }

    fs.offset = sector->start;
    fs.sector_size = sector->size;
    fs.sector_count = 1;

    rc = nvs_init(&fs, fs.flash_device);
    if (rc) {
        printk("Error: NVS init failed\n");
    }
}

int main(void) {
    const struct device *dev;
    struct sensor_value accel[3];
    int rc;

    dev = device_get_binding("MMA8x5x");
    if (!dev) {
        printk("Error: no device\n");
        return -1;
    }

    init_nvs();

    while (1) {
        rc = sensor_sample_fetch(dev);
        if (rc) {
            printk("Error: sensor sample fetch failed\n");
            continue;
        }

        rc = sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel);
        if (rc) {
            printk("Error: sensor channel get failed\n");
            continue;
        }

        printk("Accel: X=%d.%06d, Y=%d.%06d, Z=%d.%06d\n",
               accel[0].val1, accel[0].val2,
               accel[1].val1, accel[1].val2,
               accel[2].val1, accel[2].val2);

        rc = nvs_write(&fs, 1, &accel, sizeof(accel));
        if (rc < 0) {
            printk("Error: NVS write failed\n");
        }

        k_sleep(K_MSEC(SLEEP_TIME_MS));
    }

    return 0;
}
