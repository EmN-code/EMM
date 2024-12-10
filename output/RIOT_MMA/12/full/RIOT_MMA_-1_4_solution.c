#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "thread_flags.h"
#include "thread.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/protnum.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "msg.h"
#include "mbox.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "nvs_flash.h"
#include "host/ble_ibeacon.h"
#include "hashes/sha2xx_common.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "event/thread.h"
#include "dht_params.h"
#include "MQTTClient.h"
#include "time_units.h"

#define SLEEP_DURATION 5

static mma8x5x_t dev;
mma8x5x_data_t data;

void *read_accelerometer(void *arg) {
    (void)arg;
    while(1) {
        mma8x5x_read(&dev, &data);
        printf("Acceleration - X: %d mg, Y: %d mg, Z: %d mg\n", data.x, data.y, data.z);
        ztimer_sleep(ZTIMER_MSEC, SLEEP_DURATION * 1000);
    }
    return NULL;
}

int main(void) {
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = CONFIG_MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };
    
    int res = mma8x5x_init(&dev, &params);
    if (res != MMA8X5X_OK) {
        printf("Failed to initialize accelerometer. Error: %d\n", res);
        return -1;
    }

    char stack[THREAD_STACKSIZE_DEFAULT];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, read_accelerometer, NULL, "accelerometer_reader");

    return 0;
}