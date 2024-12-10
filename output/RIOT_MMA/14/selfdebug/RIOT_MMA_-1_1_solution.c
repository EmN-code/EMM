#include "thread_flags.h"
#include "thread_arch.h"
#include "thread_config.h"
#include "ble_hs_adv.h"
#include "ble_ibeacon.h"
#include "ble_hs.h"
#include "ble_gatt.h"
#include "hashes/sha2xx_common.h"
#include "net/sock/dtls.h"
#include "timer.h"
#include "mma8x5x.h"
#include "periph_conf.h"
#include "xtimer.h"

#define SLEEP_INTERVAL (5U * 1000000U) // 5 seconds in microseconds

static mma8x5x_t dev;
static mma8x5x_params_t params = {
    // Initialize with your specific parameters
    // Refer to your device's datasheet
};

static void read_mma8x5x(void) {
    int16_t x, y, z;
    if (mma8x5x_read(&dev, &x, &y, &z) == 0) {
        printf("X: %d, Y: %d, Z: %d\n", x, y, z);
    } else {
        puts("Failed to read acceleration data");
    }
}

int main(void) {
    if (mma8x5x_init(&dev, &params) != 0) {
        puts("Failed to initialize MMA8x5x sensor");
        return 1;
    }

    while (1) {
        read_mma8x5x();
        xtimer_usleep(SLEEP_INTERVAL);
    }

    return 0;
}