#include "ztimer.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "timex.h"
#include "time_units.h"
#include "thread.h"
#include "msg.h"
#include "paho_mqtt.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "dht_params.h"
#include "mbox.h"
#include "event/thread.h"
#include "arpa/inet.h"
#include "MQTTClient.h"

#define MMA8X5X_I2C_ADDRESS 0x1D  // Example address, update as required
#define MQTT_BROKER_IP "192.168.1.100"
#define MQTT_BROKER_PORT 1883

static mma8x5x_t dev;
static mma8x5x_data_t data;

int main(void) {
    printf("Initializing MMA8x5x sensor...\n");

    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize accelerometer\n");
        return 1;
    }

    Network network;
    int rc;

    while (1) {
        rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
        if (rc != 0) {
            printf("Network connection failed, retrying in 1 second...\n");
            ztimer_sleep(ZTIMER_SEC, 1);
            continue;
        }
        break;
    }

    printf("Connected to network successfully.\n");

    while (1) {
        mma8x5x_read(&dev, &data);
        printf("Acceleration X: %d mg, Y: %d mg, Z: %d mg\n", data.x, data.y, data.z);
        ztimer_sleep(ZTIMER_SEC, 5);
    }

    return 0;
}