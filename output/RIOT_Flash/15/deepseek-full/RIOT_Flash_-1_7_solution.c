
#include "esp_system.h"
#include "periph/rtc.h"
#include "ztimer.h"
#include "stddef.h"
#include "stdbool.h"
#include "periph/i2c.h"
#include "msg.h"
#include "evtimer_mbox.h"
#include "timex.h"
#include "kernel_defines.h"
#include "periph/gpio.h"
#include "netinet/in.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "time_units.h"
#include "net/sock/util.h"
#include "stdlib.h"
#include "sys/socket.h"
#include "mbox.h"
#include "stdio.h"
#include "xtimer.h"
#include "arpa/inet.h"
#include "string.h"
#include "net/emcute.h"
#include "unistd.h"
#include "stdatomic.h"
#include "mma8x5x_regs.h"
#include "net/sock/udp.h"

#define NVS_NAMESPACE "storage"
#define DEVICE_STATE_KEY "device_state"

static void store_state(bool state) {
    // Simulate storing state in NVS
    printf("Storing state: %d\n", state);
}

static bool read_state(void) {
    // Simulate reading state from NVS
    return false; // Default state
}

int main(void) {
    bool current_state = read_state();
    while (1) {
        // Toggle state
        current_state = !current_state;
        store_state(current_state);

        // Wait for 1 second
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return 0;
}
