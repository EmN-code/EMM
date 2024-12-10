#include "thread.h"
#include "mma8x5x.h"
#include "periph_conf.h"

#define READ_INTERVAL (5U)  // read interval in seconds

static mma8x5x_t dev;
static const mma8x5x_params_t params = {
    // Initialize with hypothetical parameters; these should match your setup
    .params = {
        // Add necessary device-specific parameters here
    },
};

void *sensor_thread(void *arg) {
    (void)arg;
    int err = mma8x5x_init(&dev, &params);
    if (err != 0) {
        printf("Failed to initialize sensor\n");
        return NULL;
    }

    while (1) {
        // Assume a read function exists
        mma8x5x_data_t data;
        err = mma8x5x_read(&dev, &data);
        if (err == 0) {
            printf("Sensor Readings: x = %d, y = %d, z = %d\n", data.x, data.y, data.z);
        } else {
            printf("Sensor read failed\n");
        }
        
        // Sleep for READ_INTERVAL seconds
        thread_sleep(READ_INTERVAL);
    }
    return NULL;
}

int main(void) {
    thread_create(malloc(THREAD_STACKSIZE_DEFAULT),
                  THREAD_STACKSIZE_DEFAULT,
                  THREAD_PRIORITY_MAIN,
                  THREAD_CREATE_STACKTEST,
                  sensor_thread,
                  NULL,
                  "sensor_thread");
    return 0;
}