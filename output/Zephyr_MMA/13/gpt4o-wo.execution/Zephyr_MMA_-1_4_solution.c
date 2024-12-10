
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>

#define SENSOR_NODE DT_ALIAS(sensor)
#define NVS_PARTITION DT_LABEL(DT_ALIAS(storage_partition))

static struct nvs_fs fs;

void init_nvs(void) {
    struct flash_sector *sectors;
    int sector_count;

    sectors = (struct flash_sector *)DT_FLASH_AREA_STORAGE_PARTITION_INFO_SECTORS;
    sector_count = DT_FLASH_AREA_STORAGE_PARTITION_INFO_SECTOR_COUNT;

    fs.flash_device = device_get_binding(NVS_PARTITION);
    if (!fs.flash_device) {
        printk("Error: Flash device %s not found\n", NVS_PARTITION);
        return;
    }

    fs.offset = DT_REG_ADDR(DT_ALIAS(storage_partition));
    fs.sector_size = sectors[0].fs_size;
    fs.sector_count = sector_count;

    int ret = nvs_init(&fs, NVS_PARTITION);
    if (ret) {
        printk("Error initializing NVS (%d)\n", ret);
    }
}

void read_sensor_and_store(void) {
    const struct device *dev = device_get_binding(DT_LABEL(SENSOR_NODE));
    if (!dev) {
        printk("Error: Sensor device %s not found\n", DT_LABEL(SENSOR_NODE));
        return;
    }

    struct sensor_value accel[3];
    int ret = sensor_sample_fetch(dev);
    if (ret) {
        printk("Error fetching sensor sample (%d)\n", ret);
        return;
    }

    ret = sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel);
    if (ret) {
        printk("Error getting sensor channel data (%d)\n", ret);
        return;
    }

    uint16_t id = 1; // Example ID for the sensor data
    ret = nvs_write(&fs, id, &accel, sizeof(accel));
    if (ret < 0) {
        printk("Error writing to NVS (%d)\n", ret);
    } else {
        printk("Sensor data written to NVS\n");
    }
}

void main(void) {
    init_nvs();

    while (1) {
        read_sensor_and_store();
        k_sleep(K_SECONDS(5));
    }
}
