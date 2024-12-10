// Description: 
// This program initializes the MMA8x5x accelerometer on an ESP32 and reads the acceleration data every five seconds. 

#include "thread.h"
#include "periph/rtc.h"
#include "mma8x5x_params.h"
#include "periph/gpio.h"
#include "event/thread.h"
#include "mma8x5x.h"
#include "mma8x5x_regs.h"
#include "periph/i2c.h"
#include "periph_conf.h"
#include <stdio.h>

#define THREAD_STACKSIZE_MAIN   (THREAD_STACKSIZE_DEFAULT)

static char stack[THREAD_STACKSIZE_MAIN];

static void read_sensor(void) {
    mma8x5x_t dev;
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_ADDR_DEFAULT,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };
    
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Error: Unable to initialize the sensor.\n");
        return;
    }
    
    while (1) {
        int16_t x, y, z;
        if (mma8x5x_read(&dev, &x, &y, &z) == MMA8X5X_OK) {
            printf("Acceleration X: %d, Y: %d, Z: %d\n", x, y, z);
        } else {
            printf("Error: Unable to read acceleration.\n");
        }
        xtimer_sleep(5);
    }
}

int main(void) {
    printf("Starting MMA8x5x accelerometer data acquisition\n");
    
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                  read_sensor, NULL, "read_sensor");

    return 0;
}
