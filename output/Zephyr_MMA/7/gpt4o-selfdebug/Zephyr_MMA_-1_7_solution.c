#include <zephyr/sys/util.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/device.h>
#include <zephyr/logging/log.h>
#include <zephyr/kernel.h>
#include <zephyr/settings/settings.h>

LOG_MODULE_REGISTER(mma8x5x_reader);

#define SENSOR_NODE DT_INST(0, nxp_mma8652fc)  // Ensure this is the correct path for your device tree

const struct device *mma8x5x_dev;
static char nvs_value[16];  // Buffer to store values as strings

void sensor_read_and_store(void) {
    struct sensor_value accel[3];
    int rc = sensor_sample_fetch(mma8x5x_dev);
    if (rc == 0) {
        rc = sensor_channel_get(mma8x5x_dev, SENSOR_CHAN_ACCEL_XYZ, accel);
        if (rc == 0) {
            LOG_INF("Accel X: %d.%06d, Y: %d.%06d, Z: %d.%06d",
                    accel[0].val1, accel[0].val2,
                    accel[1].val1, accel[1].val2,
                    accel[2].val1, accel[2].val2);

            // Store the reading in NVS
            snprintf(nvs_value, sizeof(nvs_value), "%d.%06d %d.%06d %d.%06d",
                    accel[0].val1, accel[0].val2,
                    accel[1].val1, accel[1].val2,
                    accel[2].val1, accel[2].val2);
            settings_save_one("myapp/sensor", nvs_value, sizeof(nvs_value));
        }
    }
}

int main(void) {
    mma8x5x_dev = device_get_binding(DT_LABEL(SENSOR_NODE));
    if (!mma8x5x_dev) {
        LOG_ERR("Failed to get mma8x5x device binding\n");
        return -1;
    }

    // Initialize settings
    settings_subsys_init();
    settings_load();

    while (1) {
        sensor_read_and_store();
        k_sleep(K_SECONDS(5));
    }

    return 0;
}