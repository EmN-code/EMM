#include <zephyr/sys/device_mmio.h>
#include <zephyr/syscalls/device.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>

#define ACCEL_DEV_NODE DT_INST(0, nxp_mma8x5x)
#if !DT_NODE_HAS_STATUS(ACCEL_DEV_NODE, okay)
#error "No NXP MMA8x5x device found"
#endif

const struct device *accelerometer = DEVICE_DT_GET(ACCEL_DEV_NODE);

void main(void) {
    if (!device_is_ready(accelerometer)) {
        printk("Accelerometer is not ready\n");
        return;
    }
    printk("Accelerometer is ready\n");

    while (1) {
        /* Simulate an MMA8x5x reading */
        int reading = 0; // Store a fixed reading value or fetch from accelerometer

        /* Store the reading value in NVS */
        /* NVS related code would be included here */

        /* Wait for 5 seconds */
        k_sleep(K_SECONDS(5));
    }
}