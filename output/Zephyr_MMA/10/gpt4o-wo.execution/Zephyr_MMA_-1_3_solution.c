
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/printk.h>

#define SLEEP_TIME_MS   5000
#define NVS_PARTITION   FIXED_PARTITION_ID(storage_partition)

LOG_MODULE_REGISTER(main);

void main(void)
{
    const struct device *dev = device_get_binding("MMA8X5X");
    if (dev == NULL) {
        LOG_ERR("Device not found");
        return;
    }

    struct flash_area *fa;
    int ret = flash_area_open(NVS_PARTITION, &fa);
    if (ret) {
        LOG_ERR("Failed to open flash area");
        return;
    }

    struct nvs_fs fs;
    fs.offset = fa->fa_off;
    fs.sector_size = fa->fa_size;
    fs.sector_count = 1;

    ret = nvs_init(&fs, fa->fa_dev);
    if (ret) {
        LOG_ERR("Failed to init NVS");
        flash_area_close(fa);
        return;
    }

    while (1) {
        struct sensor_value accel[3];
        if (sensor_sample_fetch(dev) < 0) {
            LOG_ERR("Failed to fetch sample");
        } else if (sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel) < 0) {
            LOG_ERR("Failed to get sensor data");
        } else {
            LOG_INF("Accel: X=%d.%06d Y=%d.%06d Z=%d.%06d",
                    accel[0].val1, accel[0].val2,
                    accel[1].val1, accel[1].val2,
                    accel[2].val1, accel[2].val2);

            ret = nvs_write(&fs, 1, accel, sizeof(accel));
            if (ret < 0) {
                LOG_ERR("Failed to write to NVS");
            } else {
                LOG_INF("Data written to NVS");
            }
        }

        k_sleep(K_MSEC(SLEEP_TIME_MS));
    }

    flash_area_close(fa);
}
