#include "ztimer.h"
#include "timex.h"
#include "thread.h"
#include "stdio.h"
#include "msg.h"
#include "mma8x5x.h"
#include "string.h"

#define SLEEPTIME (5U * US_PER_SEC)

static mma8x5x_t dev;
static mma8x5x_data_t data;
mma8x5x_params_t params = {
    .i2c = 0,
    .addr = 0,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

void *read_mma8x5x(void *arg) {
    (void)arg;
    while (1) {
        int res = mma8x5x_init(&dev, &params);
        if (res != MMA8X5X_OK) {
            printf("Error initializing MMA8x5x: %d\n", res);
            ztimer_sleep(ZTIMER_SEC, 1);
            continue;
        }
        
        mma8x5x_read(&dev, &data);
        printf("Acceleration: X: %dmg, Y: %dmg, Z: %dmg\n", data.x, data.y, data.z);

        ztimer_sleep(ZTIMER_USEC, SLEEPTIME);
    }
    
    return NULL;
}

int main(void) {
    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, read_mma8x5x, NULL, "mma8x5x");
    return 0;
}