#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "stddef.h"
#include "stdbool.h"
#include "stdlib.h"
#include "stdio.h"
#include "esp_system.h"
#include "string.h"
#include "mma8x5x.h"
#include "freeRtos/FreeRTOS.h"
#include "freertos/task.h"
#include "sys/types.h"

#define CONFIG_MMA8X5X_I2C_ADDRESS 0x1D

static mma8x5x_t dev;
static mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = CONFIG_MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

void mma8x5x_read_task(void *pvParameters) {
    mma8x5x_data_t data;
    while (1) {
        if (mma8x5x_is_ready(&dev)) {
            mma8x5x_read(&dev, &data);
            printf("X: %d mg, Y: %d mg, Z: %d mg\n", data.x, data.y, data.z);
        }
        vTaskDelay(pdMS_TO_TICKS(5000)); // Delay for 5 seconds
    }
}

int main(void) {
    // Initialize accelerometer
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize accelerometer\n");
        return -1;
    }

    // Create FreeRTOS task for reading MMA8X5X data
    xTaskCreate(mma8x5x_read_task, "MMA8x5x Read", configMINIMAL_STACK_SIZE, NULL, 1, NULL);

    // Start the scheduler
    vTaskStartScheduler();

    // Should never reach here
    for (;;) {}
}