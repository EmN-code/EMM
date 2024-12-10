#include <thread.h>
#include <periph_conf.h>
#include <inttypes.h>

#include <mma8x5x.h>

static mma8x5x_t dev;
static kernel_pid_t main_thread_pid;

static void* read_sensor(void *arg) {
    (void)arg;
    int16_t x, y, z;
    while (1) {
        if (mma8x5x_read(&dev, &x, &y, &z) != 0) {
            printf("Error reading sensor data\n");
        } else {
            printf("Accel: x=%d y=%d z=%d\n", x, y, z);
        }
        // Sleep for 5 seconds
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    main_thread_pid = thread_getpid();

    // Device initialization
    mma8x5x_params_t params = MMA8X5X_PARAMS;
    if (mma8x5x_init(&dev, &params) != 0) {
        printf("Failed to initialize sensor\n");
        return 1;
    }

    // Start the sensor reading thread
    char sensor_thread_stack[THREAD_STACKSIZE_MAIN];
    thread_create(sensor_thread_stack, sizeof(sensor_thread_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, read_sensor, NULL, "mma8x5x_reader");

    return 0;
}