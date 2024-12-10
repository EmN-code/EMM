#include "ztimer.h"  // For timer functionality
#include "xtimer.h"  // Additional timer features
#include "timex.h"   // Time calculations
#include "thread.h"  // For threading capabilities
#include "msg.h"     // Message passing system
#include "mma8x5x.h" // MMA8x5x driver implementation
#include "MQTTClient.h" // MQTT client for Paho library
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "mbox.h"
#include "periph_conf.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "mma8x5x_regs.h"
#include "periph/gpio.h"
#include "mma8x5x_params.h"
#include "event/thread.h"
#include "dht_params.h"
#include "time_units.h"  

#define I2C_BUS            0
#define MMA8X5X_DEV_ADDR   0x1D  // This needs to match your specific setup
#define MMA8X5X_RATE       MMA8X5X_RATE_50HZ
#define MMA8X5X_RANGE      MMA8X5X_RANGE_2G

static mma8x5x_t dev;
static mma8x5x_data_t data;
mma8x5x_params_t params = {
    .i2c = I2C_BUS,
    .addr = MMA8X5X_DEV_ADDR,
    .rate = MMA8X5X_RATE,
    .range = MMA8X5X_RANGE
};

void *sensor_thread(void *arg) {
    (void)arg;
    while (1) {
        // Read data from sensor
        mma8x5x_read(&dev, &data);
        printf("Acceleration - X: %d mg, Y: %d mg, Z: %d mg\n", data.x, data.y, data.z);
        // Delay for 5 seconds
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }
    return NULL;
}

int main(void) {
    // Initialize MMA8x5x sensor
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("Error: Unable to initialize MMA8x5x sensor\n");
        return 1;
    }

    // Start a thread to read sensor data
    char thread_stack[THREAD_STACKSIZE_MAIN];
    if (thread_create(thread_stack, sizeof(thread_stack),
                      THREAD_PRIORITY_MAIN - 1,
                      THREAD_CREATE_STACKTEST,
                      sensor_thread, NULL, "sensor_thread") <= KERNEL_PID_UNDEF) {
        puts("Error: Unable to start sensor thread\n");
        return 1;
    }

    return 0;
}