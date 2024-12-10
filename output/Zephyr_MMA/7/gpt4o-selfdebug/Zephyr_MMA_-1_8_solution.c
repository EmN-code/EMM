#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/sensor.h>

#define MMA8x5x DT_INST(0, nxp_mma8652fc)

#if !DT_NODE_HAS_STATUS(MMA8x5x, okay)
#error "No NXP MMA8x5x device found"
#endif

static const struct device *mma8x5x_dev = DEVICE_DT_GET(MMA8x5x);

#if !DEVICE_IS_READY(mma8x5x_dev)
#error "MMA8x5x device is not ready"
#endif

void main(void) {
    struct sensor_value accel[3];
    int ret;

    while (1) {
        ret = sensor_sample_fetch(mma8x5x_dev);
        if (ret) {
            printk("Error: failed to fetch sensor sample (%d)\n", ret);
            continue;
        }

        ret = sensor_channel_get(mma8x5x_dev, SENSOR_CHAN_ACCEL_XYZ, accel);
        if (ret) {
            printk("Error: failed to get sensor channel (%d)\n", ret);
            continue;
        }

        printk("Accel X: %d.%06d ", accel[0].val1, accel[0].val2);
        printk("Y: %d.%06d ", accel[1].val1, accel[1].val2);
        printk("Z: %d.%06d\n", accel[2].val1, accel[2].val2);

        k_sleep(K_SECONDS(5));
    }
}