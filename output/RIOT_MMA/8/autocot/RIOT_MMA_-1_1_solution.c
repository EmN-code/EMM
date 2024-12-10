#include <thread.h>
#include <mma8x5x.h>
#include <periph_conf.h>

#define THREAD_PRIORITY_MAIN    THREAD_PRIORITY_MAIN - 1
#define MMA8X5X_READ_INTERVAL   (5U)

mma8x5x_t dev;
mma8x5x_params_t params = {
    .foo = 1, // Example parameter, it should be changed according to mma8x5x_params_t structure
};

void *mma8x5x_thread(void *arg) {
    (void)arg;
    while (1) {
        if (mma8x5x_is_ready(&dev)) {
            // Assume read function and print or process the data
            printf("Reading data from MMA8x5x...\n");
            
            // Add actual sensor reading logic here, e.g.
            // int16_t data;
            // mma8x5x_read(&dev, &data);
            // printf("Data: %d\n", data);
        } else {
            printf("MMA8x5x not ready\n");
        }
        thread_sleep(MMA8X5X_READ_INTERVAL);
    }
    return NULL;
}

int main(void) {
    if (mma8x5x_init(&dev, &params) != 0) {
        printf("Failed to initialize MMA8x5x sensor\n");
        return 1;
    }

    thread_create(NULL, 0, THREAD_PRIORITY_MAIN, 0, mma8x5x_thread, NULL, "mma8x5x_thread");

    return 0;
}