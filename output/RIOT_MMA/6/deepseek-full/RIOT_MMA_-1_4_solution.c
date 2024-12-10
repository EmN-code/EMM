
#include <ztimer.h>
#include <stddef.h>
#include <stdbool.h>
#include <msg.h>
#include <evtimer_mbox.h>
#include <timex.h>
#include <kernel_defines.h>
#include <event/thread.h>
#include <evtimer_msg.h>
#include <time_units.h>
#include <dht_params.h>
#include <stdlib.h>
#include <mbox.h>
#include <stdio.h>
#include <xtimer.h>
#include <thread.h>
#include <mma8x5x_regs.h>
#include <unistd.h>
#include <mma8x5x.h>
#include <MQTTClient.h>
#include <mma8x5x_params.h>

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

void event_loop(void *arg)
{
    (void)arg;
    while (1) {
        msg_t msg;
        msg_receive(&msg);
        // Handle the message here
    }
}

int main(void)
{
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    // Initialize the MMA8x5x accelerometer
    mma8x5x_t dev;
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = CONFIG_MMA8X5X_I2C_ADDRESS, // Corrected from MMA8X5X_I2C_ADDRESS
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("MMA8x5x accelerometer initialization failed");
        return 1;
    }

    // Create the event loop thread
    thread_t *event_loop_thread = thread_create(NULL, 512, THREAD_PRIORITY_MAIN - 1,
                                                THREAD_CREATE_STACKTEST, event_loop, NULL, "event_loop");

    if (event_loop_thread == NULL) {
        puts("Failed to create event loop thread");
        return 1;
    }

    // Read accelerometer data every 5 seconds
    while (1) {
        mma8x5x_data_t data;
        mma8x5x_read(&dev, &data);
        printf("Accelerometer data: x=%d, y=%d, z=%d\n", data.x, data.y, data.z);
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return 0;
}
