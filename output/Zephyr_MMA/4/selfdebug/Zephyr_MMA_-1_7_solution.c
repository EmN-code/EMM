
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/sys/printk.h>

#define MMA8X5X_NODE DT_NODELABEL(mma8x5x)
#define STORAGE_PARTITION_ID FIXED_PARTITION_ID(storage_partition)

static struct nvs_fs fs;

void init_nvs(void) {
    int ret;
    struct flash_sector *sectors;
    uint8_t num_sectors;

    sectors = (struct flash_sector *)flash_map_get_sectors(STORAGE_PARTITION_ID, &num_sectors);
    if (!sectors) {
        printk("Failed to get flash sectors\n");
        return;
    }

    fs.flash_device = FLASH_AREA_DEVICE(STORAGE_PARTITION_ID);
    fs.offset = sectors[0].fs_off;
    fs.sector_size = sectors[0].fs_size;
    fs.sector_count = num_sectors;

    ret = nvs_init(&fs, fs.flash_device);
    if (ret) {
        printk("NVS init failed: %d\n", ret);
    }
}

void read_sensor_and_store(const struct device *dev) {
    struct sensor_value accel[3];
    int ret;

    ret = sensor_sample_fetch(dev);
    if (ret) {
        printk("Sensor sample fetch failed: %d\n", ret);
        return;
    }

    ret = sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel);
    if (ret) {
        printk("Sensor channel get failed: %d\n", ret);
        return;
    }

    printk("Accel: x=%d.%06d, y=%d.%06d, z=%d.%06d\n",
           accel[0].val1, accel[0].val2,
           accel[1].val1, accel[1].val2,
           accel[2].val1, accel[2].val2);

    ret = nvs_write(&fs, 1, accel, sizeof(accel));
    if (ret < 0) {
        printk("NVS write failed: %d\n", ret);
    }
}

int main(void) {
    const struct device *dev;

    dev = DEVICE_DT_GET(MMA8X5X_NODE);
    if (!device_is_ready(dev)) {
        printk("Device %s is not ready\n", dev->name);
        return 0;
    }

    init_nvs();

    while (1) {
        read_sensor_and_store(dev);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
