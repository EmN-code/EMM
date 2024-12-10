#include <ztimer.h>
#include <xtimer.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <paho_mqtt.h>
#include <netinet/in.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <mbox.h>
#include <msg.h>
#include <kernel_defines.h>
#include <event/thread.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <dht_params.h>
#include <dht.h>
#include <arpa/inet.h>
#include <MQTTClient.h>
#include <thread.h>
#include <timex.h>
#include <time_units.h>

#define I2C_DEV(0)     /* Define with actual I2C device interface for ESP32 */
#define MMA8X5X_I2C_ADDRESS 0x1D  // Assuming 0x1D is the correct I2C address for MMA8x5x

static mma8x5x_t dev;
static mma8x5x_data_t data;
static const mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

int main(void)
{
    // Initialize the MMA8x5x sensor
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x sensor\n");
        return 1;
    }

    while (1) {
        // Read accelerometer data
        mma8x5x_read(&dev, &data);
        printf("Acceleration in mg: X=%d, Y=%d, Z=%d\n", data.x, data.y, data.z);
        
        // Wait for 5 seconds
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return 0;
}