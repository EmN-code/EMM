
#include "architecture.h"
#include "stddef.h"
#include "stdlib.h"
#include "mma8x5x.h"
#include "mutex.h"
#include "freertos/task.h"
#include "thread.h"
#include "kernel_defines.h"
#include "unistd.h"
#include "freertos/FreeRTOS.h"
#include "assert.h"
#include "stdatomic.h"
#include "sched.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "periph/i2c.h"
#include "stdio.h"
#include "shell.h"
#include "esp_system.h"
#include "msg.h"

#define MMA8X5X_I2C_ADDRESS CONFIG_MMA8X5X_I2C_ADDRESS
#define MMA8X5X_RATE_50HZ 50
#define MMA8X5X_RANGE_2G 2

static mma8x5x_t dev;
static mma8x5x_data_t data;

void read_accelerometer(void *arg) {
    (void)arg;
    while (1) {
        mma8x5x_read(&dev, &data);
        printf("Acceleration: X: %d mg, Y: %d mg, Z: %d mg\n", data.x, data.y, data.z);
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}

int main(void) {
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    int res = mma8x5x_init(&dev, &params);
    if (res != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x accelerometer\n");
        return 1;
    }

    xTaskCreate(read_accelerometer, "read_acc", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    vTaskStartScheduler();

    return 0;
}
