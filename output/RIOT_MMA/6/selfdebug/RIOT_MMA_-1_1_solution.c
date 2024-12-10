#include "thread.h"
#include "periph_conf.h"
#include "mma8x5x.h"
#include "kernel_types.h"

// Definitions
#define THREAD_STACKSIZE (THREAD_STACKSIZE_MAIN)
#define THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

// MMA8x5x initialization parameters (customize per your hardware)
mma8x5x_t sensor;
mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = MMA8X5X_ADDR,
};

// Thread stack and variables
char stack[THREAD_STACKSIZE];
kernel_pid_t thread_pid;

void *mma8x5x_thread(void *arg) {
    (void)arg;
    uint16_t x, y, z;
    int res = mma8x5x_init(&sensor, &params);
    if (res != 0) {
        printf("Failed to initialize MMA8x5x sensor, error code %d\n", res);
        return NULL;
    }

    while (1) {
        if (mma8x5x_read(&sensor, &x, &y, &z) != 0) {
            printf("Error reading sensor data\n");
        }
        else {
            printf("X: %d, Y: %d, Z: %d\n", x, y, z);
        }
        thread_sleep(5 * US_PER_SEC);
    }

    return NULL;
}

int main(void) {
    thread_pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY, THREAD_CREATE_STACKTEST, mma8x5x_thread, NULL, "mma8x5x");
    if (thread_pid <= KERNEL_PID_UNDEF) {
        printf("Error creating thread\n");
        return 1;
    }

    return 0;
}