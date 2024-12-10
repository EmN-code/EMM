#include <thread.h> 
#include <kernel.h> 
#include <xtimer.h> 
#include <stdio.h> 
#include <periph/i2c.h> 
#include <mma8x5x.h> 

// Define I2C interface and address for the MMA8x5x
static const i2c_t i2c_dev = I2C_DEV(0);
static const uint16_t mma8x5x_addr = 0x1D; // Default I2C address for MMA8X5X

// Thread stack
#define THREAD_STACKSIZE_MAIN    (THREAD_STACKSIZE_DEFAULT)

// Function to initialize the MMA8x5x sensor
int init_mma8x5x(mma8x5x_t *dev, mma8x5x_params_t *params) {
    params->i2c = i2c_dev;
    params->i2c_addr = mma8x5x_addr;
    return mma8x5x_init(dev, params);
}

// Thread function to read sensor data
void *read_mma8x5x(void *arg) {
    (void)arg;
    mma8x5x_t dev;
    mma8x5x_params_t params;

    if (init_mma8x5x(&dev, &params) != 0) {
        printf("Failed to initialize MMA8x5x sensor\n");
        return NULL;
    }
    
    while (1) {
        int16_t x, y, z;

        // Read acceleration data
        if (mma8x5x_read(&dev, &x, &y, &z) == 0) {
            printf("Acceleration [mg]: X:%d Y:%d Z:%d\n", x, y, z);
        } else {
            printf("Failed to read acceleration data\n");
        }

        // Wait for 5 seconds
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    // Create a thread to read from the MMA8x5x sensor
    static char read_stack[THREAD_STACKSIZE_MAIN];
    thread_create(read_stack, sizeof(read_stack), THREAD_PRIORITY_MAIN - 1, 0,
                  read_mma8x5x, NULL, "read_mma8x5x");

    return 0;
}
