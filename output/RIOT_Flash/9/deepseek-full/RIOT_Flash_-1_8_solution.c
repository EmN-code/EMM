
#include "esp_system.h"
#include "ztimer.h"
#include "stddef.h"
#include "stdbool.h"
#include "mutex.h"
#include "msg.h"
#include "evtimer_mbox.h"
#include "nvs_flash.h"
#include "timex.h"
#include "kernel_defines.h"
#include "netinet/in.h"
#include "evtimer_msg.h"
#include "time_units.h"
#include "net/sock/util.h"
#include "stdlib.h"
#include "sys/socket.h"
#include "nvs.h"
#include "mbox.h"
#include "stdio.h"
#include "xtimer.h"
#include "arpa/inet.h"
#include "string.h"
#include "net/emcute.h"
#include "unistd.h"
#include "stdatomic.h"
#include "net/sock/udp.h"

#define DEVICE_STATE_KEY "device_state"

static void update_state(void) {
    static bool state = false;
    state = !state;

    // Simulate storing the state persistently
    printf("Updating device state to %d\n", state);
}

int main(void) {
    while (1) {
        update_state();
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    return 0;
}
