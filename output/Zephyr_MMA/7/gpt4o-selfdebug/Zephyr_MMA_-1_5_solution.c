#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/storage/flash.h>
#include <zephyr/include/storage/flash_map.h>
#include <zephyr/include/storage/nvs/nvs.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

#define SLEEP_TIME_MS 5000

static const struct device *mma8x5x_dev;
static struct nvs_fs fs;
static uint8_t sensor_id = 1;

void main(void) {
    int rc;
    struct sensor_value accel[3];

    LOG_INF("Starting MMA8x5x sensor data logger");

    mma8x5x_dev = device_get_binding("MMA8X5X");
    if (!mma8x5x_dev) {
        LOG_ERR("Could not get MMA8x5x device");
        return;
    }

    fs.offset = FLASH_AREA_OFFSET(storage);
    fs.sector_size = 4096U;
    fs.sector_count = 3U;
    rc = nvs_init(&fs, DT_CHOSEN_ZEPHYR_FLASH_CONTROLLER_LABEL);

    if (rc) {
        LOG_ERR("Could not initialize NVS");
        return;
    }

    while (1) {
        rc = sensor_sample_fetch(mma8x5x_dev);
        if (rc) {
            LOG_ERR("Sensor sample update error");
            continue;
        }

        rc = sensor_channel_get(mma8x5x_dev, SENSOR_CHAN_ACCEL_XYZ, accel);
        if (rc) {
            LOG_ERR("Cannot read MMA8x5x channels");
            continue;
        }

        rc = nvs_write(&fs, sensor_id, &accel, sizeof(accel));
        if (rc > 0) {
            LOG_INF("Accel X: %d.%06d, Y: %d.%06d, Z: %d.%06d", accel[0].val1, accel[0].val2,
                    accel[1].val1, accel[1].val2, accel[2].val1, accel[2].val2);
        } else {
            LOG_ERR("Failed to write to NVS");
        }

        k_msleep(SLEEP_TIME_MS);
    }
}