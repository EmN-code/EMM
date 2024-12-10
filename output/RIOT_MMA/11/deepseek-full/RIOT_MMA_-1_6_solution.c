
#include "thread.h"
#include "msg.h"
#include "xtimer.h"
#include "mma8x5x.h"
#include "mma8x5x_params.h"

#define STACKSIZE 512
#define PRIORITY 5
#define MSG_QUEUE_SIZE 8

static char stack[STACKSIZE];
static msg_t msg_queue[MSG_QUEUE_SIZE];

static void accelerometer_thread(void *arg) {
    (void)arg;
    mma8x5x_t dev;
    mma8x5x_data_t data;
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Accelerometer initialization failed\n");
        return;
    }

    while (1) {
        if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {
            printf("Accelerometer data: x=%d, y=%d, z=%d\n", data.x, data.y, data.z);
        } else {
            printf("Failed to read accelerometer data\n");
        }
        xtimer_sleep(5);
    }
}

int main(void) {
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    kernel_pid_t pid = thread_create(stack, sizeof(stack), PRIORITY, 0, accelerometer_thread, NULL, "accelerometer_thread");
    if (pid <= KERNEL_PID_UNDEF) {
        printf("Failed to create accelerometer thread\n");
        return 1;
    }

    return 0;
}
