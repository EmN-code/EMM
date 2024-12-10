#include "thread.h"
#include "periph_conf.h"
#include "mma8x5x.h"
#include <stdint.h>

#define SLEEP_TIME (5U * US_PER_SEC) // Sleep for 5 seconds

static mma8x5x_t dev;
static mma8x5x_params_t params = {
    .params = {/*initialize parameters specific to your setup*/}
};

void *sensor_thread(void *arg) {
    (void)arg;
    if (mma8x5x_init(&dev, &params) != 0) {
        printf("Error initializing MMA8x5x sensor\n");
        return NULL;
    }
    else {
        printf("MMA8x5x sensor initialized successfully\n");
    }

    while (1) {
        int is_ready = mma8x5x_is_ready(&dev);
        if (is_ready) {
            printf("Sensor data is ready to be read.\n");
            // Here, you would typically invoke a function to read data from the sensor
        }
        else {
            printf("Sensor not ready.\n");
        }
        thread_sleep(SLEEP_TIME);
    }

    return NULL;
}

int main(void) {
    thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1, 0, sensor_thread, NULL, "sensor");
    return 0;
}