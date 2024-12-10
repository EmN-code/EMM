#include "thread.h"
#include "periph_conf.h"
#include "net/protnum.h"
#include "PERIPH/i2c.h"
#include "mma8x5x.h"
#include "xtimer.h"

#define I2C_DEV I2C_DEV(0)  // Using I2C device 0 for interaction with MMA8x5x
#define I2C_ADDR 0x1C       // Default I2C address for MMA8x5x

void read_mma8x5x(void) {
    mma8x5x_t sensor;
    mma8x5x_params_t sensor_params = {
        .i2c_dev = I2C_DEV,
        .addr = I2C_ADDR
    };

    if (mma8x5x_init(&sensor, &sensor_params) != 0) {
        puts("Failed to initialize MMA8x5x sensor");
        return;
    }

    while (1) {
        mma8x5x_data_t data;
        if (mma8x5x_read(&sensor, &data) != 0) {
            puts("Failed to retrieve data from MMA8x5x sensor");
        } else {
            printf("Acceleration: x=%d, y=%d, z=%d\n", data.x, data.y, data.z);
        }
        xtimer_sleep(5);
    }
}

int main(void) {
    puts("Starting MMA8x5x reader...");

    // Starting the thread to continuously read MMA8x5x data
    thread_t read_thread;
    char stack[THREAD_STACKSIZE_DEFAULT];

    kernel_pid_t pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                                     THREAD_CREATE_STACKTEST, read_mma8x5x, NULL, "read_mma8x5x");

    if (pid <= KERNEL_PID_UNDEF) {
        puts("Failed to create thread");
    } else {
        puts("MMA8x5x reading thread started");
    }

    return 0;
}