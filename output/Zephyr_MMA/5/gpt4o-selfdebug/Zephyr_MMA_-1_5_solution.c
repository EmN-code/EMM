#include <zephyr.h>
#include <drivers/sensor.h>
#include <storage/flash_map.h>
#include <zephyr/devices.h>
#include <string.h>
#include <zephyr/kernel.h>
#include <stdio.h>
#include <storage/flash_map.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/spi.h>
#include <logging/log.h>

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

#define STACKSIZE 1024
#define PRIORITY 7

void main(void) {
    LOG_INF("Starting application");

    const struct device *sensor = device_get_binding(DT_LABEL(DT_INST(0, nxp_mma8652fc)));
    const struct flash_area *fa;
    int rc;

    if (!device_is_ready(sensor)) {
        LOG_ERR("MMA8x5x device not found");
        return;
    }

    LOG_INF("Device found: %s", sensor->name);

    while (1) {
        struct sensor_value acc[3];

        rc = sensor_sample_fetch(sensor);
        if (rc) {
            LOG_ERR("Sensor fetch failed");
            continue;
        }

        sensor_channel_get(sensor, SENSOR_CHAN_ACCEL_XYZ, acc);

        LOG_INF("X: %d, Y: %d, Z: %d", (int)acc[0].val1, (int)acc[1].val1, (int)acc[2].val1);

        rc = flash_area_open(FLASH_AREA_ID(storage), &fa);
        if (rc) {
            LOG_ERR("Failed to open flash area");
            continue;
        }

        rc = flash_area_erase(fa, 0, FLASH_AREA_SIZE(storage));
        if (rc) {
            LOG_ERR("Failed to erase flash area");
        }

        rc = flash_area_write(fa, 0, acc, sizeof(acc));
        if (rc) {
            LOG_ERR("Failed to write to flash area");
        } else {
            LOG_INF("Persisted to flash: X=%d, Y=%d, Z=%d", (int)acc[0].val1, (int)acc[1].val1, (int)acc[2].val1);
        }

        flash_area_close(fa);

        k_sleep(K_SECONDS(5));
    }
}