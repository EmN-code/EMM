#include "xtimer.h"
#include "timex.h"
#include "stdio.h"
#include "mma8x5x.h"
#include "periph/i2c.h"

#define I2C_NUM         0
#define ADDR            MMA8X5X_DEFAULT_ADDR
#define POLL_INTERVAL   (5U * US_PER_SEC)

int main(void) {
    /* Initialize I2C */
    if (i2c_init(I2C_NUM) != 0) {
        printf("Failed to initialize I2C\n");
        return 1;
    }
    
    /* Initialize the MMA8x5x sensor */
    mma8x5x_t dev;
    if (mma8x5x_init(&dev, I2C_NUM, ADDR) != 0) {
        printf("Failed to initialize MMA8x5x\n");
        return 1;
    }
    
    printf("MMA8x5x sensor initialized successfully\n");

    /* Start periodic read */
    while(1) {
        mma8x5x_data_t data;
        if (mma8x5x_read(&dev, &data) != 0) {
            printf("Failed to read data\n");
        } else {
            printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
        }
        
        /* Wait for the next reading */
        xtimer_usleep(POLL_INTERVAL);
    }
    return 0;
}