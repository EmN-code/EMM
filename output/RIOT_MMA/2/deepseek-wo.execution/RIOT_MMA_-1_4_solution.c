
#include <net/sock/udp.h>
#include <assert.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/socket.h>
#include <event/thread.h>
#include <mma8x5x.h>
#include <event/periodic_callback.h>
#include <evtimer_mbox.h>
#include <periph/i2c.h>
#include <net/sock/util.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <timex.h>
#include <net/emcute.h>
#include <xtimer.h>
#include <periph/gpio.h>
#include <evtimer_msg.h>
#include <mbox.h>
#include <ztimer.h>
#include <dht_params.h>
#include <netinet/in.h>
#include <thread.h>
#include <kernel_defines.h>

#define MMA8X5X_I2C_ADDRESS (0x1C)
#define MMA8X5X_RATE_50HZ (0x0A)
#define MMA8X5X_RANGE_2G (0x00)

static mma8x5x_t dev;
mma8x5x_data_t data;

static void periodic_read(void) {
    mma8x5x_read(&dev, &data);
    printf("Acceleration: X: %d mg, Y: %d mg, Z: %d mg\n", data.x, data.y, data.z);
}

int main(void) {
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    int res = mma8x5x_init(&dev, &params);
    if (res != MMA8X5X_OK) {
        printf("Initialization failed with error code %d\n", res);
        return 1;
    }

    while (1) {
        periodic_read();
        xtimer_sleep(5);
    }

    return 0;
}
