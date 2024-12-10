#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "thread.h"
#include "msg.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "dht_params.h"
#include "mbox.h"
#include "event/thread.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "MQTTClient.h"
#include "mma8x5x.h"

static mma8x5x_t dev;
static mma8x5x_data_t data;
static mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

int init_accelerometer() {
    int rc;
    rc = mma8x5x_init(&dev, &params);
    if (rc == MMA8X5X_OK) {
        printf("Accelerometer initialized successfully\n");
    }
    else {
        printf("Failed to initialize accelerometer\n");
        return rc;
    }
    return MMA8X5X_OK;
}

int read_accelerometer() {
    while (1) {
        mma8x5x_read(&dev, &data);
        printf("X: %d mg, Y: %d mg, Z: %d mg\n", data.x, data.y, data.z);
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }
    return 0;
}

int connect_network() {
    int rc;
    int retries = 5; // Retry up to 5 times
    while (retries--) {
        rc = NetworkConnect(&network, "MQTT_BROKER_IP", MQTT_BROKER_PORT);
        if (rc != 0) {
            printf("Network connection failed. Retrying...\n");
            ztimer_sleep(ZTIMER_MSEC, 1000);
        } else {
            printf("Network connected successfully\n");
            break;
        }
    }
    return rc;
}

int main(void) {
    int rc;
    rc = init_accelerometer();
    if (rc != MMA8X5X_OK) {
        return rc;
    }

    rc = connect_network();
    if (rc != 0) {
        return rc;
    }

    read_accelerometer();
    return 0;
}