#include <stdio.h>
#include "xtimer.h"
#include "mma8x5x.h"
#include "msg.h"

#define DELAY (5U * US_PER_SEC) // 5 seconds

int main(void) {
    printf("RIOT on ESP32: Reading from MMA8x5x sensor every 5 seconds.\n");

    mma8x5x_t dev;
    if (mma8x5x_init(&dev, I2C_DEV(0)) != 0) {
        printf("Failed to initialize MMA8x5x sensor\n");
        return 1;
    }

    while (1) {
        mma8x5x_3d_data_t data;
        if (mma8x5x_read(&dev, &data) == 0) {
            printf("Acceleration: x=%d y=%d z=%d\n", data.x, data.y, data.z);
        } else {
            printf("Error reading MMA8x5x sensor\n");
        }
        xtimer_sleep(DELAY);
    }

    return 0;
}