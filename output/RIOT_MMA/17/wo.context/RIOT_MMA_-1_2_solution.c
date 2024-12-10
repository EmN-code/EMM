#include "xtimer.h"
#include "periph/i2c.h"
#include "mma8x5x.h"

#define I2C_DEV         I2C_DEV(0) 
#define MMA8X5X_ADDR    (MMA8X5X_I2C_ADDRESS)
#define READ_INTERVAL   (5U) // seconds

typedef struct {
    kernel_pid_t sender_pid;
    uint16_t type;
    union {
        void *ptr;
        uint32_t value;
    } content;
} msg_t;

static mma8x5x_t dev;
static mma8x5x_data_t data;

int main(void) {
    mma8x5x_params_t params = {
        .i2c = I2C_DEV,
        .addr = MMA8X5X_ADDR,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("Failed to initialize MMA8x5x!");
        return 1;
    }

    puts("MMA8x5x initialized successfully!");

    while (1) {
        mma8x5x_read(&dev, &data);
        printf("Accel Data - X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);

        xtimer_sleep(READ_INTERVAL);
    }

    return 0;
}