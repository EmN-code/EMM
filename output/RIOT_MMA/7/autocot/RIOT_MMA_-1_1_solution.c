#include "thread.h"
#include "periph_conf.h"
#include "mma8x5x.h"

// Define the parameters for the MMA8x5x sensor
const mma8x5x_params_t mma8x5x_params = {
    // Initialize with appropriate values for your specific setup
};

// Define a device structure for the MMA8x5x sensor
mma8x5x_t mma8x5x_dev;

// This function reads the sensor value and prints it
void *read_mma8x5x(void *arg) {
    (void)arg;

    while (1) {
        // Initialize the MMA8x5x sensor
        if (mma8x5x_init(&mma8x5x_dev, &mma8x5x_params) == 0) {
            printf("MMA8x5x sensor initialized successfully\n");
        } else {
            printf("Failed to initialize MMA8x5x sensor\n");
            thread_yield_higher();
            continue;
        }
        
        // Read sensor values (e.g., acceleration values)
        int16_t x, y, z;
        if (mma8x5x_read(&mma8x5x_dev, &x, &y, &z) == 0) {
            printf("Acceleration values: x = %d, y = %d, z = %d\n", x, y, z);
        } else {
            printf("Failed to read acceleration values\n");
        }

        // Sleep for 5 seconds
        xtimer_sleep(5);
    }

    return NULL;
}

int main(void) {
    // Create a thread to read the MMA8x5x sensor every 5 seconds
    thread_create(
        read_mma8x5x_stack, sizeof(read_mma8x5x_stack),
        THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
        read_mma8x5x, NULL, "read_mma8x5x"
    );

    return 0;
}