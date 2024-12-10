#include <zephyr/types.h>  // For Zephyr-specific types
#include <zephyr.h>        // Basic Zephyr headers
#include <drivers/sensor.h> // For accessing sensor drivers
#include <settings/settings.h> // For NVS functionality
#include <device.h>         // For device management


#define SLEEP_TIME_MS 5000
#define SENSOR_NODE DT_ALIAS(mma8x5x)

#if !DT_NODE_HAS_STATUS(SENSOR_NODE, okay)
#error "Unsupported board: mma8x5x sensor devicetree alias is not defined"
#endif

const struct device *mma8x5x;

void main(void) {
    int err;
    mma8x5x = device_get_binding(DT_LABEL(SENSOR_NODE));

    if (!mma8x5x) {
        printk("No device found; did initialization fail?\n");
        return;
    }

    setttings_subsys_init();

    while (1) {
        struct sensor_value accel[3];

        /* Fetch the sample from the MMA8x5x sensor */
        err = sensor_sample_fetch(mma8x5x);
        if (err) {
            printk("Error while fetching the sensor sample: %d\n", err);
            continue;
        }

        err = sensor_channel_get(mma8x5x, SENSOR_CHAN_ACCEL_XYZ, accel);
        if (err) {
            printk("	Error while getting sensor channel data: %d\n", err);
            continue;
        }

        /* Print and store the accelerometer readings in NVS */
        printk("X-axis: %d.%06d, Y-axis: %d.%06d, Z-axis: %d.%06d\n",
                accel[0].val1, accel[0].val2,
                accel[1].val1, accel[1].val2,
                accel[2].val1, accel[2].val2);

        // Here, implement NVS storage code to save the reading

        k_msleep(SLEEP_TIME_MS);
    }
}