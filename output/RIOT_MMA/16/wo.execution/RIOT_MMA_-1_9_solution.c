#include "xtimer.h"
#include "ztimer.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdio.h"
#include "stdbool.h"
#include "stdatomic.h"
#include "sys/socket.h"
#include "unistd.h"
#include "timex.h"
#include "time_units.h"
#include "thread.h"
#include "paho_mqtt.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stdatomic.h"
#include "MQTTClient.h"
#include "msg.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "ztimer.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "pthread.h"
#include "periph_conf.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "netinet/in.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "mutex.h"
#include "esp_system.h"
#include "dht_params.h"
#include "dht.h"
#include "arpa/inet.h"

static mma8x5x_t dev;
mma8x5x_data_t data;

void *read_accel_thread(void *arg) {
    while (1) {
        mma8x5x_read(&dev, &data);
        printf("Accel X: %d mg, Y: %d mg, Z: %d mg\n", 
                data.x_axis, data.y_axis, data.z_axis);
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };
    
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x\n");
        return 1;
    }

    char read_stack[THREAD_STACKSIZE_DEFAULT];
    thread_create(read_stack, sizeof(read_stack), THREAD_PRIORITY_MAIN - 1,
                  0, read_accel_thread, NULL, "read_thread");

    return 0;
}