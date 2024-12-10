#include "thread_config.h"
#include "thread_arch.h"
#include "thread_flags.h"
#include "thread.h"
#include "periph_conf.h"
#include "net/protnum.h"
#include <stdio.h>

#include "mma8x5x.h"
#include "xtimer.h"

// Define the device parameters
dht_t mma8x5x_device;
mma8x5x_params_t mma8x5x_params = {
    // Initialize with any specific parameters
};

// Thread for reading MMA8x5x data
void *mma8x5x_read_loop(void *arg) {
    (void)arg; // Unused parameter
    while (1) {
        if (mma8x5x_is_ready(&mma8x5x_device)) {
            mma8x5x_measurement_t meas;
            mma8x5x_read(&mma8x5x_device, &meas);
            printf("Acceleration: X=%d, Y=%d, Z=%d\n", meas.x_axis, meas.y_axis, meas.z_axis);
        } else {
            printf("MMA8x5x not ready\n");
        }
        xtimer_sleep(5); // Sleep for 5 seconds
    }
    return NULL;
}

int main(void) {
    // Initialize MMA8x5x
    if (mma8x5x_init(&mma8x5x_device, &mma8x5x_params) != 0) {
        printf("Failed to initialize MMA8x5x\n");
        return 1;
    }

    // Start the MMA8x5x reading thread
    kernel_pid_t pid = thread_create(
        stack, sizeof(stack),
        THREAD_PRIORITY_MAIN - 1, 0,
        mma8x5x_read_loop, NULL, "mma8x5x_read");

    if (pid <= KERNEL_PID_UNDEF) {
        printf("Error initializing MMA8x5x read thread\n");
        return 1;
    }

    return 0;
}
