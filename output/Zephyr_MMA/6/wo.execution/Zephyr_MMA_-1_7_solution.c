
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main);

#define NVS_PARTITION_ID FIXED_PARTITION_ID(storage)
#define NVS_PARTITION_DEVICE FIXED_PARTITION_DEVICE(storage)

void read_sensor_and_store(const struct device *dev) {
    struct sensor_value accel[3];
    int ret;

    ret = sensor_sample_fetch(dev);
    if (ret) {
        LOG_ERR("Failed to fetch sample (%d)", ret);
        return;
    }

    ret = sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel);
    if (ret) {
        LOG_ERR("Failed to get sensor data (%d)", ret);
        return;
    }

    struct nvs_fs fs;
    fs.offset = FIXED_PARTITION_OFFSET(storage);
    fs.sector_size = FIXED_PARTITION_SIZE(storage);
    fs.sector_count = FIXED_PARTITION_SIZE(storage) / fs.sector_size;
    fs.flash_device = NVS_PARTITION_DEVICE;

    ret = nvs_mount(&fs);
    if (ret) {
        LOG_ERR("Failed to mount NVS (%d)", ret);
        return;
    }

    ret = nvs_write(&fs, 1, &accel, sizeof(accel));
    if (ret < 0) {
        LOG_ERR("Failed to write to NVS (%d)", ret);
    } else {
        LOG_INF("Stored sensor data in NVS");
    }
}

int main(void) {
    const struct device *dev = device_get_binding("MMA8x5x");
    if (!dev) {
        LOG_ERR("Could not get MMA8x5x device");
        return -1;
    }

    while (1) {
        read_sensor_and_store(dev);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
