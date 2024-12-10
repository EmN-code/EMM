#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "msg.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "mma8x5x_params.h"
#include "nvs_flash.h"
#include "event/thread.h"
#include "thread.h"
#include "nvs.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"

static mma8x5x_t dev;
static mma8x5x_data_t data;

static kernel_pid_t thread_pid;

void *read_accel_thread(void *arg) {
    (void)arg;
    while (1) {
        mma8x5x_read(&dev, &data);
        printf("Acceleration: X=%.2fmg Y=%.2fmg Z=%.2fmg\n", 
               data.x_axis / 1024.0 * 1000, 
               data.y_axis / 1024.0 * 1000, 
               data.z_axis / 1024.0 * 1000);
        ztimer_sleep(ZTIMER_SEC, 5);
    }
    return NULL;
}

int main(void) {
    nvs_flash_init();

    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("Failed to initialize accelerometer");
        return 1;
    }

    thread_pid = thread_create((char[THREAD_STACKSIZE_MAIN]){0}, THREAD_STACKSIZE_MAIN,
                               THREAD_PRIORITY_MAIN - 1, 0,
                               read_accel_thread, NULL, "accel_reader");

    return 0;
}