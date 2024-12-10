#include <zephyr/devicetree.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>
#include <zephyr/kernel.h>
#include <zephyr/storage/nvs.h>

LOG_MODULE_REGISTER(main);

#define SLEEP_TIME_MS   5000
#define STORAGE_PARTITION_LABEL    storage

static struct nvs_fs fs;

void main(void) {
    int rc;
    struct sensor_value accel[3];
    const struct device *sensor = DEVICE_DT_GET_ONE(nxp_mma8652);

    if (!device_is_ready(sensor)) {
        LOG_ERR("Device not ready");
        return;
    }

    fs.offset = DT_FIXED_PARTITION_OFFSET(STORAGE_PARTITION_LABEL);
    rc = nvs_init(&fs, STORAGE_PARTITION_LABEL);
    
    if (rc) {
        LOG_ERR("Flash Init failed");
    }

    while (1) {
        /* Fetch the sensor reading */
        rc = sensor_sample_fetch(sensor);
        if (rc) {
            LOG_ERR("Sensor fetch failed");
            continue;
        }

        rc = sensor_channel_get(sensor, SENSOR_CHAN_ACCEL_XYZ, accel);
        if (rc) {
            LOG_ERR("Channel get failed");
            continue;
        }

        LOG_INF("Accel X %d, Y %d, Z %d", accel[0].val1, accel[1].val1, accel[2].val1);

        /* Store the reading in NVS */
        rc = nvs_write(&fs, 1, &accel[0], sizeof(accel) * 3);
        if (rc < 0) {
            LOG_ERR("NVS write error");
        }

        k_msleep(SLEEP_TIME_MS);
    }
}