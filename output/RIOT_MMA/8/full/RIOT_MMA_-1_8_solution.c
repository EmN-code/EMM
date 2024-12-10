#include <ztimer.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdbool.h>
#include <MQTTClient.h>
#include <paho_mqtt.h>
#include <architecture.h>
#include <net/emcute.h>
#include <net/gcoap.h>
#include <mma8x5x_params.h>
#include <mma8x5x_regs.h>
#include <mma8x5x.h>
#include <xtimer.h>
#include <timex.h>
#include <dht_params.h>
#include <dht.h>
#include <mutex.h>
#include <kernel_defines.h>
#include <msg.h>
#include <thread.h>
#include <mbox.h>
#include <event/thread.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>

// Constants
#define INTERVAL (5000U)  // 5 seconds
#define I2C_DEV    (I2C_DEV(0))

// Definition for MMA8x5x parameters and device instance
typedef struct {
    uint8_t i2c;   /**< I2C bus the device is connected to */
    uint8_t addr;  /**< I2C address */
    uint8_t rate;  /**< Data rate */
    uint8_t range; /**< Measurement range */
} mma8x5x_params_t;

mma8x5x_t dev;
mma8x5x_params_t params = {
    .i2c = I2C_DEV,
    .addr = MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

// Function to read data from MMA8x5x accelerometer
void read_accelerometer_data(void) {
    mma8x5x_data_t data;
    mma8x5x_read(&dev, &data);
    printf("Accel Data [X, Y, Z]: [%d, %d, %d] mg\n", data.x, data.y, data.z);
}

int main(void) {
    int rc;

    // Initialize accelerometer
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("Initialization of MMA8x5x failed");
        return 1;
    }

    // Periodically read accelerometer data every 5 seconds
    while (1) {
        read_accelerometer_data();
        ztimer_sleep(ZTIMER_SEC, 5);  // Sleep for 5 seconds
    }

    return 0;
}
