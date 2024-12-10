#include <ztimer.h>
#include <unistd.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdatomic.h>
#include <architecture.h>
#include <esp_system.h>
#include <esp_wifi.h>
#include <net/sock/util.h>
#include <paho_mqtt.h>
#include <MQTTClient.h>
#include <kernel_defines.h>
#include <net/emcute.h>
#include <net/gcoap.h>
#include <msg.h>
#include <xtimer.h>
#include <timex.h>
#include <dht.h>
#include <dht_params.h>
#include <mma8x5x.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <mutex.h>
#include <thread.h>
#include <mbox.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <event/thread.h>

static mma8x5x_t dev;
static mma8x5x_data_t data;
mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

int main(void) {
    int rc;

    printf("Initializing MMA8x5x\n");
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x\n");
        return -1;
    }

    while (1) {
        printf("Reading MMA8x5x data\n");
        mma8x5x_read(&dev, &data);
        printf("Acceleration: x=%d mg, y=%d mg, z=%d mg\n", data.x, data.y, data.z);

        // Sleep for 5 seconds
        ztimer_sleep(ZTIMER_SEC, 5);

        // Dummy network check (replace with actual network handling code)
        while (1) {
            rc = NetworkConnect(NULL, NULL, 0);  // adjust parameters as necessary
            if (rc != 0) {
                printf("Network connection failed. Retrying in 1 second.\n");
                ztimer_sleep(ZTIMER_SEC, 1);
                continue;
            }
            printf("Network connected successfully\n");
            break;
        }
    }

    return 0;
}