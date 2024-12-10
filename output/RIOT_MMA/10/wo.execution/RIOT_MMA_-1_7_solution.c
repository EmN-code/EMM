#include "stddef.h"
#include "stdbool.h"
#include "stdlib.h"
#include "stdint.h"
#include "stdio.h"
#include "string.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "xtimer.h"
#include "mma8x5x.h"
#include "mma8x5x_params.h"

static mma8x5x_t dev;
static mma8x5x_data_t data;
mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

void sensor_task(void *pvParameter) {
    // Initialize the sensor
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize the accelerometer!\n");
        while (1);
    }
    
    while (1) {
        // Read data from the sensor
        mma8x5x_read(&dev, &data);
        printf("Acceleration X: %d mg, Y: %d mg, Z: %d mg\n", data.x, data.y, data.z);
        
        // Delay for 5 seconds
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}

int main(void) {
    // Create a FreeRTOS task for the sensor_read function
    xTaskCreate(sensor_task, "sensor_task", 2048, NULL, 5, NULL);
    
    // Start the scheduler
    vTaskStartScheduler();
    
    // Loop forever
    while (1);
    return 0;
}