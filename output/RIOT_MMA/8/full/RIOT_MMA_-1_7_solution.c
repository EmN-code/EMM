#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "thread.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdint.h"
#include "stdlib.h"
#include "stdio.h"
#include "mma8x5x.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "dht.h"
#include "dht_params.h"
#include "msg.h"
#include "mutex.h"
#include "mbox.h"
#include "event/thread.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"
#include "net/emcute.h"
#include "net/sock/util.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include "sys/socket.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "architecture.h"
#include "kernel_defines.h"
#include "unistd.h"
#include "esp_system.h"

#define I2C_ADDRESS CONFIG_MMA8X5X_I2C_ADDRESS
#define READ_INTERVAL (5U * US_PER_SEC) // 5 seconds

static mma8x5x_t dev;
static mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

void read_sensor_data(void) {
    mma8x5x_data_t data;
    while (1) {
        if (mma8x5x_read(&dev, &data) == 0) {
            printf("Acceleration [mg]: X=%d, Y=%d, Z=%d\n", data.x, data.y, data.z);
        } else {
            printf("Failed to read acceleration data\n");
        }
        ztimer_sleep(ZTIMER_USEC, READ_INTERVAL);
    }
}

int main(void) {
    printf("Initializing MMA8x5x sensor...\n");
    while (1) {
        int init_result = mma8x5x_init(&dev, &params);
        if (init_result == MMA8X5X_OK) {
            printf("Sensor initialized successfully.\n");
            break;
        } else {
            printf("Sensor initialization failed with code %d. Retrying...\n", init_result);
            xtimer_sleep(1);
        }
    }
    read_sensor_data();
    return 0;
}