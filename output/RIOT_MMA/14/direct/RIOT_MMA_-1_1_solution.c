#include "ztimer.h"
#include "timex.h"
#include "mma8x5x.h"
#include "periph/i2c.h"
#include "fmt.h"
#include <stdio.h>

#define INTERVAL (5U * US_PER_SEC) // 5 seconds interval

mma8x5x_t dev;

static void read_sensor(void) {
    int16_t x, y, z;

    if (mma8x5x_read(&dev, &x, &y, &z) == 0) {
        printf("X: %d, Y: %d, Z: %d\n", x, y, z);
    } else {
        printf("Error reading sensor\n");
    }
}

int main(void) {
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_DEFAULT_ADDR,
    };

    if (mma8x5x_init(&dev, &params) != 0) {
        printf("Initialization failed\n");
        return 1;
    }

    printf("Sensor initialized successfully\n");

    while (1) {
        read_sensor();
        ztimer_sleep(ZTIMER_SEC, 5);
    }

    return 0;
}