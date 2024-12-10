#include "thread.h"
#include "mma8x5x.h"
#include "event/thread.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"

static mma8x5x_t dev;
static mma8x5x_data_t data;
mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = CONFIG_MMA8X5X_I2C_ADDRESS, // Ensure this is defined
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_8G
};

void *reader_thread(void *arg) {
    (void)arg; // unused
    while(1) {
        mma8x5x_read(&dev, &data);
        printf("Acceleration Data: x: %d mg, y: %d mg, z: %d mg\n", data.x, data.y, data.z);
        thread_sleep(5);
    }
    return NULL;
}

int main(void) {
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x\n");
        return 1;
    }
    
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, reader_thread, NULL, "reader");
    
    return 0;
}