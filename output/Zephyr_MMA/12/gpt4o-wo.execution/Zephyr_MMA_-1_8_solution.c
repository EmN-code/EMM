
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>
#include <zephyr/types.h>
#include <zephyr/sys/printk.h>
#include <zephyr/kernel.h>

#define MMA8X5X_NODE DT_NODELABEL(mma8x5x)
#define FLASH_NODE DT_NODELABEL(flash_controller)
#define STORAGE_NODE DT_NODELABEL(storage)

#define NVS_PARTITION_ID FIXED_PARTITION_ID(storage)
#define NVS_PARTITION_DEVICE FIXED_PARTITION_DEVICE(storage)

LOG_MODULE_REGISTER(main);

void read_sensor_and_store(const struct device *sensor_dev, struct nvs_fs *fs) {
    struct sensor_value accel[3];
    int ret;

    ret = sensor_sample_fetch(sensor_dev);
    if (ret) {
        LOG_ERR("Failed to fetch sample (%d)", ret);
        return;
    }

    ret = sensor_channel_get(sensor_dev, SENSOR_CHAN_ACCEL_XYZ, accel);
    if (ret) {
        LOG_ERR("Failed to get sensor data (%d)", ret);
        return;
    }

    ret = nvs_write(fs, 1, &accel, sizeof(accel));
    if (ret < 0) {
        LOG_ERR("Failed to write to NVS (%d)", ret);
    } else {
        LOG_INF("Stored sensor data in NVS");
    }
}

int main(void) {
    const struct device *sensor_dev = DEVICE_DT_GET(MMA8X5X_NODE);
    const struct device *flash_dev = DEVICE_DT_GET(FLASH_NODE);
    struct nvs_fs fs;
    int ret;

    if (!device_is_ready(sensor_dev)) {
        LOG_ERR("Sensor device not ready");
        return -ENODEV;
    }

    if (!device_is_ready(flash_dev)) {
        LOG_ERR("Flash device not ready");
        return -ENODEV;
    }

    fs.flash_device = flash_dev;
    fs.offset = FIXED_PARTITION_OFFSET(NVS_PARTITION_ID);
    fs.sector_size = FIXED_PARTITION_SIZE(NVS_PARTITION_ID);
    fs.sector_count = 1;

    ret = nvs_init(&fs, NVS_PARTITION_DEVICE);
    if (ret) {
        LOG_ERR("Failed to initialize NVS (%d)", ret);
        return -EIO;
    }

    while (1) {
        read_sensor_and_store(sensor_dev, &fs);
        k_sleep(K_SECONDS(5));
    }

    return 0;
}
