#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "thread.h"
#include "timex.h"
#include "stdio.h"
#include "stdbool.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdint.h"
#include "stdatomic.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "mqttclient.h"
#include "paho_mqtt.h"
#include "mutex.h"
#include "msg.h"
#include "mbox.h"
#include "mma8x5x.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "dht_params.h"
#include "dht.h"
#include "esp_system.h"
#include "architecture.h"
#include "kernel_defines.h"
#include "arpa/inet.h"
#include "time_units.h"

#define I2C_DEV 0
#define MMA8X5X_I2C_ADDRESS (0x1D)
#define MMA8X5X_RATE_50HZ 0x20
#define MMA8X5X_RANGE_2G  0x00
#define PERIODIC_SEND_TIME_S (5U)

static mma8x5x_t mma8_device;
static mma8x5x_data_t mma8_data;
mma8x5x_params_t mma8_params = {
    .i2c = I2C_DEV,
    .addr = MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

int main(void) {
    int res;

    // Initialize the accelerometer
    res = mma8x5x_init(&mma8_device, &mma8_params);
    if (res != 0) {
        printf("Failed to initialize MMA8x5x sensor. Error code: %d\n", res);
        return -1;
    }

    printf("MMA8x5x sensor initialized successfully.\n");

    // Network connection placeholder
    Network network;
    int rc;
    char* address_ip = "192.168.1.1";
    int port_number = 1883;

    // Connect network with retry mechanism
    while (1) {
        rc = NetworkConnect(&network, address_ip, port_number);
        if(rc != 0){
            printf("Network connect failed, retrying...\n");
            xtimer_sleep(1);
            continue;
        } else {
            printf("Network connected successfully.\n");
            break;
        }
    }

    while (1) {
        // Read data from the accelerometer
        mma8x5x_read(&mma8_device, &mma8_data);
        printf("X: %d mg, Y: %d mg, Z: %d mg\n", mma8_data.x, mma8_data.y, mma8_data.z);

        // Wait for the next cycle
        xtimer_sleep(PERIODIC_SEND_TIME_S);
    }

    return 0;
}