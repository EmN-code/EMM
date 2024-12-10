#include "ztimer.h"
#include "xtimer.h"
#include "time_units.h"
#include "timex.h"
#include "thread.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdatomic.h"
#include "stdio.h"
#include "stdbool.h"
#include "periph_conf.h"
#include "periph_rtc.h"
#include "periph_i2c.h"
#include "periph_gpio.h"
#include "netinet/in.h"
#include "net_gcoap.h"
#include "net_sock_udp.h"
#include "net_sock_util.h"
#include "net_emcute.h"
#include "mma8x5x.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "mbox.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "event_thread.h"
#include "dht.h"
#include "dht_params.h"
#include "MQTTClient.h"
#include "paho_mqtt.h"
#include "arpa/inet.h"
#include "kernel_defines.h"

#define ADDRESS CONFIG_MMA8X5X_I2C_ADDRESS

static mma8x5x_t dev;
static mma8x5x_data_t data;

typedef struct {
    i2c_t i2c;
    uint8_t addr;
    uint8_t rate;
    uint8_t range;
} mma8x5x_params_t;

mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

static void read_accelerometer(void) {
    while (1) {
        int res = mma8x5x_read(&dev, &data);
        if (res == 0) {
            printf("Acceleration: X=%dmg Y=%dmg Z=%dmg\n", data.x, data.y, data.z);
        } else {
            printf("Error reading accelerometer\n");
        }
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }
}

int main(void) {
    if (mma8x5x_init(&dev, &params) == 0) {
        printf("MMA8x5x initialized successfully!\n");
    } else {
        printf("Failed to initialize MMA8x5x\n");
        return 1;
    }
    read_accelerometer();
    return 0;
}