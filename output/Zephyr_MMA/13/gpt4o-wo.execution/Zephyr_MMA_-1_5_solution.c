
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>

#define SLEEP_TIME_MS   5000
#define NVS_PARTITION_LABEL "storage_partition"

LOG_MODULE_REGISTER(main);

static struct nvs_fs fs;

void init_nvs(void) {
    int err;
    struct flash_sector *sectors;
    uint32_t sector_count;

    sectors = (struct flash_sector *)DT_FLASH_AREA_STORAGE_PARTITION_INFO_SECTORS;
    sector_count = DT_FLASH_AREA_STORAGE_PARTITION_INFO_SECTOR_COUNT;

    fs.flash_device = device_get_binding(DT_LABEL(DT_ALIAS(NVS_PARTITION_LABEL)));
    if (!fs.flash_device) {
        LOG_ERR("Flash device %s not found", DT_LABEL(DT_ALIAS(NVS_PARTITION_LABEL)));
        return;
    }

    fs.offset = DT_REG_ADDR(DT_ALIAS(NVS_PARTITION_LABEL));
    fs.sector_size = sectors[0].fs_size;
    fs.sector_count = sector_count;

    err = nvs_init(&fs, DT_LABEL(DT_ALIAS(NVS_PARTITION_LABEL)));
    if (err) {
        LOG_ERR("NVS init failed: %d", err);
    }
}

void read_sensor_and_store(const struct device *dev) {
    struct sensor_value accel[3];
    int err;

    if (sensor_sample_fetch(dev) < 0) {
        LOG_ERR("Sensor fetch failed");
        return;
    }

    if (sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel) < 0) {
        LOG_ERR("Sensor channel get failed");
        return;
    }

    err = nvs_write(&fs, 1, &accel, sizeof(accel));
    if (err < 0) {
        LOG_ERR("NVS write failed: %d", err);
    } else {
        LOG_INF("Stored sensor data");
    }
}

int main(void) {
    const struct device *dev = device_get_binding("MMA8x5x");

    if (!dev) {
        LOG_ERR("Device MMA8x5x not found");
        return -1;
    }

    init_nvs();

    while (1) {
        read_sensor_and_store(dev);
        k_sleep(K_MSEC(SLEEP_TIME_MS));
    }

    return 0;
}
