// Description: This code initializes the MMA8x5x accelerometer and reads its data every 5 seconds.

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stddef.h>
#include <stdatomic.h>
#include <esp_system.h>
#include <atomic.h>
#include <mma8x5x.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <event/thread.h>
#include <evtimer_mbox.h>
#include <xtimer.h>
#include <timex.h>
#include <paho_mqtt.h>
#include <MQTTClient.h>
#include <unistd.h>
#include <thread.h>
#include <time_units.h>
#include <msg.h>
#include <mbox.h>
#include <freertos/task.h>
#include <dht_params.h>
#include <evtimer_msg.h>
#include <kernel_defines.h>

#define MMA8X5X_I2C_ADDRESS (0x1D) // Default I2C address for MMA8x5x
#define I2C_DEV(dev_num) (dev_num) // Placeholder macro for I2C device
#define MMA8X5X_RATE_50HZ (50) // Sampling rate
#define MMA8X5X_RANGE_2G (2) // G range

static mma8x5x_t dev;
mma8x5x_data_t data;
mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

void read_accelerometer_task(void *pvParameters) {
    while (true) {
        if (mma8x5x_is_ready(&dev)) {
            mma8x5x_read(&dev, &data);
            printf("X: %d mg, Y: %d mg, Z: %d mg\n", data.x, data.y, data.z);
        }
        vTaskDelay(pdMS_TO_TICKS(5000)); // Delay for 5000 milliseconds (5 seconds)
    }
}

int main(void) {
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Error: Unable to initialize the accelerometer\n");
        return 1;
    }

    xTaskCreate(
        read_accelerometer_task, // Task function
        "ReadAccel",             // Task name
        2048,                    // Stack size
        NULL,                    // Parameters
        tskIDLE_PRIORITY,        // Priority
        NULL                     // Task handle
    );

    vTaskStartScheduler(); // Start the FreeRTOS scheduler
    return 0;
}