#include <zephyr.h>
#include <device.h>
#include <drivers/sensor.h>
#include <storage/flash_map.h>
#include <drivers/nvs.h>
#include <logging/log.h>

LOG_MODULE_REGISTER(mma8x5x_reader);

#define SLEEP_TIME_MS   (5000)
#define SENSOR_LABEL    "MMA8x5x"
#define STORAGE_PARTITION  storage_partition

void main(void)
{
    const struct device *sensor = device_get_binding(SENSOR_LABEL);
    if (!sensor) {
        LOG_ERR("Failed to get sensor device");
        return;
    }

    struct sensor_value accel_x, accel_y, accel_z;
    const struct flash_area *fa;
    struct nvs_fs fs;
    int err;

    /* Initialize NVS */
    err = flash_area_open(STORAGE_PARTITION, &fa);
    if (err) {
        LOG_ERR("Failed to open flash area");
        return;
    }
    fs.offset = fa->fa_off;
    fs.sector_size = fa->fa_size;
    fs.sector_count = 1;  // Assume single-sector storage for simplicity

    err = nvs_init(&fs, fa->fa_device);
    if (err) {
        LOG_ERR("NVS initialization failed");
        return;
    }

    while (1) {
        if (sensor_sample_fetch(sensor) < 0) {
            LOG_ERR("Sensor sample update error");
            continue;
        }

        sensor_channel_get(sensor, SENSOR_CHAN_ACCEL_X, &accel_x);
        sensor_channel_get(sensor, SENSOR_CHAN_ACCEL_Y, &accel_y);
        sensor_channel_get(sensor, SENSOR_CHAN_ACCEL_Z, &accel_z);

        LOG_INF("Accel X: %d.%06d", accel_x.val1, accel_x.val2);
        LOG_INF("Accel Y: %d.%06d", accel_y.val1, accel_y.val2);
        LOG_INF("Accel Z: %d.%06d", accel_z.val1, accel_z.val2);

        /* Store the reading in NVS */
        nvs_write(&fs, 1, &accel_x, sizeof(accel_x));
        nvs_write(&fs, 2, &accel_y, sizeof(accel_y));
        nvs_write(&fs, 3, &accel_z, sizeof(accel_z));

        k_msleep(SLEEP_TIME_MS);
    }
}