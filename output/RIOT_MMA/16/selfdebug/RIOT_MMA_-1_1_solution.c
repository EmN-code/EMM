#include <thread.h>
#include <thread_config.h>
#include <periph_conf.h>
#include <stdio.h>
#include <ztimer.h>

#include "driver_mma8x5x.h"  // Ensure to have this driver for MMA8x5x

static kernel_pid_t thread_pid;
static char stack[THREAD_STACKSIZE_DEFAULT];

void *mma8x5x_thread(void *arg) {
    mma8x5x_t dev;
    mma8x5x_params_t params = {
        .params = {
            /* configuration parameters specific to your setup */
        }
    };
    
    if (mma8x5x_init(&dev, &params) != 0) {
        printf("Failed to initialize MMA8x5x\n");
        return NULL;
    }

    while (1) {
        if (mma8x5x_is_ready(&dev)) {
            // Add code to read data from the sensor here
            printf("MMA8x5x is ready and data is read\n");
        }
        ztimer_sleep(ZTIMER_MSEC, 5000);  // sleep for 5 seconds
    }

    return NULL;
}

int main(void) {
    thread_pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN, 
                               THREAD_CREATE_STACKTEST, mma8x5x_thread, 
                               NULL, "mma8x5x_thread");

    if (thread_pid <= KERNEL_PID_UNDEF) {
        printf("Error: thread_create failed.\n");
        return -1;
    }

    while (1) {
        thread_yield();
    }

    return 0;
}