#include <thread.h>
#include <periph/i2c.h>
#include <mma8x5x.h>
#include <xtimer.h>

// I2C and MMA8x5x Configurations
#define I2C_INTERFACE  I2C_DEV(0)
#define MMA8X5X_ADDR   (0x1D)   // Example default address
#define TIMER_INTERVAL (5U * US_PER_SEC)   // 5 seconds interval

static mma8x5x_t dev;

// Function to read and print MMA8x5x data
void read_mma8x5x_data(void) {
    int16_t x, y, z;
    if (mma8x5x_read(&dev, &x, &y, &z) == 0) {
        printf("MMA8x5x Axis Data: X: %d Y: %d Z: %d\n", x, y, z);
    } else {
        printf("Error reading from MMA8x5x sensor\n");
    }
}

void *mma8x5x_thread(void *arg) {
    (void)arg;
    while (1) {
        read_mma8x5x_data();
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    i2c_init(I2C_INTERFACE);
    
    mma8x5x_params_t params;
    params.i2c      = I2C_INTERFACE;
    params.address  = MMA8X5X_ADDR;
    
    if (mma8x5x_init(&dev, &params) != 0) {
        printf("Failed to initialize MMA8x5x\n");
        return 1;
    }
    printf("MMA8x5x initialized\n");
    
    thread_create(NULL, 0, THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, mma8x5x_thread, NULL, "mma8x5x");
    
    return 0;
}