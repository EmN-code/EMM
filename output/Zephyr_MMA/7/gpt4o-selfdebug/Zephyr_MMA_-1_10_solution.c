#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/nvs.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main_module);

void mma8x5x_sample_fetch_handler(const struct device *dev)
{
    struct sensor_value accel[3];
    if (sensor_sample_fetch(dev) < 0) {
        LOG_ERR("Failed to fetch sample");
        return;
    }

    if (sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel) < 0) {
        LOG_ERR("Failed to get sensor channel");
        return;
    }

    LOG_INF("Acceleration: X=%f, Y=%f, Z=%f", sensor_value_to_double(&accel[0]), sensor_value_to_double(&accel[1]), sensor_value_to_double(&accel[2]));
}

int main(void)
{
    const struct device *mma8x5x_dev = device_get_binding("MMA8451");

    if (!mma8x5x_dev) {
        LOG_ERR("No NXP MMA8x5x device found");
        return -1;
    }

    LOG_INF("Found MMA8x5x device");

    while (1) {
        mma8x5x_sample_fetch_handler(mma8x5x_dev);
        k_msleep(5000);
    }

    return 0;
}