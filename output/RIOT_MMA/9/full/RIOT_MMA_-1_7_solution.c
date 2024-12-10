#include "ztimer.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "stddef.h"
#include "stdbool.h"
#include "stdio.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "paho_mqtt.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "architecture.h"
#include "arpa/inet.h"
#include "xtimer.h"
#include "unistd.h"
#include "thread.h"
#include "timex.h"
#include "time_units.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "dht_params.h"
#include "dht.h"
#include "MQTTClient.h"
#include "net/emcute.h"

#define THREAD_STACKSIZE_DEFAULT    (1024)
#define THREAD_PRIORITY_MAIN        (10)

static char stack[THREAD_STACKSIZE_DEFAULT];

static void *read_sensor_data(void *arg) {
    (void)arg;
    mma8x5x_t dev;
    mma8x5x_data_t data;
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != 0) {
        printf("Error: Unable to initialize MMA8x5x\n");
        return NULL;
    }
    
    while (1) {
        // Simulating the read function since direct reading code is not allowed
        mma8x5x_read(&dev, &data);
        printf("x: %d mg, y: %d mg, z: %d mg\n", data.x, data.y, data.z);
        ztimer_sleep(ZTIMER_MSEC, 5000);  // sleep for 5 seconds
    }
    return NULL;
}

int main(void) {

    kernel_pid_t pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN,
                                      THREAD_CREATE_STACKTEST, read_sensor_data,
                                      NULL, "sensor_thread");
    
    if (pid <= KERNEL_PID_UNDEF) {
        printf("Error: unable to create thread\n");
        return 1;
    }

    while (1) {
        xtimer_sleep(1); // keep the main thread alive
    }
    return 0;
}
