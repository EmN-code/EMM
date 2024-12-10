#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "thread.h"
#include "timex.h"
#include "time_units.h"
#include "struct_mgmt.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdio.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "architecture.h"
#include "arpa/inet.h"
#include "msg.h"
#include "mutex.h"
#include "mbox.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/emcute.h"
#include "kernel_defines.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "mma8x5x.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "dht_params.h"
#include "dht.h"

static mma8x5x_t dev;

int main(void) {
    // Initialize accelerometer driver
    const mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    int init_status = mma8x5x_init(&dev, &params);

    while (init_status != MMA8X5X_OK) {
        printf("Failed to initialize accelerometer. Retrying...\n");
        sleep(1);
        init_status = mma8x5x_init(&dev, &params);
    }

    printf("Accelerometer initialized successfully\n");

    mma8x5x_data_t data;

    while (1) {
        mma8x5x_read(&dev, &data);
        printf("Acceleration data - X: %d mg, Y: %d mg, Z: %d mg\n", data.x_axis, data.y_axis, data.z_axis);

        // Sleep for 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}