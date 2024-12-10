#include "thread.h"
#include "periph_conf.h"
#include "mma8x5x.h"

#define MAIN_QUEUE_SIZE     (8)
#define MMA8X5X_THREAD_PRIO (THREAD_PRIORITY_MAIN - 1)

typedef struct {
    mma8x5x_t dev;
    mma8x5x_params_t params;  // Parameters for the sensor
} sensor_data_t;

static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

static const mma8x5x_params_t mma_params = {
    .i2c = I2C_DEV(0), // Use I2C interface
    .addr = MMA8X5X_ADDR(0), // I2C address
    .mode = MMA8X5X_MODE_ACTIVE
};

void *mma8x5x_thread(void *arg) {
    sensor_data_t *sensor_data = (sensor_data_t *)arg;
    while (1) {
        if (mma8x5x_is_ready(&sensor_data->dev)) {
            printf("MMA8X5x Sensor is ready to be read\n");

            // Read sensor data
            int16_t x, y, z;
            mma8x5x_read(&sensor_data->dev, &x, &y, &z);
            printf("Acceleration: x=%d, y=%d, z=%d\n", x, y, z);
        } else {
            printf("Failed to read from MMA8x5x\n");
        }

        // Sleep for 5 seconds
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    puts("Starting MMA8x5x sensor reading program\n");

    sensor_data_t sensor_data;
    mma8x5x_init(&sensor_data.dev, &mma_params);

    // Initialize message queue
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    // Start MMA8x5x reading thread
    thread_create(NULL, THREAD_STACKSIZE_DEFAULT, MMA8X5X_THREAD_PRIO,
                  THREAD_CREATE_STACKTEST, mma8x5x_thread,
                  &sensor_data, "mma8x5x_thread");

    return 0;
}