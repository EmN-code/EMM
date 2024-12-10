#include <ztimer.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <timex.h>
#include <thread.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <msg.h>
#include <netinet/in.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <mma8x5x_regs.h>
#include <paho_mqtt.h>
#include <arpa/inet.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <dht_params.h>
#include <dht.h>
#include <evtimer_mbox.h>
#include <architecture.h>
#include <xtimer.h>
#include <kernel_defines.h>
#include <evtimer_msg.h>
#include <event/thread.h>
#include <mutex.h>
#include <time_units.h>

#define MMA8X5X_I2C_ADDRESS (0x1D)  // Assuming a default I2C address
#define MMA8X5X_RATE_50HZ   (50)    // Assuming a default sampling rate
#define MMA8X5X_RANGE_2G    (2)     // Assuming a default range

static mma8x5x_t dev;
mma8x5x_data_t data;
mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

// Function to initialize the accelerometer
int initialize_accelerometer(void) {
    int ret;
    while ((ret = mma8x5x_init(&dev, &params)) != MMA8X5X_OK) {
        printf("Accelerometer initialization failed\n");
        xtimer_sleep(1);
    }
    printf("Accelerometer initialized successfully\n");
    return ret;
}

// Function to read data from the accelerometer
void read_accelerometer_data(void) {
    while (1) {
        mma8x5x_read(&dev, &data);
        printf("Acceleration data: X=%d mg, Y=%d mg, Z=%d mg\n", data.raw[0], data.raw[1], data.raw[2]);
        xtimer_sleep(5);
    }
}

int main(void) {
    initialize_accelerometer();
    read_accelerometer_data();
    return 0;
}
