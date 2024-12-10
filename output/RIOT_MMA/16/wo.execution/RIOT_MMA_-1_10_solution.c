#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <periph/i2c.h>
#include <msg.h>
#include <time_units.h>
#include <thread.h>
#include <arpa/inet.h>
#include <periph/rtc.h>
#include <mutex.h>
#include <kernel_defines.h>
#include <net/sock/udp.h>
#include <MQTTClient.h>
#include <evtimer_msg.h>
#include <dht_params.h>
#include <sys/socket.h>
#include <xtimer.h>
#include <dht.h>
#include <net/sock/util.h>
#include <periph/gpio.h>
#include <net/gcoap.h>
#include <paho_mqtt.h>
#include <ztimer.h>
#include <stddef.h>
#include <mbox.h>
#include <event/thread.h>
#include <net/emcute.h>
#include <timex.h>
#include <netinet/in.h>
#include <mma8x5x.h>
#include <unistd.h>
#include <mma8x5x_params.h>
#include <evtimer_mbox.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include <periph_conf.h>
#include <mma8x5x_regs.h>
#include <stdatomic.h>

#define I2C_DEVICE      I2C_DEV(0)
#define MMA8X5X_ADDR    0x1D 
#define READ_INTERVAL   (5U * US_PER_SEC)

static mma8x5x_t dev;
static mma8x5x_data_t data;

void *read_accelerometer(void *arg) {
    while (1) {
        mma8x5x_read(&dev, &data);
        printf("Acceleration: x=%dmg y=%dmg z=%dmg\n", data.x, data.y, data.z);
        xtimer_usleep(READ_INTERVAL);
    }
    return NULL;
}

int main(void) {
    mma8x5x_params_t params = {
        .i2c = I2C_DEVICE,
        .addr = MMA8X5X_ADDR,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("Failed to initialize MMA8x5x");
        return 1;
    }

    char stack[THREAD_STACKSIZE_DEFAULT];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                  read_accelerometer, NULL, "accel_thread");

    return 0;
}