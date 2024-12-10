#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/sensor.h>

#define MMA8X5X_NODE DT_INST(0, nxp_mma8x5x)

// Check if the device is ready
#if DT_NODE_HAS_STATUS(MMA8X5X_NODE, okay)
static const struct device *mma8x5x = DEVICE_DT_GET(MMA8X5X_NODE);
#else
#error "No MMA8x5x device found"
#endif

void main(void) {
    if (!device_is_ready(mma8x5x)) {
        printk("MMA8x5x device not ready\n");
        return;
    }

    struct sensor_value accel[3];

    while (1) {
        // Fetch the sample from the sensor
        if (sensor_sample_fetch(mma8x5x) < 0) {
            printk("Sensor sample update error\n");
            return;
        }

        // Get sensor values
        sensor_channel_get(mma8x5x, SENSOR_CHAN_ACCEL_XYZ, accel);

        printk("Accel X: %d, Y: %d, Z: %d\n", accel[0].val1, accel[1].val1, accel[2].val1);

        // Here we would store the values to non-volatile storage (NVS)
        // However, we will omit that functionality due to lack of necessary library.

        k_sleep(K_SECONDS(5));
    }
}