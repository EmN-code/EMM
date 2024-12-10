
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>
#include <msg.h>
#include <paho_mqtt.h>
#include <ztimer.h>
#include <stddef.h>
#include <dht.h>
#include <evtimer_mbox.h>
#include <timex.h>
#include <kernel_defines.h>
#include <netinet/in.h>
#include <event/thread.h>
#include <evtimer_msg.h>
#include <time_units.h>
#include <dht_params.h>
#include <mbox.h>
#include <arpa/inet.h>
#include <thread.h>
#include <mma8x5x_regs.h>
#include <stdatomic.h>
#include <mma8x5x.h>
#include <MQTTClient.h>
#include <mma8x5x_params.h>

// Define the device and parameters
static mma8x5x_t dev;
mma8x5x_data_t data;
mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

// Function to initialize the MMA8x5x accelerometer
int mma8x5x_init(mma8x5x_t *dev, const mma8x5x_params_t *params);

// Function to read accelerometer data
void mma8x5x_read(const mma8x5x_t *dev, mma8x5x_data_t *data);

// Main function
int main(void)
{
    // Initialize the MMA8x5x accelerometer
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Accelerometer initialization failed\n");
        return 1;
    }

    // Initialize the message queue
    msg_t msg_array[8];
    msg_init_queue(msg_array, 8);

    // Main loop to read accelerometer data every 5 seconds
    while (1) {
        // Read accelerometer data
        mma8x5x_read(&dev, &data);

        // Print the accelerometer data
        printf("Accelerometer data: x=%d, y=%d, z=%d\n", data.x, data.y, data.z);

        // Sleep for 5 seconds
        sleep(5);
    }

    return 0;
}
