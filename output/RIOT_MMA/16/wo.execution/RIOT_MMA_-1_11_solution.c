#include "kernel_defines.h"
#include "stdbool.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdio.h"
#include "msg.h"
#include "xtimer.h"
#include "ztimer.h"
#include "timex.h"
#include "time_units.h"
#include "thread.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "mbox.h"
#include "mutex.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "periph/i2c.h"
#include "periph_conf.h"
#include "mqtt_client.h"
#include "mqtt_client_interface.h"
#include "paho_mqtt.h"
#include "dht_params.h"
#include "dht.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "sys/socket.h"
#include "arpa/inet.h"
#include "netinet/in.h"
#include "mma8x5x.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "unistd.h"
#include "periph/rtc.h"
#include "periph/gpio.h"
#include "stdatomic.h"

int main(void) {
    // Initialize device parameters
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };
    mma8x5x_t dev;
    int res = mma8x5x_init(&dev, &params);
    if (res != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x\n");
        return res;
    }

    // Periodically read from the accelerometer
    while (1) {
        mma8x5x_data_t data;
        mma8x5x_read(&dev, &data);
        printf("Acceleration [mg]: x=%d, y=%d, z=%d\n", data.x, data.y, data.z);
        xtimer_sleep(5);
    }
    return 0;
}