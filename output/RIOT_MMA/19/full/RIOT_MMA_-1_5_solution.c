#include <ztimer.h>
#include <xtimer.h>
#include <unistd.h>
#include <timex.h>
#include <thread_config.h>
#include <thread.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <netinet/in.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/protnum.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <mbox.h>
#include <kernel_defines.h>
#include <event/thread.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <esp_system.h>
#include <dht_params.h>
#include <arpa/inet.h>

#define MMA8X5X_I2C_ADDRESS 0x1D  // Example I2C address

static mma8x5x_t dev;
static mma8x5x_data_t data;
static char stack[THREAD_STACKSIZE_MAIN];

void *read_mma8x5x(void *arg) {
    (void)arg;
    while(1) {
        mma8x5x_read(&dev, &data);
        printf("Acceleration x: %d mg, y: %d mg, z: %d mg\n", data.x, data.y, data.z);
        ztimer_sleep(ZTIMER_SEC, 5);
    }
    return NULL;
}

int main(void) {
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("Failed to initialize MMA8x5x");
        return 1;
    }

    kernel_pid_t pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                                      THREAD_CREATE_STACKTEST, read_mma8x5x, NULL, "mma_reader");

    (void)pid;  // Silence unused variable warning

    return 0;
}