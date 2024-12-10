#include "stddef.h"
#include "unistd.h"
#include "stdio.h"
#include "time_units.h"
#include "thread_arch.h"
#include "thread.h"
#include "periph_conf.h"
#include "periph/rtc.h"
#include "periph/gpio.h"
#include "periph/i2c.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "mma8x5x_params.h"
#include "dht_params.h"
#include "architecture.h"
#include "event/thread.h"
#include "MQTTClient.h"

#define MAIN_QUEUE_SIZE    (8)

static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];
static mma8x5x_t dev;
static mma8x5x_data_t data;

int main(void) {
    puts("RIOT MMA8x5x example app");

    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };
    
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("Initialization failed");
        return 1;
    }
    
    while (1) {
        mma8x5x_read(&dev, &data);
        printf("Accel: x=%d mg, y=%d mg, z=%d mg\n", data.x, data.y, data.z);
        usleep(5000000); // 5 seconds
    }
    
    return 0;
}