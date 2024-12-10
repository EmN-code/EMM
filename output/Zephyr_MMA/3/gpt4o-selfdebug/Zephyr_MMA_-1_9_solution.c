#include <zephyr/sys/sys_io.h>
#include <zephyr/logging/log.h>
#include <zephyr/device.h>
#include <zephyr/fs/fs.h>
#include <zephyr/devices/sensor.h>
#include <stdio.h>

#define LOG_MODULE_NAME mma8x5x_reader
LOG_MODULE_REGISTER(LOG_MODULE_NAME);

void main(void) {
    const struct device *sensor = device_get_binding("MMA8X5X");
    if (!sensor) {
        LOG_ERR("Failed to find sensor MMA8X5X");
        return;
    }

    struct sensor_value value;
    struct nvs_fs fs;
    
    int rc = nvs_mount(&fs);
    if (rc) {
        LOG_ERR("NVS: Mount failed, error %d", rc);
        return;
    }

    while (1) {
        rc = sensor_sample_fetch(sensor);
        if (rc) {
            LOG_ERR("Failed to fetch sensor data, error %d", rc);
            continue;
        }

        rc = sensor_channel_get(sensor, SENSOR_CHAN_ACCEL_X, &value);
        if (rc) {
            LOG_ERR("Failed to get sensor data, error %d", rc);
            continue;
        }

        LOG_INF("Accel X: %d.%06d", value.val1, value.val2);

        rc = nvs_write(&fs, 1, &value, sizeof(value));
        if (rc < 0) {
            LOG_ERR("Failed to write to NVS, error %d", rc);
        }

        k_sleep(K_SECONDS(5));
    }
}