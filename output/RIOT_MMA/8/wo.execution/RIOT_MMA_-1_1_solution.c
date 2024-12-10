#include periph_conf.h
#include inttypes.h
#include thread.h
#include net/protnum.h

#define THREAD_PRIORITY_MAIN 5  // set the thread priority
#define READ_INTERVAL (5U)  // set interval to read the sensor in seconds

static mma8x5x_t mma_device;
static const mma8x5x_params_t mma_params = {
    .params = {/* device specific parameters if needed */}
};

void *sensor_read_thread(void *arg) {
    (void)arg;
    int is_ready = mma8x5x_is_ready(&mma_device);
    if (is_ready != 0) {
        // handle error
    }

    while (true) {
        // Read from the sensor
        int x, y, z;
        mma8x5x_read(&mma_device, &x, &y, &z);
        printf("Sensor Readings: X=%d Y=%d Z=%d\n", x, y, z);

        // Sleep for READ_INTERVAL seconds
        thread_sleep(READ_INTERVAL * 1000);
    }

    return NULL;
}

int main(void) {
    // Initialize the sensor
    int init_result = mma8x5x_init(&mma_device, &mma_params);
    if (init_result != 0) {
        // Initialization failed, handle error
        return 1;
    }

    // Create a separate thread for reading sensor data
    thread_create(NULL, 0, THREAD_PRIORITY_MAIN, THREAD_CREATE_STACKTEST, 
                  sensor_read_thread, NULL, "sensor_reader");

    // Additional setup can be done here
    while (true) {
        // Keep the main thread alive
        thread_sleep(1000);
    }

    return 0;
}