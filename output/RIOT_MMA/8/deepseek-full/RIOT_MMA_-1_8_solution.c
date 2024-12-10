
#include "periph/i2c.h"
#include "xtimer.h"
#include "mma8x5x.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "timex.h"
#include "thread.h"
#include "mbox.h"
#include "stdio.h"
#include "ztimer.h"
#include "msg.h"
#include "paho_mqtt.h"
#include "stddef.h"
#include "stdbool.h"
#include "dht.h"
#include "evtimer_mbox.h"
#include "kernel_defines.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "time_units.h"
#include "dht_params.h"
#include "stdlib.h"
#include "unistd.h"
#include "stdatomic.h"
#include "MQTTClient.h"
#include "periph/i2c.h"
#include "periph/timer.h"
#include "periph/uart.h"
#include "periph/spi.h"
#include "periph/adc.h"
#include "periph/cpuid.h"
#include "periph/flashpage.h"
#include "periph/rtc.h"
#include "periph/rtt.h"
#include "periph/spi_sync.h"
#include "periph/wdt.h"
#include "periph/pwm.h"
#include "periph/dac.h"
#include "periph/i2s.h"
#include "periph/can.h"
#include "net/gcoap.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "arpa/inet.h"
#include "net/emcute.h"
#include "net/sock/udp.h"

#define MMA8X5X_I2C_ADDRESS CONFIG_MMA8X5X_I2C_ADDRESS

static mma8x5x_t dev;
static mma8x5x_data_t data;

int main(void) {
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    int res = mma8x5x_init(&dev, &params);
    if (res != MMA8X5X_OK) {
        printf("MMA8x5x initialization failed\n");
        return 1;
    }

    while (1) {
        mma8x5x_read(&dev, &data);
        printf("Acceleration: X: %d mg, Y: %d mg, Z: %d mg\n", data.x, data.y, data.z);
        xtimer_sleep(5);
    }

    return 0;
}
