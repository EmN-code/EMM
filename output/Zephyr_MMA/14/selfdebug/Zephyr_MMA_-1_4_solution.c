
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/sys/printk.h>

#define MMA8X5X_NODE DT_NODELABEL(mma8x5x)
#define FLASH_NODE DT_CHOSEN(zephyr_flash)

#define NVS_PARTITION_ID FIXED_PARTITION_ID(storage_partition)
#define NVS_PARTITION DT_FLASH_AREA_LABEL_BY_IDX(storage_partition, 0)

static struct nvs_fs fs;

void init_nvs(void) {
    int ret;
    struct flash_sector sector;

    fs.flash_device = DEVICE_DT_GET(FLASH_NODE);
    if (!device_is_ready(fs.flash_device)) {
        printk("Flash device %s is not ready\n", fs.flash_device->name);
        return;
    }

    fs.offset = FIXED_PARTITION_OFFSET(NVS_PARTITION);
    fs.sector_size = FIXED_PARTITION_SIZE(NVS_PARTITION);
    fs.sector_count = 1;

    ret = flash_get_page_info_by_offs(fs.flash_device, fs.offset, &sector);
    if (ret) {
        printk("Failed to get flash sector info: %d\n", ret);
        return;
    }

    ret = nvs_init(&fs, fs.flash_device->name);
    if (ret) {
        printk("Failed to init NVS: %d\n", ret);
        return;
    }
}

void read_and_store_sensor_data(const struct device *dev) {
    struct sensor_value accel[3];
    int ret;

    ret = sensor_sample_fetch(dev);
    if (ret) {
        printk("Failed to fetch sample: %d\n", ret);
        return;
    }

    ret = sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel);
    if (ret) {
        printk("Failed to get sensor data: %d\n", ret);
        return;
    }

    uint8_t data[3];
    data[0] = (uint8_t)sensor_value_to_double(&accel[0]);
    data[1] = (uint8_t)sensor_value_to_double(&accel[1]);
    data[2] = (uint8_t)sensor_value_to_double(&accel[2]);

    ret = nvs_write(&fs, 1, data, sizeof(data));
    if (ret < 0) {
        printk("Failed to write to NVS: %d\n", ret);
    } else {
        printk("Data written to NVS\n");
    }
}

int main(void) {
    const struct device *dev = DEVICE_DT_GET(MMA8X5X_NODE);
    if (!device_is_ready(dev)) {
        printk("Device %s is not ready\n", dev->name);
        return 0;
    }

    init_nvs();

    while (1) {
        read_and_store_sensor_data(dev);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
