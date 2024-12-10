#include "xtimer.h"
#include "periph/i2c.h"
#include "msg.h"
#include "mma8x5x.h"

static mma8x5x_t dev;

void *read_sensor(void *arg) {
    (void)arg;
    mma8x5x_data_t data;
    int result;
    
    while (1) {
        result = mma8x5x_read(&dev, &data);
        if (result == 0) {
            printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
        }
        else {
            printf("Error reading from sensor\n");
        }
        
        xtimer_sleep(5);  // Wait for 5 seconds
    }
    
    return NULL;
}

int main(void) {
    msg_t msg;
    msg.content.value = 0;
    
    /* Initialize I2C */
    i2c_init(I2C_DEV(0));

    /* Set parameters for MMA8x5x */
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = 0x1D,  // Default I2C address
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    int res = mma8x5x_init(&dev, &params);
    if (res != 0) {
        printf("Failed to initialize accelerometer\n");
        return 1;
    }

    /* Create a thread to read sensor values */
    msg_t queue[8];
    msg_init_queue(queue, 8);
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, read_sensor, NULL, "read_sensor");

    return 0;
}